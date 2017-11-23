#ifndef CAMERA_H
#define CAMERA_H

#include "graphics_headers.h"
#include "Direction.h"

static const double DEFAULT_TRANSLATION_DISTANCE = 0.5;

class Camera {
public:
    static float positionStepSize;
    static float panStepSize;

    Camera();

    ~Camera();

    bool Initialize(int w, int h);

    glm::mat4 GetProjection();

    glm::mat4 GetView();

    glm::vec3 getPosition();

    void updatePosition(Direction direction);

    void updateDirection(Direction direction);

private:
    void moveForward_relative(double distance);

    void moveBackward_relative(double distance);

    void moveLeft_relative(double distance);

    void moveRight_relative(double distance);

    void pivotLeft_aroundFocus(double distance);

    void pivotRight_aroundFocus(double distance);

    void tiltDownward();

    void tiltUpward();

    void updateViewMatrix();

    glm::vec3 position;
    glm::vec3 focus;
    glm::vec3 up;

    float horizontalAngle;
    float verticalAngle;

    glm::mat4 projection;
    glm::mat4 view;
};

#endif /* CAMERA_H */
