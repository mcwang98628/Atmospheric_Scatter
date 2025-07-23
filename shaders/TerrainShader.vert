#version 450

layout(location = 0) in vec3 position;  // Vertex position
layout(location = 1) in vec3 normal;    // Vertex normal
layout(location = 2) in vec2 texCoord;

layout(location = 0) out vec3 fragNormal;
layout(location = 1) out vec4 fragPosition;

layout(binding = 0) uniform CameraBuffer {
    mat4 transform;
    mat4 viewProjection;
};

void main() {
    vec4 worldPos = transform * vec4(position, 1.0);
    worldPos = vec4(worldPos.x * 20, worldPos.y * 20, worldPos.z * 20, 1.0);
    
    fragNormal = mat3(transpose(inverse(transform))) * normal;
    gl_Position = viewProjection * worldPos;
    fragPosition = gl_Position;
}
