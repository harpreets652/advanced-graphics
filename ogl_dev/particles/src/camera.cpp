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
            moveForward_relative(positionStepSize);
            break;
        }
        case DOWN: {
            moveBackward_relative(positionStepSize);
            break;
        }
        case LEFT: {
            moveLeft_relative(positionStepSize);
            break;
        }
        case RIGHT: {
            moveRight_relative(positionStepSize);
            break;
        }
    }

    updateViewMatrix();
}

void Camera::updateDirection(Direction direction) {
    switch (direction) {
        case UP: {
            tiltUpward();
            break;
        }
        case DOWN: {
            tiltDownward();
            break;
        }
        case LEFT: {
            pivotLeft_aroundFocus(positionStepSize);
            break;
        }
        case RIGHT: {
            pivotRight_aroundFocus(positionStepSize);
            break;
        }
    }

    updateViewMatrix();
}

void Camera::moveForward_relative(double distance) {
    glm::vec3 direction = float(distance) * glm::normalize(focus - position);
    position += direction;
    focus += direction;
    updateViewMatrix();
}

void Camera::moveBackward_relative(double distance) {
    glm::vec3 direction = float(distance) * glm::normalize(focus - position);
    position -= direction;
    focus -= direction;
    updateViewMatrix();
}

void Camera::moveLeft_relative(double distance) {
    glm::vec3 cameraDirection = focus - position;
    cameraDirection.y = 0;
    glm::vec3 direction = float(distance) * glm::normalize(
            glm::cross(glm::vec3(0.0, 1.0, 0.0), cameraDirection)
    );
    position += direction;
    focus += direction;
    updateViewMatrix();
}

void Camera::moveRight_relative(double distance) {
    glm::vec3 cameraDirection = focus - position;
    cameraDirection.y = 0;
    glm::vec3 direction = float(distance) * glm::normalize(
            glm::cross(glm::vec3(0.0, 1.0, 0.0), cameraDirection)
    );
    position -= direction;
    focus -= direction;
    updateViewMatrix();
}

void Camera::pivotLeft_aroundFocus(double distance) {
    glm::vec3 direction = float(distance) * glm::normalize(
            glm::cross(glm::vec3(0.0, 1.0, 0.0), focus - position)
    );
    position += direction;
    updateViewMatrix();
}

void Camera::pivotRight_aroundFocus(double distance) {
    glm::vec3 direction = float(distance) * glm::normalize(
            glm::cross(glm::vec3(0.0, 1.0, 0.0), focus - position)
    );
    position -= direction;
    updateViewMatrix();
}

void Camera::tiltDownward() {
    focus.y -= DEFAULT_TRANSLATION_DISTANCE;
    updateViewMatrix();
}

void Camera::tiltUpward() {
    focus.y += DEFAULT_TRANSLATION_DISTANCE;
    updateViewMatrix();
}
void Camera::updateViewMatrix() {
    view = glm::lookAt(position, //Eye Position
                       focus, //Focus point
                       up);//Positive Y is up
}


