#version 450 core
#define M_PI 3.1415926535897932384626433832795

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

vec3 computeSkyColor(vec3 ray, vec3 origin)
{
    // Normalize the light direction
    vec3 sunDir = normalize(sunBuffer.sunPos);

    vec2 t = raySphereIntersection(origin, ray, atmosphereConstants.atmosphereRadius);
    // Intersects behind
    if (t.x > t.y) {
        return vec3(0.0, 0.0, 0.0);
    }

    // Distance between samples - length of each segment
    t.y = min(t.y, raySphereIntersection(origin, ray, atmosphereConstants.planetRadius).x);
    float segmentLen = (t.y - t.x) / float(atmosphereConstants.viewSamples);

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

    float g_2 = atmosphereConstants.anisotropy * atmosphereConstants.anisotropy;
    float phase_M = 3.0 / (8.0 * M_PI) * 
                          ((1.0 - g_2) * (1.0 + mu_2)) / 
                          ((2.0 + g_2) * pow(1.0 + g_2 - 2.0 * atmosphereConstants.anisotropy * mu, 1.5));


    // Sample along the view ray
    for (int i = 0; i < atmosphereConstants.viewSamples; ++i)
    {
        // Middle point of the sample position
        vec3 vSample = origin + ray * (tCurrent + segmentLen * 0.5);

        // Height of the sample above the planet
        float height = length(vSample) - atmosphereConstants.planetRadius;

        // Optical depth for Rayleigh and Mie scattering for current sample
        float h_R = exp(-height / atmosphereConstants.rayleighScaleHeight) * segmentLen;
        float h_M = exp(-height / atmosphereConstants.mieScaleHeight) * segmentLen;
        optDepth_R += h_R;
        optDepth_M += h_M;

        //--------------------------------
        // Secondary - light ray
        float segmentLenLight = 
            raySphereIntersection(vSample, sunDir, atmosphereConstants.atmosphereRadius).y / float(atmosphereConstants.lightSamples);
        float tCurrentLight = 0.0;

        // Light optical depth 
        float optDepthLight_R = 0.0;
        float optDepthLight_M = 0.0;

        // Sample along the light ray
        for (int j = 0; j < atmosphereConstants.lightSamples; ++j)
        {
            // Position of the light ray sample
            vec3 lSample = vSample + sunDir * 
                           (tCurrentLight + segmentLenLight * 0.5);
            // Height of the light ray sample
            float heightLight = length(lSample) - atmosphereConstants.planetRadius;

            // TODO check sample above the ground

            optDepthLight_R += exp(-heightLight / atmosphereConstants.rayleighScaleHeight) * segmentLenLight;
            optDepthLight_M += exp(-heightLight / atmosphereConstants.mieScaleHeight) * segmentLenLight;

            // Next light sample
            tCurrentLight += segmentLenLight;
        }
        // TODO check sample above ground

        vec3 sigma_R = atmosphereConstants.scatterRayleigh * h_R;
        float mieDensity = h_M;
        float sigma_mieS = atmosphereConstants.scatterMie  * mieDensity;
        float sigma_mieT = (atmosphereConstants.scatterMie  + atmosphereConstants.absorbMie) * mieDensity;
        vec3 ozone = vec3(0.65e-3f, 1.881e-3f, 0.085e-3f) * max(0.0f, 1 - 0.5 * abs(height - 25) / 15);

        vec3 sigmaS = sigma_R + sigma_mieS;
        vec3 sigmaT = sigma_R + sigma_mieT + ozone;

        vec3 eyeTrans = exp(-sumSigmaT - 0.5 * sigmaT);

        // Attenuation of the light for both Rayleigh and Mie optical depth
        //  Mie extenction coeff. = 1.1 of the Mie scattering coeff.
        vec3 att = exp(-(atmosphereConstants.scatterRayleigh * (optDepth_R + optDepthLight_R) +
        atmosphereConstants.scatterMie * 1.1f * (optDepth_M + optDepthLight_M)));

        // Accumulate the scattering 
        sum_R += h_R * att;
        sum_M += h_M * att;

        // Next view sample
        tCurrent += segmentLen;
        // sumSigmaT += deltaSumSigmaT;
    }

//    return ubo.I_sun * (sum_R * ubo.beta_R * phase_R + sum_M * ubo.beta_M * phase_M);
    return atmosphereConstants.sunIntensity * (sum_R * atmosphereConstants.scatterRayleigh * phase_R + sum_M * atmosphereConstants.scatterMie * phase_M);
}

void main() {
    vec4 worldPosition = cameraBuffer.transform * vec4(fsPosition, 1.0);
    vec3 acolor = computeSkyColor(normalize(fsPosition - worldPosition.xyz), worldPosition.xyz);

    acolor = mix(acolor, (1.0 - exp(-1.0 * acolor)), 1.0);

    finalColor = vec4(acolor, 1.0);
    // finalColor = vec4(0.0, 0.0, 1.0, 1.0);
}
