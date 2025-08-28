#pragma once
#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif
class EngineCore {
public:
    static  bool Initialize(void* gamePtr);
    static ENGINE_API void Update();

    static ENGINE_API void BeginFrame();
    static ENGINE_API void Render();
    static ENGINE_API void EndFrame();

    static ENGINE_API void Shutdown();
};