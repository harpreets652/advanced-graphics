#version 330

layout(points) in;
layout(triangle_strip) out;
layout(max_vertices = 4) out;

uniform mat4 projectViewMatrix;
uniform vec3 cameraPosition;

out vec2 TexCoord;

void main() {
    vec3 boardPosition = gl_in[0].gl_Position.xyz;
    vec3 toCamera = normalize(cameraPosition - boardPosition);
    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 right = cross(toCamera, up);

    boardPosition -= (right * 0.5);
    gl_Position = projectViewMatrix * vec4(boardPosition, 1.0);
    TexCoord = vec2(0.0, 0.0);
    EmitVertex();

    boardPosition.y += 1.0;
    gl_Position = projectViewMatrix * vec4(boardPosition, 1.0);
    TexCoord = vec2(0.0, 1.0);
    EmitVertex();

    boardPosition.y -= 1.0;
    boardPosition += right;
    gl_Position = projectViewMatrix * vec4(boardPosition, 1.0);
    TexCoord = vec2(1.0, 0.0);
    EmitVertex();

    boardPosition.y += 1.0;
    gl_Position = projectViewMatrix * vec4(boardPosition, 1.0);
    TexCoord = vec2(1.0, 1.0);
    EmitVertex();

    EndPrimitive();
}