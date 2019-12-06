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
#include <list>
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
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool InitTextures(ID3D11Device* device, const WCHAR* textureFilename);
	bool Initialize(ID3D11DeviceContext*, const char*);
	void Shutdown();
	void Render(ID3D11DeviceContext*, int num);

	//int GetVertexCount();
	int GetIndexCount(int i);
	ID3D11ShaderResourceView* GetTexture(int num);

	//bool LoadModel(const char*);
	void ReleaseModel();

	vector<Mesh*> meshes;
	//vector<Texture> textures_loaded;



private:
	//bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);
	void RenderMeshes(int num);

	bool LoadTexture(ID3D11Device*, const WCHAR*);
	void ReleaseTexture();


	const aiScene* pScene;

	bool LoadModel(const string & filePath);
	void ProcessNode(aiNode * node, const aiScene * scene);
	Mesh* ProcessMesh(aiMesh * mesh, const aiScene * scene);

	//void LoadAnimation(const string path, SkinModel* model, UINT flag);
	//void ProcessAnimation(const aiScene * pScene, SkinModel* skModel);

	//string directory;
	//string textype;
	//vector<Texture> loadMaterialTextures(aiMaterial * mat, aiTextureType type, string typeName, const aiScene * scene);
	//string determineTextureType(const aiScene * scene, aiMaterial * mat);
	//int getTextureIndex(aiString * str);
	//ID3D11ShaderResourceView * getTextureFromModel(const aiScene * scene, int textureindex);


private:
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;

	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	vector<unsigned int> m_indexCount;
	list<TextureClass*> m_Texture;
};

#endif