////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "modelclass.h"


ModelClass::ModelClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	//m_Texture = 0;
	//m_model = 0;
}
ModelClass::ModelClass(const ModelClass& other)
{
}
ModelClass::~ModelClass()
{
}

bool ModelClass::InitTextures(ID3D11Device* device, const WCHAR* textureFilename)
{
	bool result;

	this->device = device;
	result = LoadTexture(device, textureFilename);
	if (!result)
	{
		return false;
	}
}

bool ModelClass::Initialize(ID3D11DeviceContext* dvC, const char* modelFilename)
{
	bool result;

	this->deviceContext = dvC;

	// Load in the model data,
	result = LoadModel(modelFilename);
	if (!result)
	{
		return false;
	}

	// Initialize the vertex and index buffers.
	//result = InitializeBuffers(device);
	//if(!result)
	//{
	//	return false;
	//}

	// Load the texture for this model.

	return true;
}


void ModelClass::Shutdown()
{
	// Release the model texture.
	ReleaseTexture();

	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	// Release the model data.
	ReleaseModel();

	return;
}


void ModelClass::Render(ID3D11DeviceContext* deviceContext, int num)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.

	//RenderBuffers(deviceContext);

	RenderMeshes(num);

	return;
}

//int ModelClass::GetVertexCount()
//{
//	return m_vertexCount;
//}
int ModelClass::GetIndexCount(int i)
{
	return m_indexCount.at(i);
}
ID3D11ShaderResourceView* ModelClass::GetTexture(int num)
{
	return meshes.at(num)->m_Texture->GetTexture();
}


//bool ModelClass::InitializeBuffers(ID3D11Device* device)
//{
//	VertexType* vertices;
//	unsigned long* indices;
//	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
//    D3D11_SUBRESOURCE_DATA vertexData, indexData;
//	HRESULT result;
//	int i;
//
//	// Create the vertex array.
//	vertices = new VertexType[m_vertexCount];
//	if(!vertices)
//	{
//		return false;
//	}
//
//	// Create the index array.
//	indices = new unsigned long[m_indexCount];
//	if(!indices)
//	{
//		return false;
//	}
//
//
//	// Load the vertex array and index array with data.
//	for(i=0; i < m_vertexCount; i++)
//	{
//		vertices[i].position = D3DXVECTOR3(meshes.at(0)->vertices.at(i).x, meshes.at(0)->vertices.at(i).y, meshes.at(0)->vertices.at(i).z);
//		vertices[i].texture = D3DXVECTOR2(meshes.at(0)->vertices.at(i).tu, meshes.at(0)->vertices.at(i).tv);
//		vertices[i].normal = D3DXVECTOR3(meshes.at(0)->vertices.at(i).nx, meshes.at(0)->vertices.at(i).ny, meshes.at(0)->vertices.at(i).nz);
//
//		indices[i] = i;
//	}
//
//	// Set up the description of the static vertex buffer.
//    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
//    vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
//    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//    vertexBufferDesc.CPUAccessFlags = 0;
//    vertexBufferDesc.MiscFlags = 0;
//	vertexBufferDesc.StructureByteStride = 0;
//
//	// Give the subresource structure a pointer to the vertex data.
//    vertexData.pSysMem = vertices;
//	vertexData.SysMemPitch = 0;
//	vertexData.SysMemSlicePitch = 0;
//
//	// Now create the vertex buffer.
//    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
//	if(FAILED(result))
//	{
//		return false;
//	}
//
//	// Set up the description of the static index buffer.
//    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
//    indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
//    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
//    indexBufferDesc.CPUAccessFlags = 0;
//    indexBufferDesc.MiscFlags = 0;
//	indexBufferDesc.StructureByteStride = 0;
//
//	// Give the subresource structure a pointer to the index data.
//    indexData.pSysMem = indices;
//	indexData.SysMemPitch = 0;
//	indexData.SysMemSlicePitch = 0;
//
//	// Create the index buffer.
//	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
//	if(FAILED(result))
//	{
//		return false;
//	}
//
//	// Release the arrays now that the vertex and index buffers have been created and loaded.
//	delete [] vertices;
//	vertices = 0;
//
//	delete [] indices;
//	indices = 0;
//
//	return true;
//}

void ModelClass::ShutdownBuffers()
{
	// Release the index buffer.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}

void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

void ModelClass::RenderMeshes(int num)
{
	meshes.at(num)->Draw();
}


bool ModelClass::LoadTexture(ID3D11Device* device, const WCHAR* filename)
{
	bool result;

	// Create the texture object.
	TextureClass* texture = new TextureClass;
	if (!texture)
	{
		return false;
	}

	// Initialize the texture object.
	result = texture->Initialize(device, filename);
	if (!result)
	{
		return false;
	}

	m_Texture.push_back(texture);

	return true;
}


void ModelClass::ReleaseTexture()
{
	// Release the texture object.
	//if (m_Texture)
	//{
	//	m_Texture->Shutdown();
	//	delete m_Texture;
	//	m_Texture = 0;
	//}

	return;
}


//bool ModelClass::LoadModel(const char* filename)
//{
//	ifstream fin;
//	char input;
//	int i;
//
//
//	// Open the model file.
//	fin.open(filename);
//	
//	// If it could not open the file then exit.
//	if(fin.fail())
//	{
//		return false;
//	}
//
//	// Read up to the value of vertex count.
//	fin.get(input);
//	while(input != ':')
//	{
//		fin.get(input);
//	}
//
//	// Read in the vertex count.
//	fin >> m_vertexCount;
//
//	// Set the number of indices to be the same as the vertex count.
//	m_indexCount = m_vertexCount;
//
//	// Create the model using the vertex count that was read in.
//	m_model = new ModelType[m_vertexCount];
//	if(!m_model)
//	{
//		return false;
//	}
//
//	// Read up to the beginning of the data.
//	fin.get(input);
//	while(input != ':')
//	{
//		fin.get(input);
//	}
//	fin.get(input);
//	fin.get(input);
//
//	// Read in the vertex data.
//	for(i=0; i < m_vertexCount; i++)
//	{
//		fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
//		fin >> m_model[i].tu >> m_model[i].tv;
//		fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
//	}
//
//	// Close the model file.
//	fin.close();
//
//	return true;
//}


void ModelClass::ReleaseModel()
{
	//if(m_model)
	//{
	//	delete [] m_model;
	//	m_model = 0;
	//}

	return;
}


bool ModelClass::LoadModel(const string & filePath)
{
	Assimp::Importer importer;

	const aiScene* pScene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

	if (pScene == nullptr)
		return false;

	this->ProcessNode(pScene->mRootNode, pScene);

	//if (pScene->HasAnimations())
	//	ProcessAnimation(pScene, model);


	return true;
}

void ModelClass::ProcessNode(aiNode * node, const aiScene * scene)
{
	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(this->ProcessMesh(mesh, scene));
	}

	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		this->ProcessNode(node->mChildren[i], scene);
	}
}

Mesh* ModelClass::ProcessMesh(aiMesh * mesh, const aiScene * scene)
{
	// Data to fill
	vector<VertexType> vertices;
	vector<UINT> indices;

	//Get vertices
	for (UINT i = 0; i < mesh->mNumVertices; i++)
	{
		VertexType vertex;

		vertex.position.x = mesh->mVertices[i].x;
		vertex.position.y = mesh->mVertices[i].y;
		vertex.position.z = mesh->mVertices[i].z;

		if (mesh->mTextureCoords[0])
		{
			vertex.texture.x = (float)mesh->mTextureCoords[0][i].x;
			vertex.texture.y = (float)mesh->mTextureCoords[0][i].y;
		}

		vertices.push_back(vertex);
	}

	//Get indices
	for (UINT i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (UINT j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	m_indexCount.push_back(indices.size());


	Mesh* rt = new Mesh(device, deviceContext, vertices, indices, m_Texture.front());

	m_Texture.pop_front();

	return rt;
}

//void ModelClass::LoadAnimation(const string path, SkinModel* model, UINT flag)
//{
//	if (!pScene) return;
//
//	//애니메이션 정보 구성
//	if (pScene->HasAnimations())
//		ProcessAnimation(pScene, model);
//}
//
//void ModelClass::ProcessAnimation(const aiScene * pScene, SkinModel* skModel)
//{
//
//	//애니메이션의 갯수 만큼...
//	for (UINT i = 0; i < pScene->mNumAnimations; i++) {
//		auto aiAni = pScene->mAnimations[i];
//
//		float lastTime = 0.f;
//		Animation aniInfo;
//		aniInfo.SetDuration((float)aiAni->mDuration);
//		aniInfo.SetTickPerSecond((float)aiAni->mTicksPerSecond);
//
//		string aniName = (string)aiAni->mName.C_Str();
//		aniName = aniName.TrimLeft();
//
//		if (aniName != L"")
//			aniInfo.SetName((wstring)aniName);
//
//		//연결된 노드 만큼...
//		for (UINT j = 0; j < aiAni->mNumChannels; j++) {
//			auto aiAniNode = aiAni->mChannels[j];
//
//			AniNode aniNodeInfo;
//			aniNodeInfo.name = MYUTIL::ConvertToWString((CString)aiAniNode->mNodeName.C_Str());
//			UINT keyCnt = max(aiAniNode->mNumPositionKeys, aiAniNode->mNumRotationKeys);
//			keyCnt = max(keyCnt, aiAniNode->mNumScalingKeys);
//
//
//			//키 프레임 정보를 구성한다.
//			XMFLOAT3 translation = XMFLOAT3(0.f, 0.f, 0.f);
//			XMFLOAT3 scale = XMFLOAT3(0.f, 0.f, 0.f);
//			XMFLOAT4 rotation = XMFLOAT4(0.f, 0.f, 0.f, 0.f);
//			float	 time = 0.f;
//
//			//키 값은 있을 수도 있으며, 없을수도 있다.
//			//없는 경우 이전 값으로 채운다.
//			for (UINT k = 0; k < keyCnt; k++) {
//				if (aiAniNode->mNumPositionKeys > k) {
//					auto posKey = aiAniNode->mPositionKeys[k];
//					memcpy_s(&translation, sizeof(translation), &posKey.mValue, sizeof(posKey.mValue));
//					time = (float)aiAniNode->mPositionKeys[k].mTime;
//				}
//
//				if (aiAniNode->mNumRotationKeys > k) {
//					auto rotKey = aiAniNode->mRotationKeys[k];
//					rotation = XMFLOAT4(rotKey.mValue.x, rotKey.mValue.y, rotKey.mValue.z, rotKey.mValue.w);
//					time = (float)aiAniNode->mRotationKeys[k].mTime;
//				}
//
//				if (aiAniNode->mNumScalingKeys > k) {
//					auto scaleKey = aiAniNode->mScalingKeys[k];
//					memcpy_s(&scale, sizeof(scale), &scaleKey.mValue, sizeof(scaleKey.mValue));
//					time = (float)aiAniNode->mScalingKeys[k].mTime;
//				}
//
//				aniNodeInfo.keyFrame.emplace_back(KeyFrame{ time, translation, rotation, scale });
//			}
//
//			lastTime = max(aniNodeInfo.keyFrame.back().timePos, lastTime);
//			aniInfo.GetAniNodeList().emplace_back(aniNodeInfo);
//		}
//
//		aniInfo.SetLastFrame(lastTime);
//		skModel->GetAnimationList().emplace_back(aniInfo);
//	}
//}


//
//vector<Texture> ModelClass::loadMaterialTextures(aiMaterial * mat, aiTextureType type, string typeName, const aiScene * scene)
//{
//	vector<Texture> textures;
//	for (UINT i = 0; i < mat->GetTextureCount(type); i++)
//	{
//		aiString str;
//		mat->GetTexture(type, i, &str);
//		// Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
//		bool skip = false;
//		for (UINT j = 0; j < textures_loaded.size(); j++)
//		{
//			if (std::strcmp(textures_loaded[j].path.c_str(), str.C_Str()) == 0)
//			{
//				textures.push_back(textures_loaded[j]);
//				skip = true; // A texture with the same filepath has already been loaded, continue to next one. (optimization)
//				break;
//			}
//		}
//		if (!skip)
//		{   // If texture hasn't been loaded already, load it
//			HRESULT hr;
//			Texture texture;
//			if (textype == "embedded compressed texture")
//			{
//				int textureindex = getTextureIndex(&str);
//				texture.texture = getTextureFromModel(scene, textureindex);
//			}
//			else
//			{
//				string filename = string(str.C_Str());
//				filename = directory + '/' + filename;
//				wstring filenamews = wstring(filename.begin(), filename.end());
//				hr = CreateWICTextureFromFile(device, deviceContext, filenamews.c_str(), nullptr, &texture.texture);
//			}
//			texture.type = typeName;
//			texture.path = str.C_Str();
//			textures.push_back(texture);
//			this->textures_loaded.push_back(texture);  // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
//		}
//	}
//
//	return textures;
//}
//
//string ModelClass::determineTextureType(const aiScene * scene, aiMaterial * mat)
//{
//	aiString textypeStr;
//	mat->GetTexture(aiTextureType_DIFFUSE, 0, &textypeStr);
//	string textypeteststr = textypeStr.C_Str();
//	if (textypeteststr == "*0" || textypeteststr == "*1" || textypeteststr == "*2" || textypeteststr == "*3" || textypeteststr == "*4" || textypeteststr == "*5")
//	{
//		if (scene->mTextures[0]->mHeight == 0)
//		{
//			return "embedded compressed texture";
//		}
//		else
//		{
//			return "embedded non-compressed texture";
//		}
//	}
//	if (textypeteststr.find('.') != string::npos)
//	{
//		return "textures are on disk";
//	}
//}
//
//int ModelClass::getTextureIndex(aiString * str)
//{
//	string tistr;
//	tistr = str->C_Str();
//	tistr = tistr.substr(1);
//	return stoi(tistr);
//}
//
//ID3D11ShaderResourceView * ModelClass::getTextureFromModel(const aiScene * scene, int textureindex)
//{
//	HRESULT hr;
//	ID3D11ShaderResourceView *texture;
//
//	int* size = reinterpret_cast<int*>(&scene->mTextures[textureindex]->mWidth);
//
//	hr = CreateWICTextureFromMemory(device, deviceContext, reinterpret_cast<unsigned char*>(scene->mTextures[textureindex]->pcData), *size, nullptr, &texture);
//	if (FAILED(hr))
//	{
//		return false;
//	}
//
//	return texture;
//}
