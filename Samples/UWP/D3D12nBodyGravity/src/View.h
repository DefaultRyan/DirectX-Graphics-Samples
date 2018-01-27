//*********************************************************
//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************

#pragma once

#include "DXSample.h"

struct View final : winrt::implements<View, winrt::Windows::ApplicationModel::Core::IFrameworkView>
{
    explicit View(DXSample* pSample)
        : m_pSample(pSample)
    {}

    void Initialize(winrt::Windows::ApplicationModel::Core::CoreApplicationView const& applicationView);
    void SetWindow(winrt::Windows::UI::Core::CoreWindow const& window);
    void Load(winrt::hstring const& entryPoint);
    void Run();
    void Uninitialize();

private:
    void OnActivated(winrt::Windows::ApplicationModel::Core::CoreApplicationView const& applicationView, winrt::Windows::ApplicationModel::Activation::IActivatedEventArgs const& args);
    void OnKeyDown(winrt::Windows::UI::Core::CoreWindow const& window, winrt::Windows::UI::Core::KeyEventArgs const& keyEventArgs);
    void OnKeyUp(winrt::Windows::UI::Core::CoreWindow const& window, winrt::Windows::UI::Core::KeyEventArgs const& keyEventArgs);
    void OnClosed(winrt::Windows::UI::Core::CoreWindow const& sender, winrt::Windows::UI::Core::CoreWindowEventArgs const& args);

    DXSample * m_pSample;
    bool m_windowClosed = false;
};
