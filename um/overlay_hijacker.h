#pragma once
#include <windows.h>
#include <dwmapi.h>
#include <string>
#include <iostream>

#pragma comment(lib, "dwmapi.lib")

class OverlayHijacker {
public:
    static HWND FindDiscordOverlay() {
        HWND currentDiscord = FindWindowW(L"DiscordOverlayWindow", NULL);
        if (!currentDiscord) currentDiscord = FindWindowW(L"Chrome_WidgetWin_1", L"Discord Overlay"); 
        
        if (!currentDiscord) {
            struct SearchData { HWND hFound; };
            SearchData data = { NULL };
            EnumWindows([](HWND hwnd, LPARAM lParam) -> BOOL {
                SearchData* d = (SearchData*)lParam;
                wchar_t className[256]; GetClassNameW(hwnd, className, 256);
                if (wcscmp(className, L"Chrome_WidgetWin_1") == 0) {
                    wchar_t title[256]; GetWindowTextW(hwnd, title, 256);
                    if (wcsstr(title, L"Discord Overlay") != NULL || wcsstr(title, L"Discord") != NULL) {
                        RECT r; GetWindowRect(hwnd, &r);
                        if ((r.right - r.left) > 100 && (r.bottom - r.top) > 100) { 
                            d->hFound = hwnd; 
                            return FALSE; 
                        }
                    }
                }
                return TRUE;
            }, (LPARAM)&data);
            currentDiscord = data.hFound;
        }

        return currentDiscord;
    }

    // hijacked window correct styles
    static void PrepareHijackedWindow(HWND hWnd) {
        if (!hWnd) return;
        
        // transparent and layered
        LONG exStyle = GetWindowLong(hWnd, GWL_EXSTYLE);
        SetWindowLong(hWnd, GWL_EXSTYLE, exStyle | WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOPMOST);
        
    }
};

