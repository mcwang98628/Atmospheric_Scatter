#version 450 core

#define M_PI 3.1415926535897932384626433832795

layout(location = 0) in vec3 fsPosition;     // Position of the fragment

layout(location = 0) out vec4 finalColor;

layout(binding = 0) uniform UniformBufferObject {
    mat4 M;     // Model matrix
    mat4 MVP;   // Model - View - Projection matrix

// TODO other constants
    vec3 viewPos;   // Position of the viewer
    vec3 sunPos;    // Position of the sun, light direction

// Number of samples along the view ray and light ray
    int viewSamples;
    int lightSamples;

    vec3 I_sun;    // Intensity of the sun
    float R_e;      // Radius of the planet [m]
    float R_a;      // Radius of the atmosphere [m]
    vec3  beta_R;   // Rayleigh scattering coefficient
    float beta_M;   // Mie scattering coefficient
    float absorb_M;
    float H_R;      // Rayleigh scale height
    float H_M;      // Mie scale height
    float g;        // Mie scattering direction - 
//  - anisotropy of the medium

    float toneMappingFactor;    ///< Whether tone mapping is applied
} ubo;

/**
 * @brief Computes intersection between a ray and a sphere
 * @param o Origin of the ray
 * @param d Direction of the ray
 * @param r Radius of the sphere
 * @return Roots depending on the intersection
 */
vec2 raySphereIntersection(vec3 o, vec3 d, float r)
{
    // Solving analytically as a quadratic function
    //  assumes that the sphere is centered at the origin
    // f(x) = a(x^2) + bx + c
    float a = dot(d, d);
    float b = 2.0 * dot(d, o);
    float c = dot(o, o) - r * r;

    // Discriminant or delta
    float delta = b * b - 4.0 * a * c;

    // Roots not found
    if (delta < 0.0) {
      // TODO
      return vec2(1e5, -1e5);
    }

    float sqrtDelta = sqrt(delta);
    // TODO order??
    return vec2((-b - sqrtDelta) / (2.0 * a),
                (-b + sqrtDelta) / (2.0 * a));
}

/**
 * @brief Function to compute color of a certain view ray
 * @param ray Direction of the view ray
 * @param origin Origin of the view ray
 * @return color of the view ray
 */
vec3 computeSkyColor(vec3 ray, vec3 origin)
{
    // Normalize the light direction
    vec3 sunDir = normalize(ubo.sunPos);

    vec2 t = raySphereIntersection(origin, ray, ubo.R_a);
    // Intersects behind
    if (t.x > t.y) {
        return vec3(0.0, 0.0, 0.0);
    }

    // Distance between samples - length of each segment
    t.y = min(t.y, raySphereIntersection(origin, ray, ubo.R_e).x);
    float segmentLen = (t.y - t.x) / float(ubo.viewSamples);

    // TODO t min
    float tCurrent = 0.0f; 

    // Rayleigh and Mie contribution
    vec3 sum_R = vec3(0);
    vec3 sum_M = vec3(0);
    vec3 sumSigmaT = vec3(0);

    // Optical depth 
    float optDepth_R = 0.0;
    float optDepth_M = 0.0;

    // Mu: the cosine angle between the sun and ray direction
    float mu = dot(ray, sunDir);
    float mu_2 = mu * mu;
    
    //--------------------------------
    // Rayleigh and Mie Phase functions
    float phase_R = 3.0 / (16.0 * M_PI) * (1.0 + mu_2);

    float g_2 = ubo.g * ubo.g;
    float phase_M = 3.0 / (8.0 * M_PI) * 
                          ((1.0 - g_2) * (1.0 + mu_2)) / 
                          ((2.0 + g_2) * pow(1.0 + g_2 - 2.0 * ubo.g * mu, 1.5));
    
    
    // Sample along the view ray
    for (int i = 0; i < ubo.viewSamples; ++i)
    {
        // Middle point of the sample position
        vec3 vSample = origin + ray * (tCurrent + segmentLen * 0.5);

        // Height of the sample above the planet
        float height = length(vSample) - ubo.R_e;
        
        // Optical depth for Rayleigh and Mie scattering for current sample
        float h_R = exp(-height / ubo.H_R) * segmentLen;
        float h_M = exp(-height / ubo.H_M) * segmentLen;
        optDepth_R += h_R;
        optDepth_M += h_M;

        //--------------------------------
        // Secondary - light ray
        float segmentLenLight = 
            raySphereIntersection(vSample, sunDir, ubo.R_a).y / float(ubo.lightSamples);
        float tCurrentLight = 0.0;

        // Light optical depth 
        float optDepthLight_R = 0.0;
        float optDepthLight_M = 0.0;

        // Sample along the light ray
        for (int j = 0; j < ubo.lightSamples; ++j)
        {
            // Position of the light ray sample
            vec3 lSample = vSample + sunDir * 
                           (tCurrentLight + segmentLenLight * 0.5);
            // Height of the light ray sample
            float heightLight = length(lSample) - ubo.R_e;

            // TODO check sample above the ground
            
            optDepthLight_R += exp(-heightLight / ubo.H_R) * segmentLenLight;
            optDepthLight_M += exp(-heightLight / ubo.H_M) * segmentLenLight;

            // Next light sample
            tCurrentLight += segmentLenLight;
        }
        // TODO check sample above ground

        vec3 sigma_R = ubo.beta_R * h_R;
        float mieDensity = h_M;
        float sigma_mieS = ubo.beta_M  * mieDensity;
        float sigma_mieT = (ubo.beta_M  + ubo.absorb_M) * mieDensity;
        vec3 ozone = vec3(0.65e-3f, 1.881e-3f, 0.085e-3f) * max(0.0f, 1 - 0.5 * abs(height - 25) / 15);

        vec3 sigmaS = sigma_R + sigma_mieS;
        vec3 sigmaT = sigma_R + sigma_mieT + ozone;
        
        vec3 eyeTrans = exp(-sumSigmaT - 0.5 * sigmaT);

        // Attenuation of the light for both Rayleigh and Mie optical depth
        //  Mie extenction coeff. = 1.1 of the Mie scattering coeff.
        vec3 att = exp(-(ubo.beta_R * (optDepth_R + optDepthLight_R) +
        ubo.beta_M * 1.1f * (optDepth_M + optDepthLight_M)));
        
        // Accumulate the scattering 
        sum_R += h_R * att;
        sum_M += h_M * att;

        // Next view sample
        tCurrent += segmentLen;
        // sumSigmaT += deltaSumSigmaT;
    }

//    return ubo.I_sun * (sum_R * ubo.beta_R * phase_R + sum_M * ubo.beta_M * phase_M);
    return ubo.I_sun * (sum_R * ubo.beta_R * phase_R + sum_M * ubo.beta_M * phase_M);
}

void main()
{
    vec3 acolor = computeSkyColor(normalize(fsPosition - ubo.viewPos), ubo.viewPos);

    // Apply tone mapping
    acolor = mix(acolor, (1.0 - exp(-1.0 * acolor)), ubo.toneMappingFactor);

    finalColor = vec4(acolor, 1.0);
}
