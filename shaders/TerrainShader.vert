#version 450

layout(location = 0) in vec3 position;  // Vertex position
layout(location = 1) in vec3 normal;    // Vertex normal
layout(location = 2) in vec2 texCoord;

layout(location = 0) out vec3 fragNormal;
layout(location = 1) out vec3 fragPosition;

layout(binding = 0) uniform CameraBuffer {
    mat4 transform;
    mat4 view;
    mat4 projection;
};

void main() {
    vec4 worldPos = transform * vec4(position, 1.0);
    
    fragPosition = worldPos.xyz;
    fragNormal = mat3(transpose(inverse(transform))) * normal;
    gl_Position = projection * view * worldPos;
}
