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
#include "View.h"

using namespace winrt;
using namespace winrt::Windows::ApplicationModel;
using namespace winrt::Windows::ApplicationModel::Activation;
using namespace winrt::Windows::ApplicationModel::Core;
using namespace winrt::Windows::UI::Core;
using namespace winrt::Windows::UI::ViewManagement;


void View::Initialize(CoreApplicationView const& applicationView)
{
    applicationView.Activated({ this, &View::OnActivated });

    // For simplicity, this sample ignores CoreApplication's Suspend and Resume
    // events which a typical app should subscribe to.
}

void View::SetWindow(CoreWindow const& window)
{
    window.KeyDown({ this, &View::OnKeyDown });
    window.KeyUp({ this, &View::OnKeyUp });
    window.Closed({ this, &View::OnClosed });

    // For simplicity, this sample ignores a number of events on CoreWindow that a
    // typical app should subscribe to.
}

void View::Load(hstring const& /*entryPoint*/)
{
}

void View::Run()
{
    auto applicationView = ApplicationView::GetForCurrentView();
    applicationView.Title(m_pSample->GetTitle());

    m_pSample->OnInit();

    while (!m_windowClosed)
    {
        CoreWindow::GetForCurrentThread().Dispatcher().ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);

        m_pSample->OnUpdate();
        m_pSample->OnRender();
    }

    m_pSample->OnDestroy();
}

void View::Uninitialize()
{
}

void View::OnActivated(CoreApplicationView const& /*applicationView*/, IActivatedEventArgs const& args)
{
    CoreWindow::GetForCurrentThread().Activate();
}

void View::OnKeyDown(CoreWindow const& /*window*/, KeyEventArgs const& args)
{
    m_pSample->OnKeyDown(args.VirtualKey());
}

void View::OnKeyUp(CoreWindow const& /*window*/, KeyEventArgs const& args)
{
    m_pSample->OnKeyUp(args.VirtualKey());
}

void View::OnClosed(CoreWindow const& /*sender*/, CoreWindowEventArgs const& /*args*/)
{
    m_windowClosed = true;
}
