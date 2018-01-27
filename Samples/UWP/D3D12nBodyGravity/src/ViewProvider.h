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

struct ViewProvider final : winrt::implements<ViewProvider, winrt::Windows::ApplicationModel::Core::IFrameworkViewSource>
{
    explicit ViewProvider(DXSample* pSample)
        : m_pSample(pSample)
    {}
    winrt::Windows::ApplicationModel::Core::IFrameworkView CreateView();

private:
    DXSample * m_pSample;
};
