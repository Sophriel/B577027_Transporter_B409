////////////////////////////////////////////////////////////////////////////////
// Filename: skyboxclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "skyboxclass.h"


SkyboxClass::SkyboxClass()
{
	sphereIndexBuffer = 0;
	sphereVertBuffer = 0;

	VS_Buffer = 0;
	PS_Buffer = 0;
	SKYMAP_VS = 0;
	SKYMAP_PS = 0;
	SKYMAP_VS_Buffer = 0;
	SKYMAP_PS_Buffer = 0;

	smrv = 0;

	DSLessEqual = 0;
	RSCullNone = 0;

	CCWcullMode = 0;
	CWcullMode = 0;

	cbPerObjectBuffer = 0;
	cbPerFrameBuffer = 0;
	CubesTexSamplerState = 0;
	VS = 0;
	PS = 0;
}
SkyboxClass::SkyboxClass(const SkyboxClass& other)
{
}
SkyboxClass::~SkyboxClass()
{
}

bool SkyboxClass::Initialize(ID3D11Device* d3d11Device, ID3D11DeviceContext* d3d11DevCon, HWND hwnd)
{
	HRESULT result;

	result = CreateSphere(d3d11Device, 10, 10);
	if (FAILED(result))
	{
		MessageBox(hwnd, L"Could not initialize the Skybox Sphere.", L"Error", MB_OK);
		return false;
	}


	//  여기부터 쉐이더
	result = D3DX11CompileFromFile(L"../Engine/Effects.fx", 0, 0, "VS", "vs_4_0", 0, 0, 0, &VS_Buffer, 0, 0);
	if (FAILED(result))
	{
		MessageBox(hwnd, L"Could not Load the Skybox Shader.", L"Error", MB_OK);
		return false;
	}
	result = D3DX11CompileFromFile(L"../Engine/Effects.fx", 0, 0, "PS", "ps_4_0", 0, 0, 0, &PS_Buffer, 0, 0);
	if (FAILED(result))
	{
		MessageBox(hwnd, L"Could not Load the Skybox Shader.", L"Error", MB_OK);
		return false;
	}
	result = D3DX11CompileFromFile(L"../Engine/Effects.fx", 0, 0, "SKYMAP_VS", "vs_4_0", 0, 0, 0, &SKYMAP_VS_Buffer, 0, 0);
	if (FAILED(result))
	{
		MessageBox(hwnd, L"Could not Load the Skybox Shader.", L"Error", MB_OK);
		return false;
	}
	result = D3DX11CompileFromFile(L"../Engine/Effects.fx", 0, 0, "SKYMAP_PS", "ps_4_0", 0, 0, 0, &SKYMAP_PS_Buffer, 0, 0);
	if (FAILED(result))
	{
		MessageBox(hwnd, L"Could not Load the Skybox Shader.", L"Error", MB_OK);
		return false;
	}

	result = d3d11Device->CreateVertexShader(VS_Buffer->GetBufferPointer(), VS_Buffer->GetBufferSize(), NULL, &VS);
	if (FAILED(result))
	{
		MessageBox(hwnd, L"Could not Create the Skybox Shader.", L"Error", MB_OK);
		return false;
	}
	result = d3d11Device->CreatePixelShader(PS_Buffer->GetBufferPointer(), PS_Buffer->GetBufferSize(), NULL, &PS);
	if (FAILED(result))
	{
		MessageBox(hwnd, L"Could not Create the Skybox Shader.", L"Error", MB_OK);
		return false;
	}
	result = d3d11Device->CreateVertexShader(SKYMAP_VS_Buffer->GetBufferPointer(), SKYMAP_VS_Buffer->GetBufferSize(), NULL, &SKYMAP_VS);
	if (FAILED(result))
	{
		MessageBox(hwnd, L"Could not Create the Skybox Shader.", L"Error", MB_OK);
		return false;
	}
	result = d3d11Device->CreatePixelShader(SKYMAP_PS_Buffer->GetBufferPointer(), SKYMAP_PS_Buffer->GetBufferSize(), NULL, &SKYMAP_PS);
	if (FAILED(result))
	{
		MessageBox(hwnd, L"Could not Create the Skybox Shader.", L"Error", MB_OK);
		return false;
	}

	//Set Vertex and Pixel Shaders
	d3d11DevCon->VSSetShader(VS, 0, 0);
	d3d11DevCon->PSSetShader(PS, 0, 0);


	D3D11_BUFFER_DESC cbbd;
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));

	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeof(cbPerObject);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;

	result = d3d11Device->CreateBuffer(&cbbd, NULL, &cbPerObjectBuffer);
	if (FAILED(result))
	{
		MessageBox(hwnd, L"Could not Create the Skybox cbObject.", L"Error", MB_OK);
		return false;
	}

	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));

	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeof(cbPerFrame);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;

	result = d3d11Device->CreateBuffer(&cbbd, NULL, &cbPerFrameBuffer);
	if (FAILED(result))
	{
		MessageBox(hwnd, L"Could not Create the Skybox cbFrame.", L"Error", MB_OK);
		return false;
	}


	//  여기부터 텍스처
	D3DX11_IMAGE_LOAD_INFO loadSMInfo;
	loadSMInfo.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	// Load the texture
	// Then we will create a 2D texture from the file. This 2D texture will actually be an array now since we said 
	// we are loading a texture cube.
	ID3D11Texture2D* SMTexture = 0;

	result = D3DX11CreateTextureFromFile(d3d11Device, L"../Engine/data/textures/CitySkybox.dds", &loadSMInfo, 0, (ID3D11Resource**)&SMTexture, 0);
	if (FAILED(result))
	{
		MessageBox(hwnd, L"Could not Create Skybox Texture.", L"Error", MB_OK);
		return false;
	}

	// Create the textures description
	// Next we get the description of our texture so we can create a resource view description that matches the 
	// texture we loaded in.
	D3D11_TEXTURE2D_DESC SMTextureDesc;
	SMTexture->GetDesc(&SMTextureDesc);

	// Tell D3D We have a cube texture, which is an array of 2D textures
	// Now we will create the shader resource view description. We will say that this resource view is a texture cube,
	// or an array of 2D textures, so when the pixel shader is texturing a pixel, it will know how to use the 3D 
	// coordinates we give it, which are used to find the texel on the texture cube. Remember a 2D texture uses (u, v)
	// coordinates, well a 3D texture uses (u, v, w) coordinates.
	D3D11_SHADER_RESOURCE_VIEW_DESC SMViewDesc;
	SMViewDesc.Format = SMTextureDesc.Format;
	SMViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	SMViewDesc.TextureCube.MipLevels = SMTextureDesc.MipLevels;
	SMViewDesc.TextureCube.MostDetailedMip = 0;

	// Create the Resource view
	// And finally we create the resource view using the texture we loaded in from a file, the shader resource views 
	// description, and storing the shader resource view in smrv.
	result = d3d11Device->CreateShaderResourceView(SMTexture, &SMViewDesc, &smrv);
	if (FAILED(result))
	{
		MessageBox(hwnd, L"Could not Create Skybox Shader.", L"Error", MB_OK);
		return false;
	}

	// Describe the Sample State
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MaxAnisotropy = 4;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the Sample State
	result = d3d11Device->CreateSamplerState(&sampDesc, &CubesTexSamplerState);


	//  여기부터 컬링
	D3D11_RASTERIZER_DESC cmdesc;

	ZeroMemory(&cmdesc, sizeof(D3D11_RASTERIZER_DESC));
	cmdesc.FillMode = D3D11_FILL_SOLID;
	cmdesc.CullMode = D3D11_CULL_BACK;
	cmdesc.FrontCounterClockwise = true;
	result = d3d11Device->CreateRasterizerState(&cmdesc, &CCWcullMode);

	cmdesc.FrontCounterClockwise = false;

	result = d3d11Device->CreateRasterizerState(&cmdesc, &CWcullMode);

	// At the bottom of our InitScene() function, we create two new render states. One Rasterizer state, and one 
	// depth/stencil state. The rasterizer state will be used to disable culling, so that both the back and front 
	// sides of our skybox's geometry will be rendered. We do this because when we created our sphere, half of the 
	// sides are backwards, and the other half are forwards.
	cmdesc.CullMode = D3D11_CULL_NONE;
	result = d3d11Device->CreateRasterizerState(&cmdesc, &RSCullNone);

	// Now let's look at the depth/stencil state. We create a depth/stencil state by filling out a 
	// D3D11_DEPTH_STENCIL_DESC structure, calling ID3D11Device::CreateDepthStencilState(), and storing the state 
	// in an ID3D11DepthStencilState object.
	// In our skybox's VS, we will be setting its z value to 1.0f (which we will do by setting z to w, since w is 1.0f).
	// We do this so that it will always be the furthest possible from the camera, to make it look like it is 
	// infinitely far away, since all objects in your scene will be under the sky (usually). Just setting the z value 
	// to 1.0f in the vertex shader is not enough though, as some other object in your scene that are very far away 
	// might also end up with a z value of 1.0f. Because of this, we need to make sure that any other objects with 
	// the same z value or less (closer to the screen) are drawn, and not the pixels from the skybox. We can do this 
	// by setting the DepthFunc member to D3D11_COMPARISON_LESS_EQUAL.
	D3D11_DEPTH_STENCIL_DESC dssDesc;
	ZeroMemory(&dssDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dssDesc.DepthEnable = true;
	dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dssDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	d3d11Device->CreateDepthStencilState(&dssDesc, &DSLessEqual);

	return true;
}

bool SkyboxClass::CreateSphere(ID3D11Device* d3d11Device, int LatLines, int LongLines)
{
	HRESULT result;

	NumSphereVertices = ((LatLines - 2) * LongLines) + 2;
	NumSphereFaces = ((LatLines - 3)*(LongLines) * 2) + (LongLines * 2);

	float sphereYaw = 0.0f;
	float spherePitch = 0.0f;

	Vertex* vertices;
	vertices = new Vertex[NumSphereVertices];
	if (!vertices)
	{
		return false;
	}

	D3DXVECTOR4 currVertPos(0.0f, 0.0f, 1.0f, 0.0f);

	vertices[0].pos.x = 0.0f;
	vertices[0].pos.y = 0.0f;
	vertices[0].pos.z = 1.0f;

	for (int i = 0; i < LatLines - 2; ++i)
	{
		spherePitch = (i + 1) * (3.14f / (LatLines - 1));
		D3DXMatrixRotationX(&Rotationx, spherePitch);
		for (int j = 0; j < LongLines; ++j)
		{
			sphereYaw = j * (6.28f / (LongLines));
			D3DXMatrixRotationZ(&Rotationy, sphereYaw);
			D3DXVec4Transform(&currVertPos, new D3DXVECTOR4(0.0f, 0.0f, 1.0f, 0.0f), &(Rotationx * Rotationy));
			D3DXVec4Normalize(&currVertPos, &currVertPos);
			vertices[i*LongLines + j + 1].pos.x = currVertPos.x;
			vertices[i*LongLines + j + 1].pos.y = currVertPos.y;
			vertices[i*LongLines + j + 1].pos.z = currVertPos.z;
		}
	}

	vertices[NumSphereVertices - 1].pos.x = 0.0f;
	vertices[NumSphereVertices - 1].pos.y = 0.0f;
	vertices[NumSphereVertices - 1].pos.z = -1.0f;


	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * NumSphereVertices;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;
	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = vertices;

	result = d3d11Device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &sphereVertBuffer);
	if (FAILED(result))
	{
		return false;
	}


	std::vector<int> indices(NumSphereFaces * 3);

	int k = 0;
	for (int l = 0; l < LongLines - 1; ++l)
	{
		indices[k] = 0;
		indices[k + 1] = l + 1;
		indices[k + 2] = l + 2;
		k += 3;
	}

	indices[k] = 0;
	indices[k + 1] = LongLines;
	indices[k + 2] = 1;
	k += 3;

	for (int i = 0; i < LatLines - 3; ++i)
	{
		for (int j = 0; j < LongLines - 1; ++j)
		{
			indices[k] = i * LongLines + j + 1;
			indices[k + 1] = i * LongLines + j + 2;
			indices[k + 2] = (i + 1)*LongLines + j + 1;

			indices[k + 3] = (i + 1)*LongLines + j + 1;
			indices[k + 4] = i * LongLines + j + 2;
			indices[k + 5] = (i + 1)*LongLines + j + 2;

			k += 6; // next quad
		}

		indices[k] = (i*LongLines) + LongLines;
		indices[k + 1] = (i*LongLines) + 1;
		indices[k + 2] = ((i + 1)*LongLines) + LongLines;

		indices[k + 3] = ((i + 1)*LongLines) + LongLines;
		indices[k + 4] = (i*LongLines) + 1;
		indices[k + 5] = ((i + 1)*LongLines) + 1;

		k += 6;
	}

	for (int l = 0; l < LongLines - 1; ++l)
	{
		indices[k] = NumSphereVertices - 1;
		indices[k + 1] = (NumSphereVertices - 1) - (l + 1);
		indices[k + 2] = (NumSphereVertices - 1) - (l + 2);
		k += 3;
	}

	indices[k] = NumSphereVertices - 1;
	indices[k + 1] = (NumSphereVertices - 1) - LongLines;
	indices[k + 2] = NumSphereVertices - 2;

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * NumSphereFaces * 3;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];

	result = d3d11Device->CreateBuffer(&indexBufferDesc, &iinitData, &sphereIndexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void SkyboxClass::Frame(D3DXVECTOR3 CamPos)
{
	//Reset sphereWorld
	D3DXMatrixIdentity(&sphereWorld);

	//Define sphereWorld's world space matrix
	D3DXMatrixScaling(&Scale, 5.0f, 5.0f, 5.0f);
	//Make sure the sphere is always centered around camera
	D3DXMatrixTranslation(&Translation, CamPos.x, CamPos.y, CamPos.z);
	//Set sphereWorld's world space using the transformations
	sphereWorld = Scale * Translation;
}

void SkyboxClass::Render(ID3D11DeviceContext* d3d11DevCon,
	ID3D11RenderTargetView* renderTargetView, ID3D11DepthStencilView* depthStencilView,
	D3DXMATRIX camView, D3DXMATRIX camProjection)
{
	//Clear our render target and depth/stencil view
	float bgColor[4] = { 0.3f, 0.3f, 0.3f, 1.0f };
	d3d11DevCon->ClearRenderTargetView(renderTargetView, bgColor);
	d3d11DevCon->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//Set our Render Target
	d3d11DevCon->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

	//Set the default blend state (no blending) for opaque objects
	d3d11DevCon->OMSetBlendState(0, 0, 0xffffffff);

	//Set Vertex and Pixel Shaders
	d3d11DevCon->VSSetShader(VS, 0, 0);
	d3d11DevCon->PSSetShader(PS, 0, 0);


	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	// Set the spheres index buffer
	d3d11DevCon->IASetIndexBuffer(sphereIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	// Set the spheres vertex buffer
	d3d11DevCon->IASetVertexBuffers(0, 1, &sphereVertBuffer, &stride, &offset);

	// Set the world view projection matrix and send it to the constant buffer in effect file
	WVP = sphereWorld * camView * camProjection;
	D3DXMatrixTranspose(&cbPerObj.WVP, &WVP);
	D3DXMatrixTranspose(&cbPerObj.World, &sphereWorld);
	d3d11DevCon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
	d3d11DevCon->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);
	// Send our skymap resource view to pixel shader
	d3d11DevCon->PSSetShaderResources(0, 1, &smrv);
	d3d11DevCon->PSSetSamplers(0, 1, &CubesTexSamplerState);

	// Set the new VS and PS shaders
	d3d11DevCon->VSSetShader(SKYMAP_VS, 0, 0);
	d3d11DevCon->PSSetShader(SKYMAP_PS, 0, 0);
	// Set the new depth/stencil and RS states
	d3d11DevCon->OMSetDepthStencilState(DSLessEqual, 0);
	d3d11DevCon->RSSetState(RSCullNone);
	d3d11DevCon->DrawIndexed(NumSphereFaces * 3, 0, 0);

	// Set the default VS shader and depth/stencil state
	d3d11DevCon->VSSetShader(VS, 0, 0);
	d3d11DevCon->OMSetDepthStencilState(NULL, 0);
}

void SkyboxClass::Shutdown()
{
	if (sphereIndexBuffer)
	{
		sphereIndexBuffer->Release();
		sphereIndexBuffer = 0;
	}
	if (sphereVertBuffer)
	{
		sphereVertBuffer->Release();
		sphereVertBuffer = 0;
	}

	if (VS_Buffer)
	{
		VS_Buffer->Release();
		VS_Buffer = 0;
	}
	if (PS_Buffer)
	{
		PS_Buffer->Release();
		PS_Buffer = 0;
	}
	if (SKYMAP_VS)
	{
		SKYMAP_VS->Release();
		SKYMAP_VS = 0;
	}
	if (SKYMAP_PS)
	{
		SKYMAP_PS->Release();
		SKYMAP_PS = 0;
	}
	if (SKYMAP_VS_Buffer)
	{
		SKYMAP_VS_Buffer->Release();
		SKYMAP_VS_Buffer = 0;
	}
	if (SKYMAP_PS_Buffer)
	{
		SKYMAP_PS_Buffer->Release();
		SKYMAP_PS_Buffer = 0;
	}

	if (smrv)
	{
		smrv->Release();
		smrv = 0;
	}

	if (DSLessEqual)
	{
		DSLessEqual->Release();
		DSLessEqual = 0;
	}
	if (RSCullNone)
	{
		RSCullNone->Release();
		RSCullNone = 0;
	}

	if (CCWcullMode)
	{
		CCWcullMode->Release();
		CCWcullMode = 0;
	}
	if (CWcullMode)
	{
		CWcullMode->Release();
		CWcullMode = 0;
	}

	if (cbPerObjectBuffer)
	{
		cbPerObjectBuffer->Release();
		cbPerObjectBuffer = 0;
	}
	if (CubesTexSamplerState)
	{
		cbPerFrameBuffer->Release();
		cbPerFrameBuffer = 0;
	}
	if (CubesTexSamplerState)
	{
		CubesTexSamplerState->Release();
		CubesTexSamplerState = 0;
	}

	if (VS)
	{
		VS->Release();
		VS = 0;
	}
	if (PS)
	{
		PS->Release();
		PS = 0;
	}

}