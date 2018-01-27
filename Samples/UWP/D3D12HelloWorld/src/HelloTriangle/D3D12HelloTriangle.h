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

// Note that while ComPtr is used to manage the lifetime of resources on the CPU,
// it has no understanding of the lifetime of resources on the GPU. Apps must account
// for the GPU lifetime of resources to avoid destroying objects that may still be
// referenced by the GPU.
// An example of this can be found in the class method: OnDestroy().
using Microsoft::WRL::ComPtr;

class D3D12HelloTriangle : public DXSample
{
public:
    using DXSample::DXSample;

	void OnInit() override;
	void OnUpdate() override;
	void OnRender() override;
	void OnDestroy() override;

private:
    static constexpr UINT FrameCount{ 2 };

	struct Vertex
	{
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT4 color;
	};

	// Pipeline objects.
    CD3DX12_VIEWPORT m_viewport{ 0.0f, 0.0f, static_cast<float>(m_width), static_cast<float>(m_height) };
    CD3DX12_RECT m_scissorRect{ 0, 0, static_cast<LONG>(m_width), static_cast<LONG>(m_height) };
	winrt::com_ptr<IDXGISwapChain3> m_swapChain;
    winrt::com_ptr<ID3D12Device> m_device;
    winrt::com_ptr<ID3D12Resource> m_renderTargets[FrameCount];
    winrt::com_ptr<ID3D12CommandAllocator> m_commandAllocator;
    winrt::com_ptr<ID3D12CommandQueue> m_commandQueue;
    winrt::com_ptr<ID3D12RootSignature> m_rootSignature;
    winrt::com_ptr<ID3D12DescriptorHeap> m_rtvHeap;
    winrt::com_ptr<ID3D12PipelineState> m_pipelineState;
    winrt::com_ptr<ID3D12GraphicsCommandList> m_commandList;
    UINT m_rtvDescriptorSize{ 0 };

	// App resources.
    winrt::com_ptr<ID3D12Resource> m_vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;

	// Synchronization objects.
    UINT m_frameIndex{ 0 };
	winrt::handle m_fenceEvent;
    winrt::com_ptr<ID3D12Fence> m_fence;
	UINT64 m_fenceValue;

	void LoadPipeline();
	void LoadAssets();
	void PopulateCommandList();
	void WaitForPreviousFrame();
};
