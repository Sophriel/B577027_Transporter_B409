////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"

GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_LightShader = 0;
	m_Light = 0;
	m_Light1 = 0;
	m_Light2 = 0;

	m_Text = 0;

	Objs = 0;
	Polys = 0;

	CamPos.x = 0.0f;
	CamPos.y = 0.0f;
	CamPos.z = 0.0f;

	CamRot.x = 0.0f;
	CamRot.y = 0.0f;
	CamRot.z = 0.0f;

	PreX = 0.0f;
	PreY = 0.0f;

	PlayerScore = 0;
	ComScore = 0;

	GroundModel.model = 0;
	GroundModel.pos = D3DXVECTOR3(0.0f, -10.0f, 0.0f);
	LeftWallModel.model = 0;
	LeftWallModel.pos = D3DXVECTOR3(0.0f, -2.0f, -17.0f);
	RightWallModel.model = 0;
	RightWallModel.pos = D3DXVECTOR3(0.0f, -2.0f, 17.0f);
	LeftUpWallModel.model = 0;
	LeftUpWallModel.pos = D3DXVECTOR3(0.0f, 12.0f, -17.0f);
	RightUpWallModel.model = 0;
	RightUpWallModel.pos = D3DXVECTOR3(0.0f, 12.0f, 17.0f);
	PlayerModel.model = 0;
	PlayerModel.pos = D3DXVECTOR3(14.0f, 0.0f, 0.0f);
	ComModel.model = 0;
	ComModel.pos = D3DXVECTOR3(-14.0f, 0.0f, 0.0f);
	BallModel.model = 0;
	BallModel.pos = D3DXVECTOR3(0.0f, 10.0f, 0.0f);

	GroundModel.filename = "../Engine/data/objs/ground.obj";
	LeftWallModel.filename = "../Engine/data/objs/LeftWall.obj";
	RightWallModel.filename = "../Engine/data/objs/RightWall.obj";
	LeftUpWallModel.filename = "../Engine/data/objs/LeftUpWall.obj";
	RightUpWallModel.filename = "../Engine/data/objs/RightUpWall.obj";
	PlayerModel.filename = "../Engine/data/objs/Player.obj";
	ComModel.filename = "../Engine/data/objs/Player.obj";
	BallModel.filename = "../Engine/data/objs/Ball.obj";

	GroundModel.texturename = L"../Engine/data/textures/Grass.dds";
	LeftWallModel.texturename = L"../Engine/data/textures/Wall.dds";
	RightWallModel.texturename = L"../Engine/data/textures/Wall.dds";
	LeftUpWallModel.texturename = L"../Engine/data/textures/Wall.dds";
	RightUpWallModel.texturename = L"../Engine/data/textures/Wall.dds";
	PlayerModel.texturename = L"../Engine/data/textures/Player.dds";
	ComModel.texturename = L"../Engine/data/textures/Com.dds";
	BallModel.texturename = L"../Engine/data/textures/Ball.dds";

	GroundModel.bVec3[0] = 0;
	GroundModel.bVec3[1] = 200;
	GroundModel.bVec3[2] = 0;

	LeftWallModel.bVec3[0] = 0;
	LeftWallModel.bVec3[1] = 30;
	LeftWallModel.bVec3[2] = 30;

	RightWallModel.bVec3[0] = 0;
	RightWallModel.bVec3[1] = 30;
	RightWallModel.bVec3[2] = -30;

	LeftUpWallModel.bVec3[0] = 0;
	LeftUpWallModel.bVec3[1] = -20;
	LeftUpWallModel.bVec3[2] = 30;

	RightUpWallModel.bVec3[0] = 0;
	RightUpWallModel.bVec3[1] = -20;
	RightUpWallModel.bVec3[2] = -30;

	PlayerModel.bVec3[0] = -200;
	PlayerModel.bVec3[1] = 0;
	PlayerModel.bVec3[2] = 0;

	ComModel.bVec3[0] = 200;
	ComModel.bVec3[1] = 0;
	ComModel.bVec3[2] = 0;

	BallModel.bVec3[0] = 0;
	BallModel.bVec3[1] = 0;
	BallModel.bVec3[2] = 0;
}
GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}
GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;
	D3DXMATRIX baseViewMatrix;

	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if (!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(CamPos.x, CamPos.y, CamPos.z);
	m_Camera->SetRotation(CamRot.x, CamRot.y, CamRot.z);


	//  스카이박스 생성
	m_Skybox = new SkyboxClass;
	result = m_Skybox->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not Initialize Skybox.", L"Error", MB_OK);
		return false;
	}


	//  물리 월드 생성
	m_PhyWorld = new ODEclass;
	result = m_PhyWorld->Initialize();
	if (!result)
	{
		MessageBox(hwnd, L"Could not Initialize ODE World.", L"Error", MB_OK);
		return false;
	}

	// Create the model object.
	Models.push_back(&GroundModel);
	Models.push_back(&LeftWallModel);
	Models.push_back(&RightWallModel);
	Models.push_back(&LeftUpWallModel);
	Models.push_back(&RightUpWallModel);
	Models.push_back(&PlayerModel);
	Models.push_back(&ComModel);
	Models.push_back(&BallModel);

	//  obj파일들을 불러옵니다.
	ObjParser* parser = new ObjParser;

	for (unsigned int i = 0; i < Models.size(); i++)
	{
		Models.at(i)->model = new ModelClass();
		if (!Models.at(i)->model)
		{
			MessageBox(hwnd, L"Could not create the model object.", L"Error", MB_OK);
			return false;
		}

		parser->Parse(Models.at(i)->filename);

		Models.at(i)->model->Initialize(m_D3D->GetDevice(), "../B577027 이재형 기말 프로젝트 택배로봇 B409/model.txt", Models.at(i)->texturename);
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}

		//  강체 생성파트
		Models.at(i)->body = dBodyCreate(m_PhyWorld->GetWorldID());

		int preprocessFlags = (1U << dTRIDATAPREPROCESS_BUILD_FACE_ANGLES);
		dTriMeshDataID tmdata = dGeomTriMeshDataCreate();
		dGeomTriMeshDataBuildDouble(tmdata, parser->dVertices, 3 * sizeof(double), parser->vertexCount,
			(dTriIndex*)&(parser->dIndices.at(0)), Models.at(i)->model->GetIndexCount(), 3 * sizeof(dTriIndex));
		dGeomTriMeshDataPreprocess2(tmdata, preprocessFlags, NULL);

		Models.at(i)->geom = dCreateTriMesh(m_PhyWorld->GetSpaceID(), tmdata, 0, 0, 0);
		dGeomSetData(Models.at(i)->geom, tmdata);

		dGeomSetPosition(Models.at(i)->geom, Models.at(i)->pos.x, Models.at(i)->pos.y, Models.at(i)->pos.z);
		dMatrix3 Rotation;
		dRFromAxisAndAngle(Rotation, 1, 0, 0, M_PI / 2);
		dGeomSetRotation(Models.at(i)->geom, Rotation);

		//  질량 세팅
		dMass m;
		dMassSetTrimesh(&m, DENSITY, Models.at(i)->geom);
		dGeomSetPosition(Models.at(i)->geom, -m.c[0], -m.c[1], -m.c[2]);
		dMassTranslate(&m, -m.c[0], -m.c[1], -m.c[2]);

		dGeomSetBody(Models.at(i)->geom, Models.at(i)->body);
		//dBodySetMass(Models.at(i)->body, &m);
		dBodySetPosition(Models.at(i)->body, Models.at(i)->pos.x, Models.at(i)->pos.y, Models.at(i)->pos.z);

		m_PhyWorld->SetGeomIDandBounceVec3(m_PhyWorld, Models.at(i)->geom, Models.at(i)->bVec3, i);

		//  디버그 텍스트 표시
		Objs++;
		Polys += (parser->vertexCount) / 3;
	}
	
	dBodySetKinematic(GroundModel.body);
	dBodySetKinematic(LeftWallModel.body);
	dBodySetKinematic(RightWallModel.body);
	dBodySetKinematic(LeftUpWallModel.body);
	dBodySetKinematic(RightUpWallModel.body);
	dBodySetKinematic(ComModel.body);
	dBodySetKinematic(PlayerModel.body);

	dBodyAddForce(BallModel.body, 200, 100, 100);

	delete parser;


	// Create the light shader object.
	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the light object.
	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}

	// Initialize the light object.
	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.0f, -1.0f, 1.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(32.0f);	// specular power: lower value = greater effect

	m_Light1 = new LightClass;
	if (!m_Light)
	{
		return false;
	}
	m_Light1->SetDiffuseColor(0.0f, 1.0f, 0.0f, 0.0f);
	m_Light1->SetPosition(GroundModel.pos.x, GroundModel.pos.y, GroundModel.pos.z);

	m_Light2 = new LightClass;
	if (!m_Light)
	{
		return false;
	}
	m_Light2->SetDiffuseColor(1.0f, 1.0f, 1.0f, 0.0f);
	m_Light2->SetPosition(PlayerModel.pos.x + 1.0f, PlayerModel.pos.y, PlayerModel.pos.z);

	m_Light3 = new LightClass;
	if (!m_Light)
	{
		return false;
	}
	m_Light3->SetDiffuseColor(1.0f, 0.0f, 0.0f, 0.0f);
	m_Light3->SetPosition(ComModel.pos.x, ComModel.pos.y, ComModel.pos.z);

	m_Light4 = new LightClass;
	if (!m_Light)
	{
		return false;
	}
	m_Light4->SetDiffuseColor(0.0f, 0.0f, 1.0f, 0.0f);
	m_Light4->SetPosition(BallModel.pos.x, BallModel.pos.y, BallModel.pos.z);

	// Initialize a base view matrix with the camera for 2D user interface rendering.
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);

	// Create the text object.
	m_Text = new TextClass;
	if (!m_Text)
	{
		return false;
	}

	// Initialize the text object.
	result = m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), 
		hwnd, screenWidth, screenHeight, baseViewMatrix);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}

	CamPos.x = 50.0f;
	CamPos.y = 20.0f;
	CamPos.z = 0.0f;

	CamRot.x = 20.0f;
	CamRot.y = -90.0f;
	CamRot.z = 0.0f;


	// Create the sound object. 
	m_Sound = new SoundClass;
	if (!m_Sound)
	{
		return false;
	}
	// Initialize the sound object. 
	result = m_Sound->Initialize(hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct Sound.", L"Error", MB_OK);
		return false;
	}



	return true;
}


void GraphicsClass::Shutdown()
{
	// Release the sound object.
	if (m_Sound)
	{
		m_Sound->Shutdown();
		delete m_Sound;
		m_Sound = 0;
	}

	// Release the light object.
	if (m_Light4)
	{
		delete m_Light4;
		m_Light4 = 0;
	}
	if (m_Light3)
	{
		delete m_Light3;
		m_Light3 = 0;
	}
	if (m_Light2)
	{
		delete m_Light2;
		m_Light2 = 0;
	}
	if (m_Light1)
	{
		delete m_Light1;
		m_Light1 = 0;
	}
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the light shader object.
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}


	// Release the model object.
	for (unsigned int i = 0; i < Models.size(); i++)
	{
		Models.at(i)->model->Shutdown();
		delete Models.at(i)->model;
		Models.at(i)->model = 0;
	}

	dJointGroupDestroy(m_PhyWorld->GetContactgroupID());
	dSpaceDestroy(m_PhyWorld->GetSpaceID());
	dWorldDestroy(m_PhyWorld->GetWorldID());
	dCloseODE();

	if (m_Skybox)
	{
		m_Skybox->Shutdown();
		delete m_Skybox;
		m_Skybox = 0;
	}

	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the D3D object.
	if (m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	// Release the text object.
	if (m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}

	return;
}


bool GraphicsClass::Frame(int mouseX, int mouseY, int fps, int cpu, float frameTime)
{
	bool result;
	static float rotation = 0.0f;

	////  물리 실행
	m_PhyWorld->SimulationUpdate();
	for (unsigned int i = 0; i < Models.size(); i++)
	{
		Models.at(i)->pos.x = (float)dBodyGetPosition(Models.at(i)->body)[0];
		Models.at(i)->pos.y = (float)dBodyGetPosition(Models.at(i)->body)[1];
		Models.at(i)->pos.z = (float)dBodyGetPosition(Models.at(i)->body)[2];
	}
	ComFSM();
	ManageGame();

	//  카메라 이동
	CamRot.y -= (PreX - mouseX) * 0.1f;
	CamRot.x -= (PreY - mouseY) * 0.1f;

	PreX = (float)mouseX;
	PreY = (float)mouseY;

	m_Camera->SetPosition(CamPos.x, CamPos.y, CamPos.z);
	m_Camera->SetRotation(CamRot.x, CamRot.y, CamRot.z);


	//  스카이박스 설정
	m_Skybox->Frame(CamPos);


	//  Set the Screen Size.
	result = m_Text->SetScreen(m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}
	// Set the location of the mouse.
	result = m_Text->SetMousePosition(mouseX, mouseY, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}
	// Set the location of the camera.
	result = m_Text->SetCameraPosition(CamPos.x, CamPos.y, CamPos.z, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	// Set the frames per second.
	result = m_Text->SetFps(fps, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}
	// Set the cpu usage.
	result = m_Text->SetCpu(cpu, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	//  Set the Objs Count.
	result = m_Text->SetObjs(Objs, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}
	//  Set the Polygon usage.
	result = m_Text->SetPolys(Polys, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}


	// Update the rotation variable each frame.
	rotation += (float)D3DX_PI * 0.005f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	m_Light1->SetPosition(GroundModel.pos.x, GroundModel.pos.y, GroundModel.pos.z);
	m_Light2->SetPosition(PlayerModel.pos.x + 3.0f, PlayerModel.pos.y, PlayerModel.pos.z);
	m_Light3->SetPosition(ComModel.pos.x, ComModel.pos.y, ComModel.pos.z);
	m_Light4->SetPosition(BallModel.pos.x, BallModel.pos.y, BallModel.pos.z);

	// Render the graphics scene.
	result = Render(rotation);
	if (!result)
	{
		return false;
	}

	return true;
}


bool GraphicsClass::Render(float rotation)
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix, translateMatrix;
	bool result;

	D3DXVECTOR4 diffuseColor[4];
	D3DXVECTOR4 lightPosition[4];

	// Create the diffuse color array from the four light colors.
	diffuseColor[0] = m_Light1->GetDiffuseColor();
	diffuseColor[1] = m_Light2->GetDiffuseColor();
	diffuseColor[2] = m_Light3->GetDiffuseColor();
	diffuseColor[3] = m_Light4->GetDiffuseColor();

	// Create the light position array from the four light positions.
	lightPosition[0] = m_Light1->GetPosition();
	lightPosition[1] = m_Light2->GetPosition();
	lightPosition[2] = m_Light3->GetPosition();
	lightPosition[3] = m_Light4->GetPosition();

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	//  스카이박스
	m_Skybox->Render(m_D3D->GetDeviceContext(), m_D3D->GetRenderTarget(), m_D3D->GetDepthStencil(), viewMatrix, projectionMatrix);

	//  3차원 파트
	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	// Render the model using the light shader.

	// Rotate the world matrix by the rotation value so that the triangle will spin.
	//D3DXMatrixRotationY(&worldMatrix, rotation);
	//D3DXMatrixTranslation(&translateMatrix, -180.0f, 20.0f, 0.0f);
	//D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &translateMatrix);

	for (unsigned int i = 0; i < Models.size(); i++)
	{
		D3DXMatrixIdentity(&worldMatrix);
		D3DXMatrixTranslation(&translateMatrix, Models.at(i)->pos.x, Models.at(i)->pos.y, Models.at(i)->pos.z);
		D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &translateMatrix);

		Models.at(i)->model->Render(m_D3D->GetDeviceContext());
		//result = m_LightShader->Render(m_D3D->GetDeviceContext(), Models.at(i)->model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		//	Models.at(i)->model->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		//	m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
		result = m_LightShader->Render(m_D3D->GetDeviceContext(), Models.at(i)->model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
			Models.at(i)->model->GetTexture(), diffuseColor, lightPosition);

		if (!result)
		{
			return false;
		}
	}

	D3DXMatrixIdentity(&worldMatrix);

	//  2차원 파트
	// Turn off the Z buffer to begin all 2D rendering.
	m_D3D->TurnZBufferOff();

	// Turn on the alpha blending before rendering the text.
	m_D3D->TurnOnAlphaBlending();

	// Render the text strings.
	result = m_Text->Render(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	// Turn off alpha blending after rendering the text.
	m_D3D->TurnOffAlphaBlending();

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_D3D->TurnZBufferOn();


	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}


void GraphicsClass::SetData(UINT input)
{
	//  Change Texture Mip Mapping Option
	switch (input)
	{
	case 0:  //  Numpad0 Input
		m_LightShader->SetSampler(m_D3D->GetDevice(), 0);
		break;
	case 1:  //  Numpad1 Input
		m_LightShader->SetSampler(m_D3D->GetDevice(), 1);
		break;
	case 2:  //  Numpad2 Input
		m_LightShader->SetSampler(m_D3D->GetDevice(), 2);
		break;
	case 3:  //  Numpad3 Input
		m_LightShader->SetSampler(m_D3D->GetDevice(), 3);
		break;

	default:
		break;
	}
}

void GraphicsClass::GoForward()
{
	D3DXMATRIX Dir;
	D3DXMatrixIdentity(&Dir);
	D3DXVECTOR3 Direction(0, 0, 1);
	D3DXMatrixRotationYawPitchRoll(&Dir, CamRot.y * 0.0174532925f, CamRot.x * 0.0174532925f, CamRot.z * 0.0174532925f);
	D3DXVec3TransformCoord(&Direction, &Direction, &Dir);

	CamPos += Direction * speed;
}
void GraphicsClass::GoLeft()
{
	D3DXMATRIX Dir;
	D3DXMatrixIdentity(&Dir);
	D3DXVECTOR3 Direction(-1, 0, 0);
	D3DXMatrixRotationYawPitchRoll(&Dir, CamRot.y * 0.0174532925f, CamRot.x * 0.0174532925f, CamRot.z * 0.0174532925f);
	D3DXVec3TransformCoord(&Direction, &Direction, &Dir);

	CamPos += Direction * speed;
}
void GraphicsClass::GoBack()
{
	D3DXMATRIX Dir;
	D3DXMatrixIdentity(&Dir);
	D3DXVECTOR3 Direction(0, 0, -1);
	D3DXMatrixRotationYawPitchRoll(&Dir, CamRot.y * 0.0174532925f, CamRot.x * 0.0174532925f, CamRot.z * 0.0174532925f);
	D3DXVec3TransformCoord(&Direction, &Direction, &Dir);

	CamPos += Direction * speed;
}
void GraphicsClass::GoRight()
{
	D3DXMATRIX Dir;
	D3DXMatrixIdentity(&Dir);
	D3DXVECTOR3 Direction(1, 0, 0);
	D3DXMatrixRotationYawPitchRoll(&Dir, CamRot.y * 0.0174532925f, CamRot.x * 0.0174532925f, CamRot.z * 0.0174532925f);
	D3DXVec3TransformCoord(&Direction, &Direction, &Dir);

	CamPos += Direction * speed;
}

void GraphicsClass::MoveUp()
{
	PlayerModel.pos.y += 0.5f * speed;
	if (PlayerModel.pos.y > 20.0f)
		PlayerModel.pos.y = 20.0f;

	dBodySetPosition(PlayerModel.body, PlayerModel.pos.x, PlayerModel.pos.y, PlayerModel.pos.z);
}
void GraphicsClass::MoveDown()
{
	PlayerModel.pos.y -= 0.5f * speed;
	if (PlayerModel.pos.y < GroundModel.pos.y + 2.5f)
		PlayerModel.pos.y = GroundModel.pos.y + 2.5f;

	dBodySetPosition(PlayerModel.body, PlayerModel.pos.x, PlayerModel.pos.y, PlayerModel.pos.z);
}
void GraphicsClass::MoveLeft()
{
	PlayerModel.pos.z -= 0.5f * speed;
	if (PlayerModel.pos.z < LeftWallModel.pos.z)
		PlayerModel.pos.z = LeftWallModel.pos.z;

	dBodySetPosition(PlayerModel.body, PlayerModel.pos.x, PlayerModel.pos.y, PlayerModel.pos.z);
}
void GraphicsClass::MoveRight()
{
	PlayerModel.pos.z += 0.5f * speed;
	if (PlayerModel.pos.z > RightWallModel.pos.z)
		PlayerModel.pos.z = RightWallModel.pos.z;

	dBodySetPosition(PlayerModel.body, PlayerModel.pos.x, PlayerModel.pos.y, PlayerModel.pos.z);
}

void GraphicsClass::ComFSM()
{
	//ComModel.pos.y = BallModel.pos.y;
	//ComModel.pos.z = BallModel.pos.z;

	//  스무스하게 움직이도록 보정
	if (ComModel.pos.y <= BallModel.pos.y)
		ComModel.pos.y += (BallModel.pos.y - ComModel.pos.y) / 10.0f;
	else if (ComModel.pos.y > BallModel.pos.y)
		ComModel.pos.y -= (ComModel.pos.y - BallModel.pos.y) / 10.0f;

	if (ComModel.pos.z <= BallModel.pos.z)
		ComModel.pos.z += (BallModel.pos.z - ComModel.pos.z) / 10.0f;
	else if (ComModel.pos.z > BallModel.pos.z)
		ComModel.pos.z -= (ComModel.pos.z - BallModel.pos.z) / 10.0f;

	//  벽 안넘어가도록 제한
	if (ComModel.pos.y > 20.0f)
		ComModel.pos.y = 20.0f;
	else if (ComModel.pos.y < GroundModel.pos.y + 2.5f)
		ComModel.pos.y = GroundModel.pos.y + 2.5f;

	if (ComModel.pos.z < LeftWallModel.pos.z)
		ComModel.pos.z = LeftWallModel.pos.z;
	else if (ComModel.pos.z > RightWallModel.pos.z)
		ComModel.pos.z = RightWallModel.pos.z;

	dBodySetPosition(ComModel.body, ComModel.pos.x, ComModel.pos.y, ComModel.pos.z);
}

void GraphicsClass::ManageGame()
{
	//  장외
	if (BallModel.pos.y < GroundModel.pos.y - 3.0f || BallModel.pos.z < LeftWallModel.pos.z - 1.0f || BallModel.pos.z > RightWallModel.pos.z + 1.0f)
	{
		BallModel.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		dBodySetForce(BallModel.body, 0, 0, 0);
		dBodySetPosition(BallModel.body, BallModel.pos.x, BallModel.pos.y, BallModel.pos.z);
	}

	//  플레이어 방향 (적 승)
	if (BallModel.pos.x > PlayerModel.pos.x + 2.0f)
	{
		BallModel.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		dBodySetForce(BallModel.body, 0, 0, 0);
		dBodySetPosition(BallModel.body, BallModel.pos.x, BallModel.pos.y, BallModel.pos.z);

		ComScore++;

		m_Sound->PlayLose();
	}

	//  적 방향 (플레이어 승)
	else if (BallModel.pos.x < ComModel.pos.x - 2.0f)
	{
		BallModel.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		dBodySetForce(BallModel.body, 0, 0, 0);
		dBodySetPosition(BallModel.body, BallModel.pos.x, BallModel.pos.y, BallModel.pos.z);

		PlayerScore++;

		m_Sound->PlayWin();
	}

	m_Text->SetScore(PlayerScore, ComScore, m_D3D->GetDeviceContext());
}

