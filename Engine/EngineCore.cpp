#include "stdafx.h"
#include "EngineCore.h"
#include "Application/WinApplication.h"
#include <vulkanControl.h>
#include <Input\InputManager.h>



bool EngineCore::Initialize(void* gamePtr)
{
    
}

void EngineCore::Update()
{
    WinApplication::Get()->Update();

}

void EngineCore::BeginFrame()
{
    VulkanControl* vulkanController = VulkanControl::Get();
    vulkanController->BeginRenderer();
}

void EngineCore::Render()
{
}

void EngineCore::EndFrame()
{
}

void EngineCore::Shutdown()
{
}
