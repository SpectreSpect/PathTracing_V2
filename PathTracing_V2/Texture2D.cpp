#include "Texture2D.h"

Texture2D::Texture2D() : Buffer::Buffer()
{
	bufferType = Buffer::BufferType::TEXTURE2D;
	pBuf = nullptr;
	MipLevels = 0;
	Format = DXGI_FORMAT_R8G8B8A8_UNORM;
}

HRESULT Texture2D::Init(ID3D11Device* device, void* pData, UINT Width, UINT Height, DXGI_FORMAT Format, UINT BindFlags, UINT CPUAccessFlags, D3D11_USAGE Usage, 
						UINT MiscFlags, UINT MipLevels, UINT ArraySize, UINT SampleDesc_Count)
{
	D3D11_TEXTURE2D_DESC textureDesc{};
	textureDesc.Width = Width;
	textureDesc.Height = Height;
	textureDesc.Format = this->Format = Format;
	textureDesc.BindFlags = BindFlags;
	textureDesc.Usage = Usage;
	textureDesc.CPUAccessFlags = CPUAccessFlags;
	textureDesc.MiscFlags = MiscFlags;
	textureDesc.MipLevels = this->MipLevels = MipLevels;
	textureDesc.ArraySize = ArraySize;
	textureDesc.SampleDesc.Count = SampleDesc_Count;
	if (pData)
	{
		D3D11_SUBRESOURCE_DATA sb;
		sb.pSysMem = pData;
		return device->CreateTexture2D(&textureDesc, &sb, &pBuf);
	}
	else 
	{
		return device->CreateTexture2D(&textureDesc, NULL, &pBuf);
	}
}
