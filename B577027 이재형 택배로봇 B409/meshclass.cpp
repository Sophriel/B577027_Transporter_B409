#include "meshclass.h"

Mesh::Mesh(ID3D11Device *dev, ID3D11DeviceContext * deviceContext,
	vector<VertexType>& vertices, vector<UINT>& indices, TextureClass* texture)
{
	this->vertices = vertices;
	this->indices = indices;
	this->m_Texture = texture;

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
	vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.ByteWidth = sizeof(VertexType) * vertices.size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = &vertices[0];

	hr = dev->CreateBuffer(&vbd, &initData, &vertexbuffer);
	if (FAILED(hr))
		return false;

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.ByteWidth = sizeof(UINT) * indices.size();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	initData.pSysMem = &indices[0];

	hr = dev->CreateBuffer(&ibd, &initData, &indexbuffer);
	if (FAILED(hr))
		return false;

	return true;
}


void Mesh::Draw()
{
	unsigned int stride;

	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);

	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &vertexbuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(indexbuffer, DXGI_FORMAT_R32_UINT, 0);
	//deviceContext->DrawIndexed(indices.size(), 0, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Mesh::Release()
{
	vertexbuffer->Release();
	indexbuffer->Release();
}

