#include "render.h"
#include "memory.h"
#include "v_math.h"

#include <string>

bool Renderer::Initialize(HWND hWnd) {
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &pSwapChain, &pd3dDevice, &featureLevel, &pd3dDeviceContext) != S_OK)
        return false;

    CreateRenderTarget();

    ImGui::CreateContext();
    // DX11 rendering.
    ImGui_ImplDX11_Init(pd3dDevice, pd3dDeviceContext);

    // Initial size setup
    RECT rect;
    GetClientRect(hWnd, &rect);
    ImGui::GetIO().DisplaySize = ImVec2((float)(rect.right - rect.left), (float)(rect.bottom - rect.top));

    return true;

}

void Renderer::BeginFrame() {
    // Update display size in case window was resized
    IDXGISwapChain* swapChain = pSwapChain;
    if (swapChain) {
        ID3D11Texture2D* pBackBuffer = nullptr;
        if (SUCCEEDED(swapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer)))) {
            D3D11_TEXTURE2D_DESC desc;
            pBackBuffer->GetDesc(&desc);
            ImGui::GetIO().DisplaySize = ImVec2((float)desc.Width, (float)desc.Height);
            pBackBuffer->Release();
        }
    }

    ImGui_ImplDX11_NewFrame();
    // ImGui_ImplWin32_NewFrame() is skipped
    ImGui::NewFrame();
}


void Renderer::RenderESP() {
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::Begin("##ESP", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoInputs);

    auto drawList = ImGui::GetBackgroundDrawList();
    
    uintptr_t clientBase = mem->GetModuleAddress(L"client.dll");

    ViewMatrix viewMatrix = mem->Read<ViewMatrix>(clientBase + LunixOffsets::dwViewMatrix);

    for (const auto& entity : esp->GetEntities()) {
        Vector2 screenFoot, screenHead;
        if (WorldToScreen(entity.origin, screenFoot, viewMatrix, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y) &&
            WorldToScreen(entity.head, screenHead, viewMatrix, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y)) {
            
            float height = std::abs(screenFoot.y - screenHead.y);

            float width = height / 2.0f;

            ImColor color = (entity.team == 2) ? ImColor(255, 0, 0) : ImColor(0, 0, 255); // Red for T, Blue for CT approx

            drawList->AddRect(ImVec2(screenHead.x - width / 2, screenHead.y), ImVec2(screenHead.x + width / 2, screenFoot.y), color);
            
            // Health bar
            float healthHeight = height * (entity.health / 100.0f);
            drawList->AddRectFilled(ImVec2(screenHead.x - width / 2 - 5, screenFoot.y - healthHeight), ImVec2(screenHead.x - width / 2 - 2, screenFoot.y), ImColor(0, 255, 0));
        } else {
        }
    }


    ImGui::End();
}

void Renderer::EndFrame() {
    ImGui::Render();
    const float clear_color_with_alpha[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    pd3dDeviceContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
    pd3dDeviceContext->ClearRenderTargetView(mainRenderTargetView, clear_color_with_alpha);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    pSwapChain->Present(1, 0); 
}

void Renderer::Cleanup() {
    ImGui_ImplDX11_Shutdown();
    // ImGui_ImplWin32_Shutdown() skipped
    ImGui::DestroyContext();

    CleanupRenderTarget();
    if (pSwapChain) pSwapChain->Release();
    if (pd3dDeviceContext) pd3dDeviceContext->Release();
    if (pd3dDevice) pd3dDevice->Release();
}

void Renderer::CreateRenderTarget() {
    ID3D11Texture2D* pBackBuffer;
    pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
    pBackBuffer->Release();
}

void Renderer::CleanupRenderTarget() {
    if (mainRenderTargetView) mainRenderTargetView->Release();
}

Renderer* renderer = new Renderer();
