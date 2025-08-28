#pragma once
#include "stdafx.h"
#include "GameObject.h"
#include "engineMath.h"

class Atmosphere : public GameObject
{
public:
    Atmosphere();
    ~Atmosphere();

    struct AtmosphereBuffer
    {   
        Vector3 SunIntensity = Vector3(20.f, 20.f, 20.f);    // Intensity of the sun
        int viewSamples = 16;
        
        Vector3 scatterRayleigh = Vector3(5.8e-3f, 13.5e-3f, 33.1e-3f);
        float scatterMie = 21e-3f;

        int lightSamples = 8;
        float absorbMie = 4.4f;
        float  planetRadius = 6360;
        float  atmosphereRadius = 6460;

        float rayleighScaleHeight = 7.994f;
        float mieScaleHeight = 1.2f;
        float anisotropy = 0.888f;
        float padding;
    };

    struct SunBuffer
    {
        Vector3 sunDir = Vector3(0.f, .707f, -.707f);
        float padding;
    };

    void Update(float deltaTime) override;
    void Draw(VkCommandBuffer commandBuffer, uint32_t currentFrame) override;
    void UpdateDescriptorSets() override;
    void UpdateSun(float deltaSun);
    void CreateUniformBuffers() override;


private:
    AtmosphereBuffer m_atmosphereBuffer;
    SunBuffer m_sunBuffer;

    float sunAngle = .785;

    std::vector<VkBuffer>        atmosphereBuffer;
    std::vector<VkDeviceMemory>  atmosphereBufferMemory;
    std::vector<void*>           atmosphereBufferMapped;


    std::vector<VkBuffer>        sunBuffer;
    std::vector<VkDeviceMemory>  sunBufferMemory;
    std::vector<void*>           sunBufferMapped;
};