#version 450 core

layout(location = 0) in vec3 fsPosition;     // Position of the fragment
layout(location = 0) out vec4 finalColor;

layout(binding = 0) uniform CameraBuffer {
    mat4 transform;
    mat4 view;
    mat4 projection;
} cameraBuffer;

layout(binding = 1) uniform AtmosphereConstants {
    int viewSamples;
    int lightSamples;
    vec3 sunIntensity;
    vec3 scatterRayleigh;
    float scatterMie;
    float absorbMie;
    float planetRadius;
    float atmosphereRadius;
    float rayleighScaleHeight;
    float mieScaleHeight;
    float anisotropy;
} atmosphereConstants;

layout(binding = 2) uniform SunBuffer {
    vec3 sunPos;
} sunBuffer;

void main() {
    // Just output a simple color for testing
    finalColor = vec4(0.0, 0.0, 1.0, 1.0);
}
