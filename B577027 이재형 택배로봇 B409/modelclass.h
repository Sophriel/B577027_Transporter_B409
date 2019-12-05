////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>
#include <fstream>
#include <vector>
using namespace std;

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "textureclass.h"
#include "meshclass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: ModelClass
////////////////////////////////////////////////////////////////////////////////
class ModelClass
{
public:

	struct VertexType
	{
		D3DXVECTOR3 position;
	    D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
	};

public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, const char*, const WCHAR*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetVertexCount();
	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

	//bool LoadModel(const char*);
	void ReleaseModel();

	vector<Mesh*> meshes;
	bool LoadModel(const string & filePath);
	void ProcessNode(aiNode * node, const aiScene * scene);
	Mesh* ProcessMesh(aiMesh * mesh, const aiScene * scene);

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, const WCHAR*);
	void ReleaseTexture();

private:
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;

	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	unsigned int m_vertexCount, m_indexCount;
	TextureClass* m_Texture;
	//ModelType* m_model;
};

#endif