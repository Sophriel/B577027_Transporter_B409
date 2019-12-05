#pragma once
#include <d3d11.h>
#include <vector>
using namespace std;


struct ModelType
{
	float x, y, z;
	float tu, tv;
	float nx, ny, nz;
};

class Mesh
{
private:
	struct Texture
	{
		string type;
		string path;
		ID3D11ShaderResourceView *texture;
	};

public:
	Mesh(ID3D11Device *dev, ID3D11DeviceContext * deviceContext,
		vector<ModelType> & vertices, vector<UINT> & indices);
	Mesh(const Mesh & mesh);
	bool Initialize(ID3D11Device *dev);
	void Release();

public:
	ID3D11Buffer *vertexbuffer, *indexbuffer;
	vector<ModelType> vertices;
	vector<UINT> indices;
	vector<Texture> textures;

	ID3D11Device *dev;
	ID3D11DeviceContext * deviceContext;
};