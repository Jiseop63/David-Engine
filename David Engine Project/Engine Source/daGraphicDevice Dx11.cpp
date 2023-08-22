#include "daGraphicDevice Dx11.h"
#include "daApplication.h"
#include "daRenderer.h"

extern da::Application application;

namespace da::graphics
{
	GraphicDevice_Dx11::GraphicDevice_Dx11()
	{
		// window handle
		HWND hWnd = application.GetHwnd();

		// Device, Context 생성
		D3D_FEATURE_LEVEL featureLevel = (D3D_FEATURE_LEVEL)0;
		D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr
			, D3D11_CREATE_DEVICE_DEBUG, nullptr, 0
			, D3D11_SDK_VERSION
			, mDevice.GetAddressOf(), &featureLevel
			, mContext.GetAddressOf());

		UINT clientWidth = application.GetClientWidth();
		UINT clientHeight = application.GetClientHeight();

		// create swapchain
		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
		swapChainDesc.BufferCount = 2;
		swapChainDesc.BufferDesc.Width = clientWidth;
		swapChainDesc.BufferDesc.Height = clientHeight;
		if (!CreateSwapChain(&swapChainDesc, hWnd))
			return;

		mRenderTarget = std::make_shared<Texture>();
		mDepthStencil = std::make_shared<Texture>();

		// get rendertarget RenderTarget & RenderTargetView by swapchain
		// RenderTarget만들기
		Microsoft::WRL::ComPtr<ID3D11Texture2D> renderTarget = nullptr;		
		if (FAILED(mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D)
			, (void**)renderTarget.GetAddressOf())))
			return;
		mRenderTarget->SetTexture(renderTarget);

		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView = nullptr;
		mDevice->CreateRenderTargetView((ID3D11Resource*)mRenderTarget->GetTexture().Get()
			, nullptr, renderTargetView.GetAddressOf());
		mRenderTarget->SetRTV(renderTargetView);

		// DepthStencil 만들기
		D3D11_TEXTURE2D_DESC depthStencilDesc = {};
		depthStencilDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
		depthStencilDesc.CPUAccessFlags = 0;

		depthStencilDesc.Format = DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;
		
		depthStencilDesc.Width = clientWidth;
		depthStencilDesc.Height = clientHeight;
		depthStencilDesc.ArraySize = 1;
		
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.MipLevels = 0;
		depthStencilDesc.MiscFlags = 0;

		Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer = nullptr;

		if (!CreateTexture2D(&depthStencilDesc, nullptr, depthStencilBuffer.GetAddressOf()))
			return;
		mDepthStencil->SetTexture(depthStencilBuffer);

		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView = nullptr;
		if (!CreateDepthStencilView(depthStencilBuffer.Get(), nullptr, depthStencilView.GetAddressOf()))
			return;
		mDepthStencil->SetDSV(depthStencilView);

		RECT winRect = {};
		GetClientRect(hWnd, &winRect);
		int a = 0;
		mViewPort =
		{
			0.0f, 0.0f
			, (float)clientWidth
			, (float)clientHeight
			, 0.0f, 1.0f
		};
		BindViewPort(&mViewPort);
		mContext->OMSetRenderTargets(1, mRenderTarget->GetRTV().GetAddressOf(), mDepthStencil->GetDSV().Get());
	}
	GraphicDevice_Dx11::~GraphicDevice_Dx11()
	{

	}
	
	bool GraphicDevice_Dx11::CreateSwapChain(const DXGI_SWAP_CHAIN_DESC* desc, HWND hWnd)
	{
		DXGI_SWAP_CHAIN_DESC dxgiDesc = {};

		dxgiDesc.OutputWindow = hWnd;
		dxgiDesc.Windowed = true;
		dxgiDesc.BufferCount = desc->BufferCount;
		dxgiDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_DISCARD;

		dxgiDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		dxgiDesc.BufferDesc.Width = desc->BufferDesc.Width;
		dxgiDesc.BufferDesc.Height = desc->BufferDesc.Height;
		dxgiDesc.BufferDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
		dxgiDesc.BufferDesc.RefreshRate.Numerator = 240;
		dxgiDesc.BufferDesc.RefreshRate.Denominator = 1;
		dxgiDesc.BufferDesc.Scaling = DXGI_MODE_SCALING::DXGI_MODE_SCALING_UNSPECIFIED;
		dxgiDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

		dxgiDesc.SampleDesc.Count = 1;
		dxgiDesc.SampleDesc.Quality = 0;

		Microsoft::WRL::ComPtr<IDXGIDevice> pDXGIDevice = nullptr;
		Microsoft::WRL::ComPtr<IDXGIAdapter> pAdapter = nullptr;
		Microsoft::WRL::ComPtr<IDXGIFactory> pFactory = nullptr;

		if (FAILED(mDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)pDXGIDevice.GetAddressOf())))
			return false;

		if (FAILED(pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)pAdapter.GetAddressOf())))
			return false;

		if (FAILED(pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)pFactory.GetAddressOf())))
			return false;

		if (FAILED(pFactory->CreateSwapChain(mDevice.Get(), &dxgiDesc, mSwapChain.GetAddressOf())))
			return false;

		return true;
	}
	bool GraphicDevice_Dx11::CreateTexture2D(const D3D11_TEXTURE2D_DESC* desc, void* data, ID3D11Texture2D** ppTexture2D)
	{
		if (FAILED(mDevice->CreateTexture2D(desc, nullptr, ppTexture2D)))
			return false;
		return true;
	}

	bool GraphicDevice_Dx11::CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs, UINT numElements, ID3DBlob* byteCode, ID3D11InputLayout** ppInputLayout)
	{
		if (FAILED(mDevice->CreateInputLayout(pInputElementDescs, numElements, byteCode->GetBufferPointer(), byteCode->GetBufferSize(), ppInputLayout)))
			return false;

		return true; 
	}
	bool GraphicDevice_Dx11::CreateBuffer(ID3D11Buffer** buffer, D3D11_BUFFER_DESC* desc, D3D11_SUBRESOURCE_DATA* data)
	{
		if (FAILED(mDevice->CreateBuffer(desc, data, buffer)))
			return false;

		return true;
	}

	bool GraphicDevice_Dx11::CompileFromFile(const std::wstring& fileName, const std::string& funcName, const std::string& version, ID3DBlob** ppCode)
	{
		ID3DBlob* errorBlob = nullptr;
		D3DCompileFromFile(fileName.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
			, funcName.c_str(), version.c_str(), 0, 0, ppCode, &errorBlob);
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
			errorBlob = nullptr;
		}

		return false;
	}
	bool GraphicDevice_Dx11::CreateVertexShader(const void* pShaderBytecode, SIZE_T bytecodeLength, ID3D11VertexShader** ppVertexShader)
	{
		if (FAILED(mDevice->CreateVertexShader(pShaderBytecode, bytecodeLength, nullptr, ppVertexShader)))
			return false;

		return true;		
	}
	bool GraphicDevice_Dx11::CreatePixelShader(const void* pShaderBytecode, SIZE_T bytecodeLength, ID3D11PixelShader** ppPixelShader)
	{
		if (FAILED(mDevice->CreatePixelShader(pShaderBytecode, bytecodeLength, nullptr, ppPixelShader)))
			return false;

		return true;
	}

	bool GraphicDevice_Dx11::CreateComputeShader(const void* pShaderBytecode, SIZE_T bytecodeLength, ID3D11ComputeShader** ppComputeShader)
	{
		if (FAILED(mDevice->CreateComputeShader(pShaderBytecode, bytecodeLength, nullptr, ppComputeShader)))
			return false;

		return true;
	}
	
	bool GraphicDevice_Dx11::CreateSamplerState(const D3D11_SAMPLER_DESC* pSamplerDesc, ID3D11SamplerState** ppSamplerState)
	{
		if (FAILED(mDevice->CreateSamplerState(pSamplerDesc, ppSamplerState)))
			return false;

		return true;
	}
	bool GraphicDevice_Dx11::CreateRasterizerState(const D3D11_RASTERIZER_DESC* pRasterizerDesc, ID3D11RasterizerState** ppRasterizerState)
	{
		if (FAILED(mDevice->CreateRasterizerState(pRasterizerDesc, ppRasterizerState)))
			return false;

		return true;
	}
	bool GraphicDevice_Dx11::CreateDepthStencilState(const D3D11_DEPTH_STENCIL_DESC* pDepthStencilDesc, ID3D11DepthStencilState** ppDepthStencilState)
	{
		if (FAILED(mDevice->CreateDepthStencilState(pDepthStencilDesc, ppDepthStencilState)))
			return false;

		return true;
	}
	bool GraphicDevice_Dx11::CreateBlendState(const D3D11_BLEND_DESC* pBlendDesc, ID3D11BlendState** ppBlendState)
	{
		if (FAILED(mDevice->CreateBlendState(pBlendDesc, ppBlendState)))
			return false;

		return true;
	}

	bool GraphicDevice_Dx11::CreateShaderResourceView(ID3D11Resource* pResource, const D3D11_SHADER_RESOURCE_VIEW_DESC* pDesc, ID3D11ShaderResourceView** ppSRView)
	{
		if (FAILED(mDevice->CreateShaderResourceView(pResource, pDesc, ppSRView)))
			return false;

		return true;
	}

	bool GraphicDevice_Dx11::CreateRenderTargetView(ID3D11Resource* pResource, const D3D11_RENDER_TARGET_VIEW_DESC* pDesc, ID3D11RenderTargetView** ppRTView)
	{
		if (FAILED(mDevice->CreateRenderTargetView(pResource, pDesc, ppRTView)))
			return false;

		return true;
	}

	bool GraphicDevice_Dx11::CreateDepthStencilView(ID3D11Resource* pResource, const D3D11_DEPTH_STENCIL_VIEW_DESC* pDesc, ID3D11DepthStencilView** ppDSView)
	{
		if (FAILED(mDevice->CreateDepthStencilView(pResource, pDesc, ppDSView)))
			return false;

		return true;
	}
	
	bool GraphicDevice_Dx11::CreateUnorderedAccessView(ID3D11Resource* pResource, const D3D11_UNORDERED_ACCESS_VIEW_DESC* pDesc, ID3D11UnorderedAccessView** ppUAView)
	{
		if (FAILED(mDevice->CreateUnorderedAccessView(pResource, pDesc, ppUAView)))
			return false;

		return true;
	}
	
	void GraphicDevice_Dx11::BindInputLayout(ID3D11InputLayout* pInputLayout)
	{
		mContext->IASetInputLayout(pInputLayout);
	}
	void GraphicDevice_Dx11::BindPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY topology)
	{
		mContext->IASetPrimitiveTopology(topology);
	}


	void GraphicDevice_Dx11::BindViewPort(D3D11_VIEWPORT* viewPort)
	{
		mContext->RSSetViewports(1, viewPort);
	}

	void GraphicDevice_Dx11::BindVertexBuffer(UINT startSlot, ID3D11Buffer* const* ppVertexBuffers, const UINT* pStrides, const UINT* pOffsets)
	{
		mContext->IASetVertexBuffers(startSlot, 1, ppVertexBuffers, pStrides, pOffsets);
	}
	void GraphicDevice_Dx11::BindIndexBuffer(ID3D11Buffer* pIndexBuffer, DXGI_FORMAT format, UINT offset)
	{
		mContext->IASetIndexBuffer(pIndexBuffer, format, offset);
	}

	void GraphicDevice_Dx11::BindVertexShader(ID3D11VertexShader* pVetexShader)
	{
		mContext->VSSetShader(pVetexShader, 0, 0);
	}
	void GraphicDevice_Dx11::BindPixelShader(ID3D11PixelShader* pPixelShader)
	{
		mContext->PSSetShader(pPixelShader, 0, 0);
	}

	void GraphicDevice_Dx11::BindComputeShader(ID3D11ComputeShader* pComputeShader)
	{
		mContext->CSSetShader(pComputeShader, 0, 0);
	}

	void GraphicDevice_Dx11::Dispatch(UINT threadGroupCountX, UINT threadGroupCountY, UINT threadGroupCountZ)
	{
		mContext->Dispatch(threadGroupCountX, threadGroupCountY, threadGroupCountZ);
	}

	void GraphicDevice_Dx11::SetConstantBuffer(ID3D11Buffer* buffer, void* data, UINT size)
	{
		D3D11_MAPPED_SUBRESOURCE subRes = {};
		mContext->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subRes);
		memcpy(subRes.pData, data, size);
		mContext->Unmap(buffer, 0);
	}

	void GraphicDevice_Dx11::BindConstantBuffer(eShaderStage stage, eCBType type, ID3D11Buffer* buffer)
	{
		switch (stage)
		{
		case eShaderStage::VS:
			mContext->VSSetConstantBuffers((UINT)type, 1, &buffer);
			break;
		case eShaderStage::HS:
			mContext->HSSetConstantBuffers((UINT)type, 1, &buffer);
			break;
		case eShaderStage::DS:
			mContext->DSSetConstantBuffers((UINT)type, 1, &buffer);
			break;
		case eShaderStage::GS:
			mContext->GSSetConstantBuffers((UINT)type, 1, &buffer);
			break;
		case eShaderStage::PS:
			mContext->PSSetConstantBuffers((UINT)type, 1, &buffer);
			break;
		case eShaderStage::CS:
			mContext->CSSetConstantBuffers((UINT)type, 1, &buffer);
			break;
		case eShaderStage::End:
			break;
		default:
			break;
		}
	}
	void GraphicDevice_Dx11::BindsConstantBuffer(eCBType type, ID3D11Buffer* buffer)
	{
		mContext->VSSetConstantBuffers((UINT)type, 1, &buffer);
		mContext->HSSetConstantBuffers((UINT)type, 1, &buffer);
		mContext->DSSetConstantBuffers((UINT)type, 1, &buffer);
		mContext->GSSetConstantBuffers((UINT)type, 1, &buffer);
		mContext->PSSetConstantBuffers((UINT)type, 1, &buffer);
		mContext->CSSetConstantBuffers((UINT)type, 1, &buffer);
	}

	void GraphicDevice_Dx11::BindBuffer(ID3D11Buffer* buffer, void* data, UINT size)
	{
		D3D11_MAPPED_SUBRESOURCE sub = {};
		mContext->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &sub);
		memcpy(sub.pData, data, size);
		mContext->Unmap(buffer, 0);
	}

	void GraphicDevice_Dx11::BindUnorderedAccessViews(UINT slot, ID3D11UnorderedAccessView** ppUnorderedAccessViews, const UINT* pUAVInitialCounts)
	{
		mContext->CSSetUnorderedAccessViews(slot, 1, ppUnorderedAccessViews, pUAVInitialCounts);
	}

	void GraphicDevice_Dx11::BindShaderResource(eShaderStage stage, UINT startSlot, ID3D11ShaderResourceView** ppSRV)
	{
		switch (stage)
		{
		case eShaderStage::VS:
			mContext->VSSetShaderResources(startSlot, 1, ppSRV);
			break;
		case eShaderStage::HS:
			mContext->HSSetShaderResources(startSlot, 1, ppSRV);
			break;
		case eShaderStage::DS:
			mContext->DSSetShaderResources(startSlot, 1, ppSRV);
			break;
		case eShaderStage::GS:
			mContext->GSSetShaderResources(startSlot, 1, ppSRV);
			break;
		case eShaderStage::PS:
			mContext->PSSetShaderResources(startSlot, 1, ppSRV);
			break;
		case eShaderStage::CS:
			mContext->CSSetShaderResources(startSlot, 1, ppSRV);
			break;
		case eShaderStage::End:
			break;
		default:
			break;
		}
	}

	void GraphicDevice_Dx11::BindSamplers(eShaderStage stage, UINT startSlot, ID3D11SamplerState** ppSamplers)
	{
		switch (stage)
		{
		case eShaderStage::VS:
			mContext->VSSetSamplers(startSlot, 1, ppSamplers);
			break;
		case eShaderStage::HS:
			mContext->HSSetSamplers(startSlot, 1, ppSamplers);
			break;
		case eShaderStage::DS:
			mContext->DSSetSamplers(startSlot, 1, ppSamplers);
			break;
		case eShaderStage::GS:
			mContext->GSSetSamplers(startSlot, 1, ppSamplers);
			break;
		case eShaderStage::PS:
			mContext->PSSetSamplers(startSlot, 1, ppSamplers);
			break;
		case eShaderStage::CS:
			mContext->CSSetSamplers(startSlot, 1, ppSamplers);
			break;
		case eShaderStage::End:
			break;
		default:
			break;
		}
	}
	void GraphicDevice_Dx11::BindRasterizerState(ID3D11RasterizerState* pRasterizerState)
	{
		mContext->RSSetState(pRasterizerState);
	}
	void GraphicDevice_Dx11::BindDepthStencilState(ID3D11DepthStencilState* pDepthStencilState)
	{
		mContext->OMSetDepthStencilState(pDepthStencilState, 0);
	}
	void GraphicDevice_Dx11::BindBlendState(ID3D11BlendState* pBlendState)
	{
		mContext->OMSetBlendState(pBlendState, nullptr, 0xffffffff);
	}

	void GraphicDevice_Dx11::DrawIndexed(UINT indexCount, UINT startIndexLocation, INT baseVertexLocation)
	{
		mContext->DrawIndexed(indexCount, startIndexLocation, baseVertexLocation);
	}
	void GraphicDevice_Dx11::DrawIndexedInstanced(UINT indexCountPerInstance, UINT instanceCount, UINT startIndexLocation, INT baseVertexLocation, UINT startInstanceLocation)
	{
		mContext->DrawIndexedInstanced(indexCountPerInstance, instanceCount, startIndexLocation, baseVertexLocation, startInstanceLocation);
	}
	void GraphicDevice_Dx11::ClearTarget()
	{
		FLOAT bgColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
		mContext->ClearRenderTargetView(mRenderTarget->GetRTV().Get(), bgColor);
		mContext->ClearDepthStencilView(mDepthStencil->GetDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, (UINT8)0.0f);
		mContext->OMSetRenderTargets(1, mRenderTarget->GetRTV().GetAddressOf(), mDepthStencil->GetDSV().Get());
	}
	void GraphicDevice_Dx11::UpdateViewPort()
	{
		HWND hwnd = application.GetHwnd();
		RECT  winRect = {};
		GetClientRect(hwnd, &winRect);
		mViewPort =
		{
			0.0f, 0.0f
			, (float)(winRect.right - winRect.left)
			, (float)(winRect.bottom - winRect.top)
			, 0.0f, 1.0f
		};
		BindViewPort(&mViewPort);
	}
	void GraphicDevice_Dx11::Draw()
	{
	}
	void GraphicDevice_Dx11::Present()
	{
		mSwapChain->Present(0, 0);
	}
}
