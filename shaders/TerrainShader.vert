#version 450

layout(location = 0) in vec3 position;  // Vertex position
layout(location = 1) in vec3 normal;    // Vertex normal
layout(location = 2) in vec2 texCoord;

layout(location = 0) out vec3 fragNormal;
layout(location = 1) out vec4 fragPosition;

layout(binding = 0) uniform CameraBuffer {
    mat4 c_viewProjection;
    vec3 c_cameraPosition;
};

void main() {
    vec4 posVec4 = vec4(position, 1.0);

    vec4 worldPos = vec4(posVec4.x * 20, posVec4.y * 20, posVec4.z * 20, 1.0);
    
    fragNormal = c_cameraPosition * normal;
    gl_Position = c_viewProjection * worldPos;
    fragPosition = gl_Position;
}
