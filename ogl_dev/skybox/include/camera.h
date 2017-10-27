#ifndef CAMERA_H
#define CAMERA_H

#include "graphics_headers.h"
#include "Direction.h"

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

    //Note~ camera movement incomplete
    void updatePosition(Direction direction);

    void updateDirection(Direction direction);

private:
    void updateViewMatrix();
    void initOrientation();
    void updateFocusAndUp();

    glm::vec3 position;
    glm::vec3 focus;
    glm::vec3 up;

    float horizontalAngle;
    float verticalAngle;

    glm::mat4 projection;
    glm::mat4 view;
};

#endif /* CAMERA_H */
