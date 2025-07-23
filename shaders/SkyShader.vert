#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

layout(location = 0) out vec4 fsPosition;

layout(binding = 0) uniform CameraBuffer {
    mat4 transform;
    mat4 viewProjection;
};

void main()
{
    vec4 posVec4 = vec4(position, 1.0);
    vec4 worldPos = transform * posVec4;
    worldPos = vec4(worldPos.x * 100, worldPos.y * 100, worldPos.z * 100, 1.0);
    gl_Position = viewProjection * worldPos;
    fsPosition = gl_Position;
}
