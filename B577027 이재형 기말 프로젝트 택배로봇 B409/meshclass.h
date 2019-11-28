#pragma once
#include "modelclass.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

class Mesh
{
private:
	struct Texture {
		string type;
		string path;
		ID3D11ShaderResourceView *texture;
	};

public:
	Mesh(ID3D11Device *dev, ID3D11Device * device, ID3D11DeviceContext * deviceContext,
		std::vector<ModelClass::ModelType> & vertices, std::vector<UINT> & indices);
	Mesh(const Mesh & mesh);
	bool Initialize(ID3D11Device *dev);
	void Release();

private:
	ID3D11Buffer *vertexbuffer, *indexbuffer;
	vector<ModelClass::ModelType> vertices;
	vector<UINT> indices;
	vector<Texture> textures;

	ID3D11Device *dev;
	ID3D11DeviceContext * deviceContext;
};