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
	CamPos.y = 200.0f;
	CamPos.z = -500.0f;

	CamRot.x = 0.0f;
	CamRot.y = 0.0f;
	CamRot.z = 0.0f;

	PreX = 0.0f;
	PreY = 0.0f;

	PlayerScore = 0;
	ComScore = 0;

	GroundModel.model = 0;
	GroundModel.pos = D3DXVECTOR3(-100.0f, 0.0f, 0.0f);
	GroundModel.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	GroundModel.box = D3DXVECTOR3(100.0f, 10.0f, 650.0f);
	GroundModel1.model = 0;
	GroundModel1.pos = D3DXVECTOR3(100.0f, 0.0f, 0.0f);
	GroundModel1.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	GroundModel1.box = D3DXVECTOR3(100.0f, 10.0f, 650.0f);
	LeftWallModel.model = 0;
	LeftWallModel.pos = D3DXVECTOR3(200.0f, 0.0f, 0.0f);
	LeftWallModel.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	LeftWallModel.box = D3DXVECTOR3(10.0f, 2500.0f, 100.0f);
	RightWallModel.model = 0;
	RightWallModel.pos = D3DXVECTOR3(-200.0f, 0.0f, 0.0f);
	RightWallModel.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	RightWallModel.box = D3DXVECTOR3(10.0f, 2500.0f, 100.0f);

	PlayerModel.model = 0;
	PlayerModel.pos = D3DXVECTOR3(-100.0f, 40.0f, 0.0f);
	PlayerModel.rot = D3DXVECTOR3(0.0f, -90.0f, 0.0f);
	PlayerModel.box = D3DXVECTOR3(50.0f, 40.0f, 20.0f);
	ComModel.model = 0;
	ComModel.pos = D3DXVECTOR3(100.0f, 30.0f, 0.0f);
	ComModel.rot = D3DXVECTOR3(0.0f, 90.0f, 0.0f);
	ComModel.box = D3DXVECTOR3(50.0f, 30.0f, 20.0f);

	GroundModel.filename = "../Engine/data/objs/ground.obj";
	GroundModel1.filename = "../Engine/data/objs/ground.obj";
	LeftWallModel.filename = "../Engine/data/objs/SideWall.obj";
	RightWallModel.filename = "../Engine/data/objs/SideWall.obj";
	PlayerModel.filename = "../Engine/data/objs/Player.fbx";
	ComModel.filename = "../Engine/data/objs/com.fbx";

	GroundModel.texturename.push_back(L"../Engine/data/textures/metal.dds");
	GroundModel1.texturename.push_back(L"../Engine/data/textures/metal.dds");
	LeftWallModel.texturename.push_back(L"../Engine/data/textures/metal.dds");
	RightWallModel.texturename.push_back(L"../Engine/data/textures/metal.dds");
	PlayerModel.texturename.push_back(L"../Engine/data/textures/Player.png");

	ComModel.texturename.push_back(L"../Engine/data/textures/face_color_map.png");
	ComModel.texturename.push_back(L"../Engine/data/textures/body_color_map.png");
	ComModel.texturename.push_back(L"../Engine/data/textures/body_color_map.png");
	ComModel.texturename.push_back(L"../Engine/data/textures/body_color_map.png");
	ComModel.texturename.push_back(L"../Engine/data/textures/body_color_map.png");
	ComModel.texturename.push_back(L"../Engine/data/textures/eyes_specular_map.png");
	ComModel.texturename.push_back(L"../Engine/data/textures/eyes_specular_map.png");
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
	Models.push_back(&GroundModel1);
	Models.push_back(&LeftWallModel);
	Models.push_back(&RightWallModel);
	Models.push_back(&PlayerModel);
	Models.push_back(&ComModel);

	for (unsigned int i = 0; i < Models.size(); i++)
	{
		Models.at(i)->model = new ModelClass();
		if (!Models.at(i)->model)
		{
			MessageBox(hwnd, L"Could not create the model object.", L"Error", MB_OK);
			return false;
		}

		for (int j = 0; j < Models.at(i)->texturename.size(); j++)
		{
			Models.at(i)->model->InitTextures(m_D3D->GetDevice(), Models.at(i)->texturename.at(j));
		}

		Models.at(i)->model->Initialize(m_D3D->GetDeviceContext(), Models.at(i)->filename);
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}

		//  강체 생성파트
		Models.at(i)->body = dBodyCreate(m_PhyWorld->GetWorldID());
		Models.at(i)->geom = dCreateBox(m_PhyWorld->GetSpaceID(), Models.at(i)->box.x, Models.at(i)->box.y, Models.at(i)->box.z);

		dGeomSetPosition(Models.at(i)->geom, Models.at(i)->pos.x, Models.at(i)->pos.y, Models.at(i)->pos.z);
		dMatrix3 Rotation;
		dRFromAxisAndAngle(Rotation, 1, 0, 0, M_PI / 2);
		dGeomSetRotation(Models.at(i)->geom, Rotation);

		//  질량 세팅
		dMass m;
		dMassSetBox(&m, DENSITY, 1, 1, 1);
		dGeomSetPosition(Models.at(i)->geom, -m.c[0], -m.c[1], -m.c[2]);
		dMassTranslate(&m, -m.c[0], -m.c[1], -m.c[2]);

		dGeomSetBody(Models.at(i)->geom, Models.at(i)->body);
		dBodySetMass(Models.at(i)->body, &m);
		dBodySetPosition(Models.at(i)->body, Models.at(i)->pos.x, Models.at(i)->pos.y, Models.at(i)->pos.z);


		//  디버그 텍스트 표시
		Objs++;
		Polys += Models.at(i)->model->GetIndexCount(0) / 3;
	}

	dBodySetKinematic(GroundModel.body);
	dBodySetKinematic(GroundModel1.body);
	dBodySetKinematic(LeftWallModel.body);
	dBodySetKinematic(RightWallModel.body);

	//dGeomSetPosition(PlayerModel.geom, PlayerModel.pos.x, PlayerModel.pos.y + 190, PlayerModel.pos.z);
	//dGeomSetPosition(ComModel.geom, ComModel.pos.x, ComModel.pos.y + 190, ComModel.pos.z);

	//dBodySetPosition(PlayerModel.body, PlayerModel.pos.x, PlayerModel.pos.y + 190, PlayerModel.pos.z);
	//dBodySetPosition(ComModel.body, ComModel.pos.x, ComModel.pos.y + 190, ComModel.pos.z);

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
	m_Light1->SetDiffuseColor(0.0f, 0.0f, 1.0f, 0.0f);
	m_Light1->SetPosition(GroundModel.pos.x, GroundModel.pos.y, GroundModel.pos.z);

	m_Light2 = new LightClass;
	if (!m_Light)
	{
		return false;
	}
	m_Light2->SetDiffuseColor(1.0f, 0.0f, 0.0f, 0.0f);
	m_Light2->SetPosition(GroundModel1.pos.x, GroundModel1.pos.y, GroundModel1.pos.z);

	m_Light3 = new LightClass;
	if (!m_Light)
	{
		return false;
	}
	m_Light3->SetDiffuseColor(1.0f, 1.0f, 1.0f, 0.0f);
	m_Light3->SetPosition(PlayerModel.pos.x, PlayerModel.pos.y, PlayerModel.pos.z);

	m_Light4 = new LightClass;
	if (!m_Light)
	{
		return false;
	}
	m_Light4->SetDiffuseColor(1.0f, 1.0f, 1.0f, 0.0f);
	m_Light4->SetPosition(ComModel.pos.x, ComModel.pos.y, ComModel.pos.z);

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

	//  물리 실행
	m_PhyWorld->SimulationUpdate();
	for (unsigned int i = 0; i < Models.size(); i++)
	{
		Models.at(i)->pos.x = (float)dBodyGetPosition(Models.at(i)->body)[0];
		Models.at(i)->pos.y = (float)dBodyGetPosition(Models.at(i)->body)[1];
		Models.at(i)->pos.z = (float)dBodyGetPosition(Models.at(i)->body)[2];

		Models.at(i)->rot.x = (float)dBodyGetRotation(Models.at(i)->body)[0];
		Models.at(i)->rot.y = (float)dBodyGetRotation(Models.at(i)->body)[1];
		Models.at(i)->rot.z = (float)dBodyGetRotation(Models.at(i)->body)[2];
	}

	//PlayerModel.pos.y = (float)dBodyGetPosition(PlayerModel.body)[1] - 190;
	//ComModel.pos.y = (float)dBodyGetPosition(ComModel.body)[1] - 190;
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

	m_Light1->SetPosition(GroundModel.pos.x, GroundModel.pos.y + 10.0f, GroundModel.pos.z);
	m_Light2->SetPosition(GroundModel1.pos.x, GroundModel1.pos.y + 10.0f, GroundModel1.pos.z);
	m_Light3->SetPosition(PlayerModel.pos.x, PlayerModel.pos.y + 10.0f, PlayerModel.pos.z);
	m_Light4->SetPosition(ComModel.pos.x, ComModel.pos.y + 10.0f, ComModel.pos.z);

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

	for (unsigned int i = 0; i < Models.size(); i++)
	{
		D3DXMatrixIdentity(&worldMatrix);
		D3DXMatrixIdentity(&translateMatrix);
		D3DXMatrixRotationYawPitchRoll(&worldMatrix, Models.at(i)->rot.y * 0.0174532925f, Models.at(i)->rot.x * 0.0174532925f, Models.at(i)->rot.z * 0.0174532925f);
		D3DXMatrixTranslation(&translateMatrix, Models.at(i)->pos.x, Models.at(i)->pos.y, Models.at(i)->pos.z);
		D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &translateMatrix);

		for (int j = 0; j < Models.at(i)->model->meshes.size(); j++)
		{
			Models.at(i)->model->Render(m_D3D->GetDeviceContext(), j);

			result = m_LightShader->Render(m_D3D->GetDeviceContext(), Models.at(i)->model->GetIndexCount(j), worldMatrix, viewMatrix, projectionMatrix,
				Models.at(i)->model->GetTexture(j), diffuseColor, lightPosition);

			if (!result)
			{
				return false;
			}
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
	dBodyAddForce(PlayerModel.body, 0, 500, 0);
}
void GraphicsClass::MoveDown()
{
	dBodyAddForce(PlayerModel.body, 0, 0, 0);
}
void GraphicsClass::MoveLeft()
{
	dBodyAddForce(PlayerModel.body, -100, 0, 0);
}
void GraphicsClass::MoveRight()
{
	dBodyAddForce(PlayerModel.body, 100, 0, 00);
}

void GraphicsClass::ManageGame()
{
	if (PlayerModel.pos.y < GroundModel.pos.y - 10.0f )
	{
		dBodySetPosition(PlayerModel.body, -100, 50, 0);
		ComScore++;
		m_Sound->PlayFall();
	}

	if (ComModel.pos.y < GroundModel.pos.y - 10.0f)
	{
		dBodySetPosition(ComModel.body, 100, 50, 0);
		PlayerScore++;
		m_Sound->PlayFall();
	}	
	
	m_Text->SetScore(PlayerScore, ComScore, m_D3D->GetDeviceContext());
}

