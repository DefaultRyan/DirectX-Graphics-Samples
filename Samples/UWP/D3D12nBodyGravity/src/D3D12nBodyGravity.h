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
#include "SimpleCamera.h"
#include "StepTimer.h"

// Note that while ComPtr is used to manage the lifetime of resources on the CPU,
// it has no understanding of the lifetime of resources on the GPU. Apps must account
// for the GPU lifetime of resources to avoid destroying objects that may still be
// referenced by the GPU.
// An example of this can be found in the class method: OnDestroy().
using Microsoft::WRL::ComPtr;

class D3D12nBodyGravity : public DXSample
{
public:
	D3D12nBodyGravity(UINT width, UINT height, std::wstring name);

	void OnInit() override;
	void OnUpdate() override;
	void OnRender() override;
	void OnDestroy() override;
	void OnKeyDown(winrt::Windows::System::VirtualKey key) override;
	void OnKeyUp(winrt::Windows::System::VirtualKey key) override;

private:
	static constexpr UINT FrameCount = 2;
	static constexpr UINT ThreadCount = 1;
	static constexpr float ParticleSpread = 400.0f;
	static constexpr UINT ParticleCount = 10000;		// The number of particles in the n-body simulation.

	// "Vertex" definition for particles. Triangle vertices are generated 
	// by the geometry shader. Color data will be assigned to those 
	// vertices via this struct.
	struct ParticleVertex
	{
        DirectX::XMFLOAT4 color;
	};

	// Position and velocity data for the particles in the system.
	// Two buffers full of Particle data are utilized in this sample.
	// The compute thread alternates writing to each of them.
	// The render thread renders using the buffer that is not currently
	// in use by the compute shader.
	struct Particle
	{
        DirectX::XMFLOAT4 position;
        DirectX::XMFLOAT4 velocity;
	};

	struct ConstantBufferGS
	{
        DirectX::XMFLOAT4X4 worldViewProjection;
        DirectX::XMFLOAT4X4 inverseView;

		// Constant buffers are 256-byte aligned in GPU memory. Padding is added
		// for convenience when computing the struct's size.
		float padding[32];
	};

	struct ConstantBufferCS
	{
		UINT param[4];
		float paramf[4];
	};

	// Pipeline objects.
    CD3DX12_VIEWPORT m_viewport{ 0.0f, 0.0f, static_cast<float>(m_width), static_cast<float>(m_height) };
    CD3DX12_RECT m_scissorRect{ 0, 0, static_cast<LONG>(m_width), static_cast<LONG>(m_height) };
	winrt::com_ptr<IDXGISwapChain3> m_swapChain;
    winrt::com_ptr<ID3D12Device> m_device;
    winrt::com_ptr<ID3D12Resource> m_renderTargets[FrameCount];
    UINT m_frameIndex{ 0 };
    winrt::com_ptr<ID3D12CommandAllocator> m_commandAllocators[FrameCount];
    winrt::com_ptr<ID3D12CommandQueue> m_commandQueue;
    winrt::com_ptr<ID3D12RootSignature> m_rootSignature;
    winrt::com_ptr<ID3D12RootSignature> m_computeRootSignature;
    winrt::com_ptr<ID3D12DescriptorHeap> m_rtvHeap;
    winrt::com_ptr<ID3D12DescriptorHeap> m_srvUavHeap;
    UINT m_rtvDescriptorSize{ 0 };
    UINT m_srvUavDescriptorSize{ 0 };

	// Asset objects.
    winrt::com_ptr<ID3D12PipelineState> m_pipelineState;
    winrt::com_ptr<ID3D12PipelineState> m_computeState;
    winrt::com_ptr<ID3D12GraphicsCommandList> m_commandList;
    winrt::com_ptr<ID3D12Resource> m_vertexBuffer;
    winrt::com_ptr<ID3D12Resource> m_vertexBufferUpload;
	D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
    winrt::com_ptr<ID3D12Resource> m_particleBuffer0[ThreadCount];
    winrt::com_ptr<ID3D12Resource> m_particleBuffer1[ThreadCount];
    winrt::com_ptr<ID3D12Resource> m_particleBuffer0Upload[ThreadCount];
    winrt::com_ptr<ID3D12Resource> m_particleBuffer1Upload[ThreadCount];
    winrt::com_ptr<ID3D12Resource> m_constantBufferGS;
    UINT8* m_pConstantBufferGSData{ nullptr };
    winrt::com_ptr<ID3D12Resource> m_constantBufferCS;

    UINT m_srvIndex[ThreadCount]{};		// Denotes which of the particle buffer resource views is the SRV (0 or 1). The UAV is 1 - srvIndex.
	UINT m_heightInstances;
	UINT m_widthInstances;
	SimpleCamera m_camera;
	StepTimer m_timer;

	// Compute objects.
    winrt::com_ptr<ID3D12CommandAllocator> m_computeAllocator[ThreadCount];
    winrt::com_ptr<ID3D12CommandQueue> m_computeCommandQueue[ThreadCount];
    winrt::com_ptr<ID3D12GraphicsCommandList> m_computeCommandList[ThreadCount];

	// Synchronization objects.
	winrt::handle m_swapChainEvent;
    winrt::com_ptr<ID3D12Fence> m_renderContextFence;
    UINT64 m_renderContextFenceValue{ 0 };
	winrt::handle m_renderContextFenceEvent;
    UINT64 m_frameFenceValues[FrameCount]{};

    winrt::com_ptr<ID3D12Fence> m_threadFences[ThreadCount];
	winrt::handle m_threadFenceEvents[ThreadCount];

	// Thread state.
    LONG volatile m_terminating{ 0 };
	UINT64 volatile m_renderContextFenceValues[ThreadCount];
	UINT64 volatile m_threadFenceValues[ThreadCount];

	struct ThreadData
	{
		D3D12nBodyGravity* pContext;
		UINT threadIndex;
	};
	ThreadData m_threadData[ThreadCount];
	winrt::handle m_threadHandles[ThreadCount];

	// Indices of the root signature parameters.
	enum GraphicsRootParameters : UINT32
	{
		GraphicsRootCBV = 0,
		GraphicsRootSRVTable,
		GraphicsRootParametersCount
	};

	enum ComputeRootParameters : UINT32
	{
		ComputeRootCBV = 0,
		ComputeRootSRVTable,
		ComputeRootUAVTable,
		ComputeRootParametersCount
	};

	// Indices of shader resources in the descriptor heap.
	enum DescriptorHeapIndex : UINT32
	{
		UavParticlePosVelo0 = 0,
		UavParticlePosVelo1 = UavParticlePosVelo0 + ThreadCount,
		SrvParticlePosVelo0 = UavParticlePosVelo1 + ThreadCount,
		SrvParticlePosVelo1 = SrvParticlePosVelo0 + ThreadCount,
		DescriptorCount = SrvParticlePosVelo1 + ThreadCount
	};

	void LoadPipeline();
	void LoadAssets();
	void CreateAsyncContexts();
	void CreateVertexBuffer();
	float RandomPercent();
	void LoadParticles(_Out_writes_(numParticles) Particle* pParticles, const DirectX::XMFLOAT3 &center, const DirectX::XMFLOAT4 &velocity, float spread, UINT numParticles);
	void CreateParticleBuffers();
	void PopulateCommandList();

	static DWORD WINAPI ThreadProc(ThreadData* pData)
	{
		return pData->pContext->AsyncComputeThreadProc(pData->threadIndex);
	}
	DWORD AsyncComputeThreadProc(int threadIndex);
	void Simulate(UINT threadIndex);

	void WaitForRenderContext();
	void MoveToNextFrame();
};
