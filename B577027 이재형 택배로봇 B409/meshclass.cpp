#include "meshclass.h"

Mesh::Mesh(ID3D11Device *dev, ID3D11DeviceContext * deviceContext,
	vector<ModelType>& vertices, vector<UINT>& indices)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	this->dev = dev;
	this->deviceContext = deviceContext;

	this->Initialize(dev);
}

Mesh::Mesh(const Mesh & mesh)
{
	this->deviceContext = mesh.deviceContext;
	this->indexbuffer = mesh.indexbuffer;
	this->vertexbuffer = mesh.vertexbuffer;
}

bool Mesh::Initialize(ID3D11Device *dev)
{
	HRESULT hr;

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(ModelType) * vertices.size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = &vertices[0];

	hr = dev->CreateBuffer(&vbd, &initData, &vertexbuffer);
	if (FAILED(hr))
		return false;

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * indices.size();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;

	initData.pSysMem = &indices[0];

	hr = dev->CreateBuffer(&ibd, &initData, &indexbuffer);
	if (FAILED(hr))
		return false;

	return true;
}

void Mesh::Release()
{
	//	UINT offset = 0;
	//	this->deviceContext->IASetVertexBuffers(0, 1, this->vertexbuffer.GetAddressOf(), this->vertexbuffer.StridePtr(), &offset);
	//	this->deviceContext->IASetIndexBuffer(this->indexbuffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	//	this->deviceContext->DrawIndexed(this->indexbuffer.BufferSize(), 0, 0);
	
	vertexbuffer->Release();
	indexbuffer->Release();
}