#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

layout(location = 0) out vec3 fsPosition;
layout(location = 1) out vec3 fsNormal;
layout(location = 2) out vec2 fsTexCoord;

layout(binding = 0) uniform UniformBufferObject {
    mat4 M;     // Model matrix
    mat4 MVP;   // Model - View - Projection matrix

// TODO other constants
    vec3 viewPos;   // Position of the viewer
    vec3 sunPos;    // Position of the sun, light direction

// Number of samples along the view ray and light ray
    int viewSamples;
    int lightSamples;

    float I_sun;    // Intensity of the sun
    float R_e;      // Radius of the planet [m]
    float R_a;      // Radius of the atmosphere [m]
    vec3  beta_R;   // Rayleigh scattering coefficient
    float beta_M;   // Mie scattering coefficient
    float H_R;      // Rayleigh scale height
    float H_M;      // Mie scale height
    float g;        // Mie scattering direction - 
//  - anisotropy of the medium

    float toneMappingFactor;    ///< Whether tone mapping is applied
} ubo;

void main()
{
    vec4 posVec4 = vec4(position, 1.0);
    fsPosition = vec3(ubo.M * posVec4);
	gl_Position = ubo.MVP * posVec4;

    // <UNUSED>
    // TODO precompute normal matrix
    //mat3 N = transpose(inverse(mat3(M)));
    //fsNormal = normalize(N * normal);
    //fsNormal = normalize(vec4(M * vec4(position, 0.0)).xyz);

    // <UNUSED>
    //fsTexCoord = texCoord;
}

