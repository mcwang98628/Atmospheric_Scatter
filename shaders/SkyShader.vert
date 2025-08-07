#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

layout(location = 0) out vec4 fsPosition;
layout(location = 1) out vec4 worldPosition;



layout(binding = 0) uniform CameraBuffer {
    mat4 c_viewProjection;
    vec3 c_cameraPosition;
};

void main()
{
    vec4 posVec4 = vec4(position, 1.0);
    worldPosition = vec4(posVec4.x * 100, posVec4.y * 100, posVec4.z * 100, 1.0);
    gl_Position = c_viewProjection * worldPosition;
    fsPosition = gl_Position;
}
