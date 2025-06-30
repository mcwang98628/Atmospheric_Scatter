#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

layout(location = 0) out vec3 fsPosition;

layout(binding = 0) uniform CameraBuffer {
    mat4 transform;
    mat4 view;
    mat4 projection;
};

void main()
{
    vec4 posVec4 = vec4(position * 20.0, 1.0);
    vec4 worldPos = transform * posVec4;
    fsPosition = worldPos.xyz;
    gl_Position = projection * view * worldPos;
}
