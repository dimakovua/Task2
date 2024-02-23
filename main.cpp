#include "FocuseChangedEventHandler.h"


extern std::atomic<bool> bURLBarFocused;
static HHOOK g_keyboardHook = nullptr; // Global hook handle


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_DESTROY:
        // User closed the window, post the WM_QUIT message with a 0 exit code
        PostQuitMessage(0);
        return 0;
        // Handle other messages here...
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        auto p = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);

        if (wParam == WM_KEYDOWN && p->vkCode == VK_RETURN) {
            std::cout << "Enter key pressed" << std::endl;
            if (bURLBarFocused.load()) {
                FocusChangedEventHandler::ModifyURL();
            }
        }
    }
    return CallNextHookEx(nullptr, nCode, wParam, lParam);
}


void SetKeyboardHook()
{
    g_keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, NULL, 0);
    if (g_keyboardHook == NULL)
    {
        std::cerr << "Failed to install keyboard hook!" << std::endl;
    }
}

void RemoveKeyboardHook()
{
    if (g_keyboardHook != NULL)
    {
        UnhookWindowsHookEx(g_keyboardHook);
        g_keyboardHook = NULL;
    }
}


int main() {
    SetKeyboardHook();

    // Initialize COM library
    HRESULT hr;
    int ret = 0;
    CComPtr<IUIAutomationElement> pTargetElement = nullptr;
    CComPtr<IUIAutomationFocusChangedEventHandler> eventHandler = nullptr;

    hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if (FAILED(hr)) return 1;
    CComPtr<IUIAutomation> pAutomation = NULL;
    hr = CoCreateInstance(__uuidof(CUIAutomation), NULL, CLSCTX_INPROC_SERVER, __uuidof(IUIAutomation), (void**)&pAutomation);
    if (FAILED(hr) || pAutomation == NULL)
    {
        CoUninitialize();
        return 1;
    }
    eventHandler = new FocusChangedEventHandler(pAutomation);
    hr = pAutomation->AddFocusChangedEventHandler(nullptr, eventHandler);

    if (FAILED(hr))
    {
        CoUninitialize();
        return 1;
    }
    SetKeyboardHook();
    // Main message loop
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    //Clean Up
    hr = pAutomation->RemoveFocusChangedEventHandler(eventHandler);
    if (FAILED(hr))
    {
        CoUninitialize();
        return 1;
    }
    RemoveKeyboardHook();

    return 0;
}
