#include "FocuseChangedEventHandler.h"


std::atomic<bool> bURLBarFocused(false);
CComPtr<IUIAutomationElement> FocusChangedEventHandler::_currentFocuse = nullptr;
FocusChangedEventHandler::FocusChangedEventHandler(CComPtr<IUIAutomation> automation) : _refCount(1), _automation(automation) {}

FocusChangedEventHandler::~FocusChangedEventHandler() {}

ULONG FocusChangedEventHandler::AddRef() {
    return InterlockedIncrement(&_refCount);
}

ULONG FocusChangedEventHandler::Release() {
    long val = InterlockedDecrement(&_refCount);
    if (val == 0)
    {
        delete this;
        return 0;
    }
    return val;
}

HRESULT FocusChangedEventHandler::QueryInterface(REFIID riid, void** ppInterface) {
    if (riid == __uuidof(IUnknown))
        *ppInterface = static_cast<IUIAutomationFocusChangedEventHandler*>(this);
    else if (riid == __uuidof(IUIAutomationFocusChangedEventHandler))
        *ppInterface = static_cast<IUIAutomationFocusChangedEventHandler*>(this);
    else
    {
        *ppInterface = NULL;
        return E_NOINTERFACE;
    }
    this->AddRef();
    return S_OK;
}

HRESULT FocusChangedEventHandler::HandleFocusChangedEvent(IUIAutomationElement* sender) {
    if (sender == nullptr) {
        bURLBarFocused = false;
        return S_OK;
    }
    _currentFocuse = sender;
    CONTROLTYPEID controlType;
    sender->get_CurrentControlType(&controlType);

    if (controlType == UIA_EditControlTypeId) {
        CComVariant varAccessKey;
        sender->GetCurrentPropertyValue(UIA_AccessKeyPropertyId, &varAccessKey);
        if (varAccessKey.vt == VT_BSTR && wcscmp(varAccessKey.bstrVal, L"Ctrl+L") == 0) {
            Sleep(1000);
            bURLBarFocused.store(true);
            std::wcout << L"URL bar is focused." << std::endl;
        }
        else {
            bURLBarFocused.store(false);
        }
    }
    else {
        bURLBarFocused.store(false);
    }

    return S_OK;
}


bool FocusChangedEventHandler::ModifyURL()
{
    CComVariant var;
    if (FAILED(_currentFocuse->GetCurrentPropertyValue(UIA_ValueValuePropertyId, &var))) return false;
    if (var.vt != VT_BSTR || !var.bstrVal) return false;

    // Convert BSTR to std::wstring
    std::wstring url_string(var.bstrVal, SysStringLen(var.bstrVal));
    std::wcout << L"Current URL: " << url_string << std::endl;

    // Check if URL starts with "http://" or "https://"
    if (url_string.find(L"http://") == 0 || url_string.find(L"https://") == 0) {
        std::wcout << L"URL already contains http/https scheme. Modification not required." << std::endl;
        return true; // No modification required, return true
    }

    // Prefix "test:" to the URL if it does not start with "http://" or "https://"
    std::wstring modifiedUrl = L"test:" + url_string;
    CComBSTR newBstrVal(modifiedUrl.c_str());
    CComPtr<IValueProvider> valueProvider;
    HRESULT hr = _currentFocuse->GetCurrentPattern(UIA_ValuePatternId, reinterpret_cast<IUnknown**>(&valueProvider));
    if (SUCCEEDED(hr) && valueProvider) {
        // Set the modified URL
        valueProvider->SetValue(newBstrVal);
        std::wcout << L"URL modified to: " << modifiedUrl << std::endl;
    }

    bURLBarFocused.store(false); // Reset the flag

    // Optional: Simulate Enter key press to navigate after URL modification
    INPUT input[2] = {};
    input[0].type = input[1].type = INPUT_KEYBOARD;
    input[0].ki.wVk = input[1].ki.wVk = VK_RETURN;
    input[1].ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(ARRAYSIZE(input), input, sizeof(INPUT));

    return true;
}
