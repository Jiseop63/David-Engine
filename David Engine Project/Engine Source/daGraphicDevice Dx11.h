#pragma once
#include "David Engine.h"
#include "daGraphics.h"
#include "daTexture.h"

namespace da::graphics
{
	class GraphicDevice_Dx11
	{
	public:
		GraphicDevice_Dx11();
		~GraphicDevice_Dx11();

	public:
		bool CreateSwapChain(const DXGI_SWAP_CHAIN_DESC* desc, HWND hwnd);
		bool CreateTexture2D(const D3D11_TEXTURE2D_DESC* desc, void* data, ID3D11Texture2D** ppTexture2D);
		bool CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs, UINT numElements, ID3DBlob* byteCode, ID3D11InputLayout** ppInputLayout);
		bool CreateBuffer(ID3D11Buffer** buffer, D3D11_BUFFER_DESC* desc, D3D11_SUBRESOURCE_DATA* data);
		
		bool CompileFromFile(const std::wstring& fileName, const std::string& funcName, const std::string& version, ID3DBlob** ppCode);
		bool CreateVertexShader(const void* pShaderBytecode, SIZE_T bytecodeLength, ID3D11VertexShader** ppVertexShader);
		bool CreateGeometryShader(const void* pShaderBytecode, SIZE_T bytecodeLength, ID3D11GeometryShader** ppGeometryShader);
		bool CreatePixelShader(const void* pShaderBytecode, SIZE_T bytecodeLength, ID3D11PixelShader** ppPixelShader);
		bool CreateComputeShader(const void* pShaderBytecode, SIZE_T bytecodeLength, ID3D11ComputeShader** ppComputeShader);

		bool CreateSamplerState(const D3D11_SAMPLER_DESC* pSamplerDesc, ID3D11SamplerState** ppSamplerState);
		bool CreateRasterizerState(const D3D11_RASTERIZER_DESC* pRasterizerDesc, ID3D11RasterizerState** ppRasterizerState);
		bool CreateDepthStencilState(const D3D11_DEPTH_STENCIL_DESC* pDepthStencilDesc, ID3D11DepthStencilState** ppDepthStencilState);
		bool CreateBlendState(const D3D11_BLEND_DESC* pBlendDesc, ID3D11BlendState** ppBlendState);

		bool CreateShaderResourceView(ID3D11Resource* pResource, const D3D11_SHADER_RESOURCE_VIEW_DESC* pDesc, ID3D11ShaderResourceView** ppSRView);
		bool CreateRenderTargetView(ID3D11Resource* pResource, const D3D11_RENDER_TARGET_VIEW_DESC* pDesc, ID3D11RenderTargetView** ppDSView);
		bool CreateDepthStencilView(ID3D11Resource* pResource, const D3D11_DEPTH_STENCIL_VIEW_DESC* pDesc, ID3D11DepthStencilView** ppDSView);
		bool CreateUnorderedAccessView(ID3D11Resource* pResource, const D3D11_UNORDERED_ACCESS_VIEW_DESC* pDesc, ID3D11UnorderedAccessView** ppDSView);

		void BindInputLayout(ID3D11InputLayout* pInputLayout);
		void BindPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY topology);

		void BindVertexBuffer(UINT startSlot, ID3D11Buffer* const* ppVertexBuffers, const UINT* pStrides, const UINT* pOffsets);
		void BindIndexBuffer(ID3D11Buffer* pIndexBuffer, DXGI_FORMAT format, UINT offset);

		void BindVertexShader(ID3D11VertexShader* pVetexShader);
		void BindHullShader(ID3D11HullShader* pHullShader);
		void BindDomainShader(ID3D11DomainShader* pDomainShader);
		void BindGeometryShader(ID3D11GeometryShader* pGeometryShader);
		void BindPixelShader(ID3D11PixelShader* pPixelShader);
		void BindComputeShader(ID3D11ComputeShader* pComputeShader);
		void Dispatch(UINT threadGroupCountX, UINT threadGroupCountY, UINT threadGroupCountZ);

		void BindViewPort(D3D11_VIEWPORT* viewPort);

		void SetConstantBuffer(ID3D11Buffer* buffer, void* data, UINT size);
		void BindConstantBuffer(eShaderStage stage, eCBType type, ID3D11Buffer* buffer);
		void BindsConstantBuffer(eCBType type, ID3D11Buffer* buffer);
		void BindBuffer(ID3D11Buffer* buffer, void* data, UINT size);
		void BindUnorderedAccessViews(UINT slot, ID3D11UnorderedAccessView** ppUnorderedAccessViews, const UINT* pUAVInitialCounts);

		void BindSamplers(eShaderStage stage, UINT startSlot, ID3D11SamplerState** ppSamplers);
		void BindShaderResource(eShaderStage stage, UINT startSlot, ID3D11ShaderResourceView** ppSRV);
		void BindRasterizerState(ID3D11RasterizerState* pRasterizerState);
		void BindDepthStencilState(ID3D11DepthStencilState* pDepthStencilState);
		void BindBlendState(ID3D11BlendState* pBlendState);

	public:
		void DrawIndexed(UINT indexCount, UINT startIndexLocation, INT baseVertexLocation);
		void DrawIndexedInstanced(UINT indexCountPerInstance, UINT instanceCount, UINT startIndexLocation, INT baseVertexLocation, UINT startInstanceLocation);
		void ClearTarget();
		void UpdateViewPort();
		void Draw();
		void Present();


	public:
		ID3D11Device* GetID3D11Device() { return mDevice.Get(); }
		D3D11_VIEWPORT GetViewPort() { return mViewPort; }

	private:
		
		Microsoft::WRL::ComPtr<ID3D11Device> mDevice; // 실제 그래픽카드 하드웨어 객체		
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mContext; // 이객체를 이용하여 명령을 내린다.		
		std::shared_ptr<da::graphics::Texture> mRenderTarget;
		std::shared_ptr<da::graphics::Texture> mDepthStencil;
		Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain; // 더블버퍼링 작업을 진행해주는 swapChain
		D3D11_VIEWPORT mViewPort;
	};

	inline GraphicDevice_Dx11*& GetDevice()
	{
		static GraphicDevice_Dx11* device = nullptr;
		return device;
	}
}
