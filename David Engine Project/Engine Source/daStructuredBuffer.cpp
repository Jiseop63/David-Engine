#include "daStructuredBuffer.h"
#include "daGraphicDevice Dx11.h"

namespace da::graphics
{
    StructuredBuffer::StructuredBuffer()
        : mViewType(eViewType::SRV)
        , mReadBuffer(nullptr)
        , mWriteBuffer(nullptr)
        , mSRV(nullptr)
        , mUAV(nullptr)
        , mSize(0)
        , mStride(0)
        , mSRVSlot(0)
        , mUAVSlot(0)
    {
    }
    StructuredBuffer::~StructuredBuffer()
    {
    }
    bool StructuredBuffer::Create(UINT size, UINT stride, eViewType type, void* data, bool cpuAccess)
    {
        mSize = size;
        mStride = stride;
        mViewType = type;

        Desc.ByteWidth = mSize * mStride;
        Desc.StructureByteStride = mSize;

        Desc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
        Desc.CPUAccessFlags = 0;
        Desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
        Desc.MiscFlags = D3D11_RESOURCE_MISC_FLAG::D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

        if (eViewType::UAV == mViewType)
        {
            Desc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
            Desc.CPUAccessFlags = 0;
            Desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_FLAG::D3D11_BIND_UNORDERED_ACCESS;	// Texture Register Binding
        }

        if (data)
        {
            D3D11_SUBRESOURCE_DATA tSub = {};
            tSub.pSysMem = data;

            if (!(GetDevice()->CreateBuffer(Buffer.GetAddressOf(), &Desc, &tSub)))
                return false;
        }
        else
        {
            if (!GetDevice()->CreateBuffer(Buffer.GetAddressOf(), &Desc, nullptr))
                return false;
        }
        
        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.BufferEx.NumElements = mStride;
        srvDesc.ViewDimension = D3D_SRV_DIMENSION::D3D_SRV_DIMENSION_BUFFEREX;

        if (!GetDevice()->CreateShaderResourceView(Buffer.Get(), &srvDesc, mSRV.GetAddressOf()))
            return false;

        if (eViewType::UAV == mViewType)
        {
            D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
            uavDesc.Buffer.NumElements = mStride;
            uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;

            if (!GetDevice()->CreateUnorderedAccessView(Buffer.Get(), &uavDesc, mUAV.GetAddressOf()))
                return false;
        }

        if (cpuAccess)
            CreateRWBuffer();

        return true;
    }
    bool StructuredBuffer::CreateRWBuffer()
    {
        D3D11_BUFFER_DESC wDesc(Desc);

        wDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED; // 구조화 버퍼 추가 플래그 설정
        wDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;	// Texture Register Binding	

        wDesc.Usage = D3D11_USAGE_DYNAMIC;
        wDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

        if (!GetDevice()->CreateBuffer(mWriteBuffer.GetAddressOf(), &wDesc, nullptr))
            return false;

        D3D11_BUFFER_DESC rDesc(Desc);

        rDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED; // 구조화 버퍼 추가 플래그 설정
        rDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;	// Texture Register Binding

        rDesc.Usage = D3D11_USAGE_DEFAULT;
        rDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

        if (!GetDevice()->CreateBuffer(mReadBuffer.GetAddressOf(), &rDesc, nullptr))
            return false;

        return true;
    }
    void StructuredBuffer::SetData(void* data, UINT stride)
    {
        if (mStride < stride)
            Create(mSize, stride, mViewType, data);
        else
            GetDevice()->BindBuffer(mWriteBuffer.Get(), data, mSize * stride);

        GetDevice()->CopyResource(Buffer.Get(), mWriteBuffer.Get());
    }
    void StructuredBuffer::GetData(void* data, UINT size)
    {
        GetDevice()->CopyResource(mReadBuffer.Get(), Buffer.Get());

        if (size == 0)
            GetDevice()->BindBuffer(mReadBuffer.Get(), data, mSize * mStride);
        else
            GetDevice()->BindBuffer(mReadBuffer.Get(), data, size);
    }
    void StructuredBuffer::BindSRV(eShaderStage stage, UINT slot)
    {
        mSRVSlot = slot;
        GetDevice()->BindShaderResource(stage, slot, mSRV.GetAddressOf());
    }
    void StructuredBuffer::BindUAV(UINT slot)
    {
        mUAVSlot = slot;
        UINT offset = -1;
        GetDevice()->BindUnorderedAccessViews(slot, mUAV.GetAddressOf(), &offset);
    }
    void StructuredBuffer::Clear()
    {
        // srv clear
        ID3D11ShaderResourceView* srv = nullptr;
        GetDevice()->BindShaderResource(eShaderStage::VS, mSRVSlot, &srv);
        GetDevice()->BindShaderResource(eShaderStage::HS, mSRVSlot, &srv);
        GetDevice()->BindShaderResource(eShaderStage::DS, mSRVSlot, &srv);
        GetDevice()->BindShaderResource(eShaderStage::GS, mSRVSlot, &srv);
        GetDevice()->BindShaderResource(eShaderStage::PS, mSRVSlot, &srv);
        GetDevice()->BindShaderResource(eShaderStage::CS, mSRVSlot, &srv);

        // uav clear
        ID3D11UnorderedAccessView* uav = nullptr;
        UINT offset = -1;
        GetDevice()->BindUnorderedAccessViews(mUAVSlot, &uav, &offset);
    }
}
