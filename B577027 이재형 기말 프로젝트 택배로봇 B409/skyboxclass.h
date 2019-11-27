#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: skyboxclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _SKYBOXCLASS_H_
#define _SKYBOXCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>
#include <D3DX11tex.h>
#include <vector>


///////////////////////
// MY CLASS INCLUDES //
///////////////////////


////////////////////////////////////////////////////////////////////////////////
// Class name: SkyboxClass
////////////////////////////////////////////////////////////////////////////////
class SkyboxClass
{
	struct Vertex	//Overloaded Vertex Structure
	{
		Vertex() {}
		Vertex(float x, float y, float z,
			float u, float v,
			float nx, float ny, float nz)
			: pos(x, y, z), texCoord(u, v), normal(nx, ny, nz) {}

		D3DXVECTOR3 pos;
		D3DXVECTOR2 texCoord;
		D3DXVECTOR3 normal;
	};

	struct cbPerObject
	{
		D3DXMATRIX WVP;
		D3DXMATRIX World;
	};

	struct Light
	{
		Light()
		{
			ZeroMemory(this, sizeof(Light));
		}
		D3DXVECTOR3 dir;
		float pad;
		D3DXVECTOR4 ambient;
		D3DXVECTOR4 diffuse;
	};

	struct cbPerFrame
	{
		Light light;
	};

public:
	SkyboxClass();
	SkyboxClass(const SkyboxClass&);
	~SkyboxClass();

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* d3d11DevCon, HWND hwnd);
	void Frame(D3DXVECTOR3 CamPos);
	void Render(ID3D11DeviceContext* deviceContext, 
		ID3D11RenderTargetView* renderTargetView, ID3D11DepthStencilView* depthStencilView, 
		D3DXMATRIX camView, D3DXMATRIX camProjection);
	void Shutdown();

	int NumSphereVertices;
	int NumSphereFaces;


private:
	bool CreateSphere(ID3D11Device* d3d11Device, int LatLines, int LongLines);

private:
	D3DXMATRIX Rotationx;
	D3DXMATRIX Rotationy;

	D3DXMATRIX sphereWorld;

	D3DXMATRIX Rotation;
	D3DXMATRIX Scale;
	D3DXMATRIX Translation;
	float rot = 0.01f;

private:
	ID3D11Buffer* sphereIndexBuffer;
	ID3D11Buffer* sphereVertBuffer;

	ID3D10Blob* VS_Buffer;
	ID3D10Blob* PS_Buffer;
	ID3D11VertexShader* SKYMAP_VS;
	ID3D11PixelShader* SKYMAP_PS;
	ID3D10Blob* SKYMAP_VS_Buffer;
	ID3D10Blob* SKYMAP_PS_Buffer;

	ID3D11ShaderResourceView* smrv;

	ID3D11DepthStencilState* DSLessEqual;
	ID3D11RasterizerState* RSCullNone;

	ID3D11RasterizerState* CCWcullMode;
	ID3D11RasterizerState* CWcullMode;

private:
	D3DXMATRIX WVP;

	cbPerObject cbPerObj;
	ID3D11Buffer* cbPerObjectBuffer;
	ID3D11Buffer* cbPerFrameBuffer;

	ID3D11SamplerState* CubesTexSamplerState;
	ID3D11VertexShader* VS;
	ID3D11PixelShader* PS;
};


/////////////////////////
// FUNCTION PROTOTYPES //
/////////////////////////


/////////////
// GLOBALS //
/////////////


#endif