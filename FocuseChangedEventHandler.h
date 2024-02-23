#pragma once
#include <Windows.h>
#include <AtlBase.h>
#include <AtlCom.h>
#include <UIAutomation.h>
#include <string>
#include <iostream>
#include <atomic>

#define UNICODE

class FocusChangedEventHandler : public IUIAutomationFocusChangedEventHandler {
    long volatile _refCount;
    CComPtr<IUIAutomation> _automation;
    static CComPtr<IUIAutomationElement> _currentFocuse;

public:
    FocusChangedEventHandler(CComPtr<IUIAutomation> automation);
    virtual ~FocusChangedEventHandler();

    // IUnknown methods
    IFACEMETHODIMP_(ULONG) AddRef();
    IFACEMETHODIMP_(ULONG) Release();
    IFACEMETHODIMP QueryInterface(REFIID riid, void** ppInterface);

    // IUIAutomationFocusChangedEventHandler method
    IFACEMETHODIMP HandleFocusChangedEvent(IUIAutomationElement* sender);

    static bool ModifyURL();
};