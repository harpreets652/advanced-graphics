#include "camera.h"


float Camera::positionStepSize = 0.5f;
float Camera::panStepSize = 0.05f;

Camera::Camera() {

}

Camera::~Camera() {

}

bool Camera::Initialize(int w, int h) {
    position = glm::vec3(0.0, 2.0, 10.0);
    focus = glm::vec3(0.1, 0.1, 0.0);
    up = glm::vec3(0.0, 1.0, 0.0);

    updateViewMatrix();

    projection = glm::perspective(45.0f, //the FoV typically 90 degrees is good which is what this is set to
                                  float(w) / float(h), //Aspect Ratio, so Circles stay Circular
                                  0.01f, //Distance to the near plane, normally a small value like this
                                  100.0f); //Distance to the far plane,
    return true;
}

void Camera::initOrientation() {
    glm::vec3 hTarget(focus.x, 0.0, focus.z);
    hTarget = glm::normalize(hTarget);

    if (hTarget.z >= 0.0f) {
        if (hTarget.x >= 0.0f) {
            horizontalAngle = 360.0f - glm::degrees(glm::asin(hTarget.z));
        } else {
            horizontalAngle = 180.0f + glm::degrees(glm::asin(hTarget.z));
        }
    } else {
        if (hTarget.x >= 0.0f) {
            horizontalAngle = glm::degrees(glm::asin(-hTarget.z));
        } else {
            horizontalAngle = 180.0f - glm::degrees(glm::asin(-hTarget.z));
        }
    }

    verticalAngle = -glm::degrees(glm::asin(focus.y));
}

glm::mat4 Camera::GetProjection() {
    return projection;
}

glm::mat4 Camera::GetView() {
    return view;
}

glm::vec3 Camera::getPosition() {
    return position;
}

void Camera::updatePosition(Direction direction) {
    switch (direction) {
        case UP: {
            position.y += positionStepSize;
            break;
        }
        case DOWN: {
            position.y -= positionStepSize;
            break;
        }
        case LEFT: {
            position.x -= positionStepSize;
            break;
        }
        case RIGHT: {
            position.x += positionStepSize;
            break;
        }
    }

    updateViewMatrix();
}

void Camera::updateDirection(Direction direction) {
    switch (direction) {
        case UP:
            verticalAngle += panStepSize;
            break;
        case DOWN:
            verticalAngle -= panStepSize;
            break;
        case LEFT:
            horizontalAngle -= panStepSize;
            break;
        case RIGHT:
            horizontalAngle += panStepSize;
            break;
    }

    updateViewMatrix();
}

void Camera::updateFocusAndUp() {
    const glm::vec3 vAxis(0.0f, 1.0f, 0.0f);

    glm::vec3 tempView(1.0f, 0.0f, 0.0f);
    tempView = glm::rotate(tempView, horizontalAngle, vAxis);
    tempView = glm::normalize(tempView);

    // Rotate the view vector by the vertical angle around the horizontal axis
    glm::vec3 hAxis = glm::cross(vAxis, tempView);
    hAxis = glm::normalize(hAxis);
    tempView = glm::rotate(tempView, verticalAngle, hAxis);
    tempView = glm::normalize(tempView);

    focus = tempView;

    up = glm::cross(focus, hAxis);
    up = glm::normalize(up);
}

void Camera::updateViewMatrix() {
    view = glm::lookAt(position, //Eye Position
                       focus, //Focus point
                       up);//Positive Y is up
}


