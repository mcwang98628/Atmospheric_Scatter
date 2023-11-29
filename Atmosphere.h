#pragma once
#include <glm/vec3.hpp>

typedef struct Sun
{
    glm::vec3 sunDir;
    glm::vec3 I_sun;    // Intensity of the sun
    float sunAngle;
    
}Sun;

typedef struct Atmosphere
{
    // alignas(4) float R_e;      // Radius of the planet [m]
    // alignas(4) float R_a;      // Radius of the atmosphere [m]
    // alignas(16) glm::vec3  beta_R;   // Rayleigh scattering coefficient
    // alignas(4) float beta_M;   // Mie scattering coefficient
    // alignas(4) float H_R;      // Rayleigh scale height
    // alignas(4) float H_M;      // Mie scale height
    // alignas(4) float g;        // Mie scattering direction - 
    //  - anisotropy of the medium
    
    alignas(4) float toneMappingFactor;    ///< Whether tone mapping is applied
    
    glm::vec3 scatterRayleigh = glm::vec3(5.8e-3f, 13.5e-3f, 33.1e-3f);
    float scatterMie = 21e-3f;
    
    float  hDensityRayleigh = 8;
    float hDensityMie  = 1.2f;

    float asymmetryMie = 0.8f;
    
    float  planetRadius = 6360;
    float  atmosphereRadius = 6460;

    glm::vec3 absorbOzone = glm::vec3(0.65f, 1.881f, 0.085f);
    float  ozoneCenterHeight = 25;

    float  ozoneThickness = 30;
}Atmosphere;
