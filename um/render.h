#pragma once
#include <d3d11.h>
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_win32.h"
#include "../imgui/imgui_impl_dx11.h"
#include "esp.h"
#include "overlay_hijacker.h"

class Renderer {
public:
    bool Initialize(HWND hWnd);
    void BeginFrame();
    void EndFrame();
    void RenderESP();
    void Cleanup();

    ID3D11Device* GetDevice() { return pd3dDevice; }

private:
    ID3D11Device* pd3dDevice = nullptr;
    ID3D11DeviceContext* pd3dDeviceContext = nullptr;
    IDXGISwapChain* pSwapChain = nullptr;
    ID3D11RenderTargetView* mainRenderTargetView = nullptr;

    void CreateRenderTarget();
    void CleanupRenderTarget();
};

extern Renderer* renderer;
