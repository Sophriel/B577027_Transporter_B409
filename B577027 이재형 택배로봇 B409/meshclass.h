#pragma once
#include <d3d11.h>
#include <d3dx10math.h>
#include <vector>
using namespace std;

#include "textureclass.h"

struct VertexType
{
	D3DXVECTOR3 position;
	D3DXVECTOR2 texture;
	D3DXVECTOR3 normal;
};

struct VertexBoneData
{
	UINT IDs[1];
	float Weights[1];
};

class Mesh
{
private:

public:
	Mesh(ID3D11Device *dev, ID3D11DeviceContext * deviceContext,
		vector<VertexType> & vertices, vector<UINT> & indices, TextureClass* texture);
	Mesh(const Mesh & mesh);
	bool Initialize(ID3D11Device *dev);


	void Draw();
	void Release();

public:
	ID3D11Buffer *vertexbuffer, *indexbuffer;
	vector<VertexType> vertices;
	vector<UINT> indices;
	TextureClass* m_Texture;

	ID3D11Device *dev;
	ID3D11DeviceContext * deviceContext;
};