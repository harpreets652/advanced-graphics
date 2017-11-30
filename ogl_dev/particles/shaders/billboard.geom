#version 330

// incoming topology is points, output topology are triangle strips
layout(points) in;
layout(triangle_strip) out;

// We will emit no more than four vertices.
// Keyword used to give the graphics driver a hint about the maximum number of vertices that can be emitted by the GS.
// Knowning the limit in advance gives the driver an opportunity to optimize the behavior of the GS for the particular case
layout(max_vertices = 4) out;

//input position is already in world space so only projectionViewMat is required
uniform mat4 projectViewMatrix;
uniform vec3 cameraPosition;
uniform float gBillboardSize;

// generates texture coordinates for the Frag Shader
out vec2 TexCoord;

void main() {
    // built-in variable 'gl_in' variable is an array of structures
    // input topology is point list so there is only a single vertex
    // if the input topology was a triangle we could have written 'gl_in[0]', 'gl_in[1]' and 'gl_in[2]' for the three vertices
    vec3 billboardPosition = gl_in[0].gl_Position.xyz;

    vec3 toCamera = normalize(cameraPosition - billboardPosition);
    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 right = cross(toCamera, up) * gBillboardSize;

    // left bottom of the quad
    billboardPosition -= right;
    gl_Position = projectViewMatrix * vec4(billboardPosition, 1.0);
    TexCoord = vec2(0.0, 1.0);
    // after this call, gl_position and TexCoord are cleared and undefined so we can set them again and emit another vertex
    EmitVertex();

    // top left of the quad
    billboardPosition.y += gBillboardSize;
    gl_Position = projectViewMatrix * vec4(billboardPosition, 1.0);
    TexCoord = vec2(0.0, 0.0);
    EmitVertex();

    // bottom right of the quad
    billboardPosition.y -= gBillboardSize;
    billboardPosition += right;
    gl_Position = projectViewMatrix * vec4(billboardPosition, 1.0);
    TexCoord = vec2(1.0, 1.0);
    EmitVertex();

    // top right of the quad
    billboardPosition.y += gBillboardSize;
    gl_Position = projectViewMatrix * vec4(billboardPosition, 1.0);
    TexCoord = vec2(1.0, 0.0);
    EmitVertex();

    // to end  the triangle strip, built-in function
    EndPrimitive();
}