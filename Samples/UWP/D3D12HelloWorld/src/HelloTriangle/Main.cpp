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

#include "stdafx.h"
#include "ViewProvider.h"
#include "D3D12HelloTriangle.h"

using namespace winrt;

int __stdcall wWinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    init_apartment();
	D3D12HelloTriangle sample(1200, 900, L"");
    auto viewProvider = make<ViewProvider>(&sample);
    winrt::Windows::ApplicationModel::Core::CoreApplication::Run(viewProvider);
	return 0;
}
