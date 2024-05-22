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

	m_day = Day::Afternoon;

	m_Start = 0;

	m_Skybox = 0;

	spawnPoints.push_back(XMFLOAT3(-12, 0.2f, 0));
	spawnPoints.push_back(XMFLOAT3(0, 0.2f, 0));
	spawnPoints.push_back(XMFLOAT3(12, 0.2f, 0));
	spawnPoints.push_back(XMFLOAT3(24, 0.2f, 0));

	spawnDelay = 5000;

	srand((unsigned int)time(NULL));
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}

D3DClass* GraphicsClass::GetD3DClass()
{
	return m_D3D;
}

CameraClass* GraphicsClass::GetCameraClass()
{
	return m_Camera;
}

LightClass* GraphicsClass::GetLight()
{
	return m_Light;
}



bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	XMMATRIX baseViewMatrix;

	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if(!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if(!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(5.0f, 0.5f, -2.0f);	// for chair
	

	m_ParticleShader = new ParticleShaderClass;
	if (!m_ParticleShader)
	{
		return false;
	}

	// 파티클 셰이더 개체를 초기화합니다.
	if (!m_ParticleShader->Initialize(m_D3D->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"Could not initialize the particle shader object.", L"Error", MB_OK);
		return false;
	}

	

	result = InitAllCars(hwnd);
	if (!result)
	{
		return false;
	}

	result = InitAllInteractableModels(hwnd);
	if (!result)
	{
		return false;
	}

	result = InitAllModels(hwnd);
	if (!result)
	{
		return false;
	}

	result = InitAllInstanceModels(hwnd);
	if (!result)
	{
		return false;
	}

	result = InitAllBillboardModels(hwnd);
	if (!result)
	{
		return false;
	}

	

	result = InitAllEnemys(hwnd);
	if (!result)
	{
		return false;
	}

	result = InitGunModels(hwnd);
	if (!result)
	{
		return false;
	}

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

	ChangeLightSetting();

	// Create the texture shader object.
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}

	// Initialize the texture shader object.
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);

	result = InitAllBitmap(screenWidth, screenHeight, hwnd);
	if (!result)
	{
		return false;
	}

	int txtCount = 10;

	for (int  i = 0; i < txtCount; i++)
	{
		// Create the text object.
		TextClass* txt = new TextClass;
		if (!txt)
		{
			return false;
		}

		// Initialize the text object.
		result = txt->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
			return false;
		}

		m_Texts.push_back(txt);
	}

	// Create the texture shader object.
	m_Skybox = new Skybox;
	if (!m_Skybox)
	{
		return false;
	}

	m_Skybox->InitScene(m_D3D->GetDevice()); // 스카이 박스 초기화


	return true;
}



void GraphicsClass::OnMouseDown()
{
	gun->OnClickDown(m_InteractableModels);
}

void GraphicsClass::OnMouseUp()
{
	gun->OnClickUp();
}


bool GraphicsClass::InitGunModels(HWND hwnd)
{
	bool result;

	vector<File*> files;

	// Block1

	files.push_back(new File("Player", L"./data/GunAndArm1.obj", L"./data/GunAndArm.dds",
		ModelData(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f)))); // 여기에 모델 이름, 주소 추가

	int size = files.size();

	for (int i = 0; i < size; i++)
	{
		gun = new GunClass(m_Camera, m_D3D, hwnd);
		if (!gun) return false;

		// Initialize the model object.
		result = gun->Initialize(m_D3D->GetDevice(), files[i]->GetModelName(), files[i]->GetFileName(), files[i]->GetFilePath(), files[i]->GetModelData());
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}

		m_InteractableModels.push_back(gun);
	}

	return true;
}

bool GraphicsClass::InitAllInteractableModels(HWND hwnd)
{
	bool result;

	vector<File*> files;

	

	files.push_back(new File("GameMachine", L"./data/NewObject/Machine/GameMachine.obj", L"./data/NewObject/Machine/GameMachine.dds",
		ModelData(XMFLOAT3(-12.0f, 0.2f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.1f, 0.1f, 0.1f)))); // 여기에 모델 이름, 주소 추가

	files.push_back(new File("GameMachine", L"./data/NewObject/Machine/GameMachine.obj", L"./data/NewObject/Machine/GameMachine.dds",
		ModelData(XMFLOAT3(0.0f, 0.2f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.1f, 0.1f, 0.1f)))); // 여기에 모델 이름, 주소 추가

	files.push_back(new File("GameMachine", L"./data/NewObject/Machine/GameMachine.obj", L"./data/NewObject/Machine/GameMachine.dds",
		ModelData(XMFLOAT3(12.0f, 0.2f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.1f, 0.1f, 0.1f)))); // 여기에 모델 이름, 주소 추가

	files.push_back(new File("GameMachine", L"./data/NewObject/Machine/GameMachine.obj", L"./data/NewObject/Machine/GameMachine.dds",
		ModelData(XMFLOAT3(24.0f, 0.2f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.1f, 0.1f, 0.1f)))); // 여기에 모델 이름, 주소 추가
	
	




	// Block1

	files.push_back(new File("centerLoad", L"./data/NewBuilding/Type4/building1.obj", L"./data/NewBuilding/Type4/Palette.dds", 
		ModelData(XMFLOAT3(-2.5f, 0.2f, 5.5f), XMFLOAT3(0.0f, 90.0f, 0.0f), XMFLOAT3(1.6f, 1.6f, 1.6f)))); // 여기에 모델 이름, 주소 추가

	files.push_back(new File("centerLoad", L"./data/NewBuilding/Type5/building3.obj", L"./data/NewBuilding/Type5/Texture.dds",
		ModelData(XMFLOAT3(-6.1f, 0.2f, 5.5f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.8f, 0.8f, 0.8f)))); // 여기에 모델 이름, 주소 추가

	files.push_back(new File("centerLoad", L"./data/NewBuilding/Type5/caffe1.obj", L"./data/NewBuilding/Type5/Texture.dds",
		ModelData(XMFLOAT3(-9.8f, 0.2f, 5.7f), XMFLOAT3(0.0f, 180.0f, 0.0f), XMFLOAT3(0.55f, 0.55f, 0.55f)))); // 여기에 모델 이름, 주소 추가

	files.push_back(new File("centerLoad", L"./data/NewBuilding/Type3/IceCar.obj", L"./data/NewBuilding/Type3/building.dds",
		ModelData(XMFLOAT3(-9.2f, 0.21f, -2), XMFLOAT3(0.0f, 90.0f, 0.0f), XMFLOAT3(0.17f, 0.17f, 0.17f)))); // 여기에 모델 이름, 주소 추가

	//files.push_back(new File("centerLoad", L"./data/NewBuilding/Type3/NewPark.obj", L"./data/NewBuilding/Type3/building.dds",
	//	ModelData(XMFLOAT3(-5.7, 0.21, -3.2), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.15f, 0.15f, 0.15f)))); // 여기에 모델 이름, 주소 추가

	// Block1


	// Block2


	files.push_back(new File("centerLoad", L"./data/NewBuilding/BlueHouse/house.obj", L"./data/NewBuilding/BlueHouse/Texture.dds",
		ModelData(XMFLOAT3(-1.7f, 0.2f, -2.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f)))); // 여기에 모델 이름, 주소 추가

	files.push_back(new File("centerLoad", L"./data/NewBuilding/Type2/building2.obj", L"./data/NewBuilding/Type2/Texture.dds",
		ModelData(XMFLOAT3(-1.8f, 0.2f, -3.7f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.8f, 1.8f, 1.8f)))); // 여기에 모델 이름, 주소 추가

	files.push_back(new File("centerLoad", L"./data/NewBuilding/Type2/building5.obj", L"./data/NewBuilding/Type2/Texture.dds",
		ModelData(XMFLOAT3(-1.8f, 0.2f, -5.6f), XMFLOAT3(0.0f, 180.0f, 0.0f), XMFLOAT3(1.8f, 1.8f, 1.8f)))); // 여기에 모델 이름, 주소 추가

	//// top side

	files.push_back(new File("centerLoad", L"./data/NewBuilding/Type2/building6.obj", L"./data/NewBuilding/Type2/Texture.dds",
		ModelData(XMFLOAT3(-3.3f, 0.2f, -5.9f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.8f, 1.8f, 1.8f)))); // 여기에 모델 이름, 주소 추가

	files.push_back(new File("centerLoad", L"./data/NewBuilding/Type2/building3.obj", L"./data/NewBuilding/Type2/Texture.dds",
		ModelData(XMFLOAT3(-5.2f, 0.2f, -5.9f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.8f, 1.8f, 1.8f)))); // 여기에 모델 이름, 주소 추가

	files.push_back(new File("centerLoad", L"./data/NewBuilding/Type2/building4.obj", L"./data/NewBuilding/Type2/Texture.dds",
		ModelData(XMFLOAT3(-7.1f, 0.2f, -5.9f), XMFLOAT3(0.0f, -90.0f, 0.0f), XMFLOAT3(1.8f, 1.8f, 1.8f)))); // 여기에 모델 이름, 주소 추가

	files.push_back(new File("centerLoad", L"./data/NewBuilding/Type3/building2.obj", L"./data/NewBuilding/Type3/building.dds",
		ModelData(XMFLOAT3(-9.9f, 0.2f, -5.8f), XMFLOAT3(0.0f, 90.0f, 0.0f), XMFLOAT3(1.3f, 1.3f, 1.3f)))); // 여기에 모델 이름, 주소 추가


	// Block2


	// Block3

	files.push_back(new File("centerLoad", L"./data/NewBuilding/School/School.obj", L"./data/NewBuilding/School/School.dds",
		ModelData(XMFLOAT3(3.0f, 0.26f, 6.0f), XMFLOAT3(0.0f, 90.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f)))); // 여기에 모델 이름, 주소 추가

	files.push_back(new File("centerLoad", L"./data/NewBuilding/PoliceStation/PoliceStation.obj", L"./data/NewBuilding/PoliceStation/PoliceStation.dds",
		ModelData(XMFLOAT3(7.0f, 0.2f, 6.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(3.0f, 3.0f, 3.0f)))); // 여기에 모델 이름, 주소 추가

	files.push_back(new File("centerLoad", L"./data/NewBuilding/Type7/Building1.obj", L"./data/NewBuilding/Type7/Building1.dds",
		ModelData(XMFLOAT3(10.0f, 0.26f, 2.5f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(2.0f, 2.0f, 2.0f)))); // 여기에 모델 이름, 주소 추가

	files.push_back(new File("centerLoad", L"./data/NewBuilding/Type1/building9.obj", L"./data/NewBuilding/BlueHouse/Texture.dds",
		ModelData(XMFLOAT3(10.0f, 0.2f, 4.2f), XMFLOAT3(0.0f, -90.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f)))); // 여기에 모델 이름, 주소 추가

	files.push_back(new File("centerLoad", L"./data/NewBuilding/Type1/building10.obj", L"./data/NewBuilding/BlueHouse/Texture.dds",
		ModelData(XMFLOAT3(10.0f, 0.2f, 5.6f), XMFLOAT3(0.0f, -90.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f)))); // 여기에 모델 이름, 주소 추가

	files.push_back(new File("centerLoad", L"./data/NewBuilding/Type4/building4.obj", L"./data/NewBuilding/Type4/Palette.dds",
		ModelData(XMFLOAT3(5.5f, 0.21f, 2.7f), XMFLOAT3(0.0f, 90.0f, 0.0f), XMFLOAT3(2.0f, 2.0f, 2.0f)))); // 여기에 모델 이름, 주소 추가

	// Block3

	// Block4
	
	files.push_back(new File("centerLoad", L"./data/NewBuilding/Type4/building3.obj", L"./data/NewBuilding/Type4/Palette.dds",
		ModelData(XMFLOAT3(2.45f, 0.2f, -5.3f), XMFLOAT3(0.0f, 180.0f, 0.0f), XMFLOAT3(1.6f, 1.6f, 1.6f)))); // 여기에 모델 이름, 주소 추가

	files.push_back(new File("centerLoad", L"./data/NewBuilding/LowpolyHouse/building.obj", L"./data/NewBuilding/LowpolyHouse/building.dds",
		ModelData(XMFLOAT3(5.5f, 0.2f, -5.6f), XMFLOAT3(0.0f, -90, 0.0f), XMFLOAT3(0.8f, 0.8f, 0.8f)))); // 여기에 모델 이름, 주소 추가

	files.push_back(new File("centerLoad", L"./data/NewBuilding/Hospital2/Hospital.obj", L"./data/NewBuilding/Hospital2/Hospital.dds",
		ModelData(XMFLOAT3(8.9f, 0.15f, -4.7f), XMFLOAT3(0.0f, 180.0f, 0.0f), XMFLOAT3(1.3f, 1.3f, 1.3f)))); // 여기에 모델 이름, 주소 추가

	// Block4

	
	// Block5

	files.push_back(new File("centerLoad", L"./data/NewBuilding/Type3/park1.obj", L"./data/NewBuilding/Type3/building.dds",
		ModelData(XMFLOAT3(18.0f, 0.2f, 4.05f), XMFLOAT3(0.0f, 90.0f, 0.0f), XMFLOAT3(0.11f, 0.11f, 0.11f)))); // 여기에 모델 이름, 주소 추가

	// Block5


	// Block6


	files.push_back(new File("centerLoad", L"./data/NewBuilding/Type3/parkWithoutCircle.obj", L"./data/NewBuilding/Type3/building.dds",
		ModelData(XMFLOAT3(15.5f, 0.2f, -3.9f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.2f, 0.2f, 0.2f)))); // 여기에 모델 이름, 주소 추가


	files.push_back(new File("centerLoad", L"./data/NewBuilding/Type1/building6.obj", L"./data/NewBuilding/BlueHouse/Texture.dds",
		ModelData(XMFLOAT3(22.0f, 0.2f, -6.2f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.1f, 1.1f, 1.1f)))); // 여기에 모델 이름, 주소 추가

	files.push_back(new File("centerLoad", L"./data/NewBuilding/Type1/building8.obj", L"./data/NewBuilding/BlueHouse/Texture.dds",
		ModelData(XMFLOAT3(22.5f, 0.2f, -4.6f), XMFLOAT3(0.0f, -90.0f, 0.0f), XMFLOAT3(1.1f, 1.1f, 1.0f)))); // 여기에 모델 이름, 주소 추가


	files.push_back(new File("centerLoad", L"./data/NewBuilding/Type7/Building2.obj", L"./data/NewBuilding/Type7/Building2.dds",
		ModelData(XMFLOAT3(18.8f, 0.2f, -5.0f), XMFLOAT3(0.0f, 45.0f, 0.0f), XMFLOAT3(0.5f, 0.5f, 0.5f)))); // 여기에 모델 이름, 주소 추가

	// Block6



	files.push_back(new File("centerLoad", L"./data/NewCar/BlueCar/Car7.obj", L"./data/NewCar/BlueCar/Car8.dds", 
		ModelData(XMFLOAT3(2, 0.26f, 2), XMFLOAT3(0.0f, 90.0f, 0.0f), XMFLOAT3(0.15f, 0.15f, 0.15f)))); // 여기에 모델 이름, 주소 추가

	files.push_back(new File("centerLoad", L"./data/NewCar/BlueCar/Car7.obj", L"./data/NewCar/BlueCar/Car8.dds",
		ModelData(XMFLOAT3(8.4f, 0.26f, -1.8f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.15f, 0.15f, 0.15f)))); // 여기에 모델 이름, 주소 추가


	files.push_back(new File("centerLoad", L"./data/NewCar/WhiteCar/Car12.obj", L"./data/NewCar/WhiteCar/Car12.dds", 
		ModelData(XMFLOAT3(2, 0.2f, 2.9f), XMFLOAT3(0.0f, 90.0f, 0.0f), XMFLOAT3(0.15f, 0.15f, 0.15f)))); // 여기에 모델 이름, 주소 추가

	files.push_back(new File("centerLoad", L"./data/NewCar/WhiteCar/Car12.obj", L"./data/NewCar/WhiteCar/Car12.dds", 
		ModelData(XMFLOAT3(13.7f, 0.2f, 5.6f), XMFLOAT3(0.0f, -90.0f, 0.0f), XMFLOAT3(0.15f, 0.15f, 0.15f)))); // 여기에 모델 이름, 주소 추가



	files.push_back(new File("centerLoad", L"./data/NewCar/PoliceCar/Car10.obj", L"./data/NewCar/PoliceCar/Car10.dds", 
		ModelData(XMFLOAT3(2, 0.32f, 3.8f), XMFLOAT3(0.0f, 180.0f, 0.0f), XMFLOAT3(0.2f, 0.2f, 0.2f)))); // 여기에 모델 이름, 주소 추가

	files.push_back(new File("centerLoad", L"./data/NewCar/PoliceCar/Car10.obj", L"./data/NewCar/PoliceCar/Car10.dds",
		ModelData(XMFLOAT3(13.7f, 0.32f, 6.5f), XMFLOAT3(0.0f, -180.0f, 0.0f), XMFLOAT3(0.2f, 0.2f, 0.2f)))); // 여기에 모델 이름, 주소 추가


	files.push_back(new File("centerLoad", L"./data/NewCar/OrangeCar/Car14.obj", L"./data/NewCar/OrangeCar/Car14.dds", 
		ModelData(XMFLOAT3(-2, 0.2f, 3.5f), XMFLOAT3(0.0f, 90.0f, 0.0f), XMFLOAT3(0.15f, 0.15f, 0.15f)))); // 여기에 모델 이름, 주소 추가

	files.push_back(new File("centerLoad", L"./data/NewCar/OrangeCar/Car14.obj", L"./data/NewCar/OrangeCar/Car14.dds", 
		ModelData(XMFLOAT3(-2, 0.2f, 1.7f), XMFLOAT3(0.0f, 90.0f, 0.0f), XMFLOAT3(0.15f, 0.15f, 0.15f)))); // 여기에 모델 이름, 주소 추가




	files.push_back(new File("centerLoad", L"./data/NewCar/GreenCar/Car11.obj", L"./data/NewCar/GreenCar/Car11.dds", 
		ModelData(XMFLOAT3(9.3f, 0.2f, -1.8f), XMFLOAT3(0.0f, 180.0f, 0.0f), XMFLOAT3(0.3f, 0.3f, 0.3f)))); // 여기에 모델 이름, 주소 추가

	files.push_back(new File("centerLoad", L"./data/NewCar/GreenCar/Car11.obj", L"./data/NewCar/GreenCar/Car11.dds",
		ModelData(XMFLOAT3(-2, 0.2f, 2.6f), XMFLOAT3(0.0f, 90.0f, 0.0f), XMFLOAT3(0.3f, 0.3f, 0.3f)))); // 여기에 모델 이름, 주소 추가


	files.push_back(new File("centerLoad", L"./data/NewCar/BrownCar/Car13.obj", L"./data/NewCar/BrownCar/Car13.dds", 
		ModelData(XMFLOAT3(13.7f, 0.2f, 4.7f), XMFLOAT3(0.0f, -90.0f, 0.0f), XMFLOAT3(0.3f, 0.3f, 0.3f)))); // 여기에 모델 이름, 주소 추가

	files.push_back(new File("centerLoad", L"./data/NewCar/BrownCar/Car13.obj", L"./data/NewCar/BrownCar/Car13.dds", 
		ModelData(XMFLOAT3(10.2f, 0.2f, -1.8f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.3f, 0.3f, 0.3f)))); // 여기에 모델 이름, 주소 추가


	int size = files.size();

	for (int i = 0; i < size; i++)
	{
		InteractableModelClass* tmpModel = new InteractableModelClass;
		if (!tmpModel) return false;

		// Initialize the model object.
		result = tmpModel->Initialize(m_D3D->GetDevice(), files[i]->GetModelName(), files[i]->GetFileName(), files[i]->GetFilePath(), files[i]->GetModelData());
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}

		m_InteractableModels.push_back(tmpModel);
	}

	return true;
}

bool GraphicsClass::InitAllModels(HWND hwnd)
{
	bool result;

	vector<File*> files;
	
	// Block1

	//files.push_back(new File("centerLoad", L"./data/NewBuilding/Type4/building1.obj", L"./data/NewBuilding/Type4/Palette.dds", 
	//	ModelData(XMFLOAT3(-2.5f, 0.2f, 5.5f), XMFLOAT3(0.0f, 90.0f, 0.0f), XMFLOAT3(1.6f, 1.6f, 1.6f)))); // 여기에 모델 이름, 주소 추가

	//files.push_back(new File("centerLoad", L"./data/NewBuilding/Type5/building3.obj", L"./data/NewBuilding/Type5/Texture.dds",
	//	ModelData(XMFLOAT3(-6.1f, 0.2f, 5.5f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.8f, 0.8f, 0.8f)))); // 여기에 모델 이름, 주소 추가

	//files.push_back(new File("centerLoad", L"./data/NewBuilding/Type5/caffe1.obj", L"./data/NewBuilding/Type5/Texture.dds",
	//	ModelData(XMFLOAT3(-9.8f, 0.2f, 5.7f), XMFLOAT3(0.0f, 180.0f, 0.0f), XMFLOAT3(0.55f, 0.55f, 0.55f)))); // 여기에 모델 이름, 주소 추가

	//files.push_back(new File("centerLoad", L"./data/NewBuilding/Type3/IceCar.obj", L"./data/NewBuilding/Type3/building.dds",
	//	ModelData(XMFLOAT3(-9.2, 0.21, -2), XMFLOAT3(0.0f, 90.0f, 0.0f), XMFLOAT3(0.17f, 0.17f, 0.17f)))); // 여기에 모델 이름, 주소 추가

	//files.push_back(new File("centerLoad", L"./data/NewBuilding/Type3/NewPark.obj", L"./data/NewBuilding/Type3/building.dds",
	//	ModelData(XMFLOAT3(-5.7, 0.21, -3.2), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.15f, 0.15f, 0.15f)))); // 여기에 모델 이름, 주소 추가

	// Block2

	//// left Side

	//files.push_back(new File("centerLoad", L"./data/NewBuilding/BlueHouse/house.obj", L"./data/NewBuilding/BlueHouse/Texture.dds",
	//	ModelData(XMFLOAT3(-1.7f, 0.2f, -2.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f)))); // 여기에 모델 이름, 주소 추가

	//files.push_back(new File("centerLoad", L"./data/NewBuilding/Type2/building2.obj", L"./data/NewBuilding/Type2/Texture.dds",
	//	ModelData(XMFLOAT3(-1.8f, 0.2f, -3.7f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.8f, 1.8f, 1.8f)))); // 여기에 모델 이름, 주소 추가

	//files.push_back(new File("centerLoad", L"./data/NewBuilding/Type2/building5.obj", L"./data/NewBuilding/Type2/Texture.dds",
	//	ModelData(XMFLOAT3(-1.8f, 0.2f, -5.6f), XMFLOAT3(0.0f, 180.0f, 0.0f), XMFLOAT3(1.8f, 1.8f, 1.8f)))); // 여기에 모델 이름, 주소 추가

	////// top side

	//files.push_back(new File("centerLoad", L"./data/NewBuilding/Type2/building6.obj", L"./data/NewBuilding/Type2/Texture.dds",
	//	ModelData(XMFLOAT3(-3.3f, 0.2f, -5.9f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.8f, 1.8f, 1.8f)))); // 여기에 모델 이름, 주소 추가

	//files.push_back(new File("centerLoad", L"./data/NewBuilding/Type2/building3.obj", L"./data/NewBuilding/Type2/Texture.dds",
	//	ModelData(XMFLOAT3(-5.2f, 0.2f, -5.9f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.8f, 1.8f, 1.8f)))); // 여기에 모델 이름, 주소 추가

	//files.push_back(new File("centerLoad", L"./data/NewBuilding/Type2/building4.obj", L"./data/NewBuilding/Type2/Texture.dds",
	//	ModelData(XMFLOAT3(-7.1f, 0.2f, -5.9f), XMFLOAT3(0.0f, -90.0f, 0.0f), XMFLOAT3(1.8f, 1.8f, 1.8f)))); // 여기에 모델 이름, 주소 추가

	//files.push_back(new File("centerLoad", L"./data/NewBuilding/Type3/building2.obj", L"./data/NewBuilding/Type3/building.dds",
	//	ModelData(XMFLOAT3(-9.9f, 0.2f, -5.8f), XMFLOAT3(0.0f, 90.0f, 0.0f), XMFLOAT3(1.3f, 1.3f, 1.3f)))); // 여기에 모델 이름, 주소 추가


	////// Block3

	//files.push_back(new File("centerLoad", L"./data/NewBuilding/School/School.obj", L"./data/NewBuilding/School/School.dds",
	//	ModelData(XMFLOAT3(3.0f, 0.26f, 6.0f), XMFLOAT3(0.0f, 90.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f)))); // 여기에 모델 이름, 주소 추가

	//files.push_back(new File("centerLoad", L"./data/NewBuilding/PoliceStation/PoliceStation.obj", L"./data/NewBuilding/PoliceStation/PoliceStation.dds",
	//	ModelData(XMFLOAT3(7.0f, 0.2f, 6.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(3.0f, 3.0f, 3.0f)))); // 여기에 모델 이름, 주소 추가


	//files.push_back(new File("centerLoad", L"./data/NewBuilding/Type7/Building1.obj", L"./data/NewBuilding/Type7/Building1.dds",
	//	ModelData(XMFLOAT3(10.0f, 0.26f, 2.5f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(2.0f, 2.0f, 2.0f)))); // 여기에 모델 이름, 주소 추가

	//



	//

	//files.push_back(new File("centerLoad", L"./data/NewBuilding/Type1/building9.obj", L"./data/NewBuilding/BlueHouse/Texture.dds",
	//	ModelData(XMFLOAT3(10.0f, 0.2f, 4.2f), XMFLOAT3(0.0f, -90.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f)))); // 여기에 모델 이름, 주소 추가

	//files.push_back(new File("centerLoad", L"./data/NewBuilding/Type1/building10.obj", L"./data/NewBuilding/BlueHouse/Texture.dds",
	//	ModelData(XMFLOAT3(10.0f, 0.2f, 5.6f), XMFLOAT3(0.0f, -90.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f)))); // 여기에 모델 이름, 주소 추가

	//files.push_back(new File("centerLoad", L"./data/NewBuilding/Type4/building4.obj", L"./data/NewBuilding/Type4/Palette.dds",
	//	ModelData(XMFLOAT3(5.5f, 0.21f, 2.7f), XMFLOAT3(0.0f, 90.0f, 0.0f), XMFLOAT3(2.0f, 2.0f, 2.0f)))); // 여기에 모델 이름, 주소 추가

	//// Block4

	

	//files.push_back(new File("centerLoad", L"./data/NewBuilding/Type4/building3.obj", L"./data/NewBuilding/Type4/Palette.dds",
	//	ModelData(XMFLOAT3(2.45f, 0.2f, -5.3f), XMFLOAT3(0.0f, 180.0f, 0.0f), XMFLOAT3(1.6f, 1.6f, 1.6f)))); // 여기에 모델 이름, 주소 추가

	//files.push_back(new File("centerLoad", L"./data/NewBuilding/Type2/building1.obj", L"./data/NewBuilding/Type2/Texture.dds",
	//	ModelData(XMFLOAT3(2.38f, 0.15f, -2.3f), XMFLOAT3(0.0f, 90.0f, 0.0f), XMFLOAT3(2, 2, 2)))); // 여기에 모델 이름, 주소 추가

	//files.push_back(new File("centerLoad", L"./data/NewBuilding/LowpolyHouse/building.obj", L"./data/NewBuilding/LowpolyHouse/building.dds",
	//	ModelData(XMFLOAT3(5.5f, 0.2f, -5.6f), XMFLOAT3(0.0f, -90, 0.0f), XMFLOAT3(0.8f, 0.8f, 0.8f)))); // 여기에 모델 이름, 주소 추가

	//files.push_back(new File("centerLoad", L"./data/NewBuilding/Hospital2/Hospital.obj", L"./data/NewBuilding/Hospital2/Hospital.dds",
	//	ModelData(XMFLOAT3(8.9f, 0.15f, -4.7f), XMFLOAT3(0.0f, 180.0f, 0.0f), XMFLOAT3(1.3f, 1.3f, 1.3f)))); // 여기에 모델 이름, 주소 추가


	//// Block5

	//files.push_back(new File("centerLoad", L"./data/NewBuilding/Type3/park1.obj", L"./data/NewBuilding/Type3/building.dds",
	//	ModelData(XMFLOAT3(18.0f, 0.2f, 4.05f), XMFLOAT3(0.0f, 90.0f, 0.0f), XMFLOAT3(0.11f, 0.11f, 0.11f)))); // 여기에 모델 이름, 주소 추가





	//// Block6


	files.push_back(new File("centerLoad", L"./data/NewBuilding/Type3/NoCircleobj.obj", L"./data/NewBuilding/Type3/building.dds",
		ModelData(XMFLOAT3(15.85f, 0, -5.655f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.2f, 0.2f, 0.2f)))); // 여기에 모델 이름, 주소 추가

	//files.push_back(new File("centerLoad", L"./data/NewBuilding/Type3/parkWithoutCircle.obj", L"./data/NewBuilding/Type3/building.dds",
	//	ModelData(XMFLOAT3(15.5f, 0.2f, -3.9f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.2f, 0.2f, 0.2f)))); // 여기에 모델 이름, 주소 추가



	// --> 제거

	//files.push_back(new File("centerLoad", L"./data/NewBuilding/Type3/building3.obj", L"./data/NewBuilding/Type3/building.dds",
	//	ModelData(XMFLOAT3(18.1f, -1.15f, -0.82f), XMFLOAT3(0.0f, 120.0f, 0.0f), XMFLOAT3(0.17f, 0.17f, 0.17f)))); // 여기에 모델 이름, 주소 추가
	//




	
	
	//files.push_back(new File("centerLoad", L"./data/NewBuilding/Type1/building6.obj", L"./data/NewBuilding/BlueHouse/Texture.dds",
	//	ModelData(XMFLOAT3(22.0f, 0.2f, -6.2f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.1f, 1.1f, 1.1f)))); // 여기에 모델 이름, 주소 추가

	//files.push_back(new File("centerLoad", L"./data/NewBuilding/Type1/building8.obj", L"./data/NewBuilding/BlueHouse/Texture.dds",
	//	ModelData(XMFLOAT3(22.5f, 0.2f, -4.6f), XMFLOAT3(0.0f, -90.0f, 0.0f), XMFLOAT3(1.1f, 1.1f, 1.0f)))); // 여기에 모델 이름, 주소 추가


	//files.push_back(new File("centerLoad", L"./data/NewBuilding/Type7/Building2.obj", L"./data/NewBuilding/Type7/Building2.dds",
	//	ModelData(XMFLOAT3(18.8f, 0.2f, -2.0f), XMFLOAT3(0.0f, 45.0f, 0.0f), XMFLOAT3(0.5f, 0.5f, 0.5f)))); // 여기에 모델 이름, 주소 추가
	//

	//files.push_back(new File("centerLoad", L"./data/NewBuilding/Type7/Building2.obj", L"./data/NewBuilding/Type7/Building2.dds",
	//	ModelData(XMFLOAT3(18.8f, 0.2f, -2.0f), XMFLOAT3(0.0f, 45.0f, 0.0f), XMFLOAT3(0.5f, 0.5f, 0.5f)))); // 여기에 모델 이름, 주소 추가





	files.push_back(new File("Wheel", L"./data/NewBuilding/Type3/circleWith.obj", L"./data/NewBuilding/Type3/building.dds", 
		ModelData())); // 움직이는 모델

	files.push_back(new File("Around", L"./data/NewBuilding/Type6/Around.obj", L"./data/NewBuilding/Type6/Around1.dds", 
		ModelData())); // 움직이는 모델

	files.push_back(new File("Cart1", L"./data/NewBuilding/Type3/Cart/Cart1.obj", L"./data/NewBuilding/Type3/building.dds",
		ModelData())); // 움직이는 모델

	files.push_back(new File("Cart2", L"./data/NewBuilding/Type3/Cart/Cart2.obj", L"./data/NewBuilding/Type3/building.dds",
		ModelData())); // 움직이는 모델

	files.push_back(new File("Cart3", L"./data/NewBuilding/Type3/Cart/Cart3.obj", L"./data/NewBuilding/Type3/building.dds",
		ModelData())); // 움직이는 모델

	files.push_back(new File("Cart4", L"./data/NewBuilding/Type3/Cart/Cart4.obj", L"./data/NewBuilding/Type3/building.dds",
		ModelData())); // 움직이는 모델

	files.push_back(new File("Cart5", L"./data/NewBuilding/Type3/Cart/Cart5.obj", L"./data/NewBuilding/Type3/building.dds",
		ModelData())); // 움직이는 모델

	files.push_back(new File("Cart6", L"./data/NewBuilding/Type3/Cart/Cart6.obj", L"./data/NewBuilding/Type3/building.dds",
		ModelData())); // 움직이는 모델

	int size = files.size();

	for (int i = 0; i < size; i++)
	{
		ModelClass* tmpModel = new ModelClass();
		if (!tmpModel)
		{
			return false;
		}

		// Initialize the model object.
		result = tmpModel->Initialize(m_D3D->GetDevice(), files[i]->GetModelName(), files[i]->GetFileName(), files[i]->GetFilePath(), files[i]->GetModelData());
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}

		m_Models.push_back(tmpModel);
	}

	return true;
}

bool GraphicsClass::InitAllBillboardModels(HWND hwnd)
{
	bool result;
	vector<File*> files;

	vector<ModelData> datas;
	
	datas.push_back(ModelData(XMFLOAT3(19.3f, 0.2f, 2.2f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.5f, 0.5f, 0.5f)));
	datas.push_back(ModelData(XMFLOAT3(19.9f, 0.2f, 2.2f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.5f, 0.5f, 0.5f)));
	datas.push_back(ModelData(XMFLOAT3(20.5f, 0.2f, 2.2f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.5f, 0.5f, 0.5f)));

	datas.push_back(ModelData(XMFLOAT3(19.0f, 0.2f, 2.4f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.5f, 0.5f, 0.5f)));
	datas.push_back(ModelData(XMFLOAT3(19.5f, 0.2f, 2.4f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.5f, 0.5f, 0.5f)));
	datas.push_back(ModelData(XMFLOAT3(20.5f, 0.2f, 2.4f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.5f, 0.5f, 0.5f)));

	datas.push_back(ModelData(XMFLOAT3(19.1f, 0.2f, 6.5f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.5f, 0.5f, 0.5f)));
	datas.push_back(ModelData(XMFLOAT3(19.7f, 0.2f, 6.5f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.5f, 0.5f, 0.5f)));
	datas.push_back(ModelData(XMFLOAT3(20.3f, 0.2f, 6.5f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.5f, 0.5f, 0.5f)));

	datas.push_back(ModelData(XMFLOAT3(16.1f, 0.2f, 6.5f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.5f, 0.5f, 0.5f)));
	datas.push_back(ModelData(XMFLOAT3(16.7f, 0.2f, 6.5f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.5f, 0.5f, 0.5f)));
	datas.push_back(ModelData(XMFLOAT3(17.3f, 0.2f, 6.5f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.5f, 0.5f, 0.5f)));

	datas.push_back(ModelData(XMFLOAT3(15.3f, 0.2f, 6.3f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.5f, 0.5f, 0.5f)));
	datas.push_back(ModelData(XMFLOAT3(16.1f, 0.2f, 6.3f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.5f, 0.5f, 0.5f)));
	datas.push_back(ModelData(XMFLOAT3(17.1f, 0.2f, 6.3f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.5f, 0.5f, 0.5f)));

	datas.push_back(ModelData(XMFLOAT3(16.1f, 0.2f, 2.2f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.5f, 0.5f, 0.5f)));
	datas.push_back(ModelData(XMFLOAT3(16.7f, 0.2f, 2.2f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.5f, 0.5f, 0.5f)));
	datas.push_back(ModelData(XMFLOAT3(17.3f, 0.2f, 2.2f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.5f, 0.5f, 0.5f)));

	datas.push_back(ModelData(XMFLOAT3(15.3f, 0.2f, 2.4f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.5f, 0.5f, 0.5f)));
	datas.push_back(ModelData(XMFLOAT3(16.1f, 0.2f, 2.4f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.5f, 0.5f, 0.5f)));
	datas.push_back(ModelData(XMFLOAT3(17.1f, 0.2f, 2.4f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.5f, 0.5f, 0.5f)));

	files.push_back(new File("centerLoad", L"./data/NewObject/Tree/Tree.obj", L"./data/NewObject/Tree/Tree.dds", datas)); // 여기에 모델 이름, 주소 추가
	datas.clear();

	int size = files.size();

	for (int i = 0; i < size; i++)
	{
		InstanceModelClass* tmpModel = new InstanceModelClass;
		if (!tmpModel)
		{
			return false;
		}

		// Initialize the model object.
		result = tmpModel->Initialize(m_D3D->GetDevice(), files[i]->GetFileName(), files[i]->GetFilePath(), files[i]->GetInstanceData());
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}

		m_BillboardModels.push_back(tmpModel);
	}

	return true;
}

bool GraphicsClass::InitAllEnemys(HWND hwnd)
{
	bool result;

	vector<File*> files;

	// Block1

	files.push_back(new File("Enemy", L"./data/Spino.obj", L"./data/Spino.dds",
		ModelData(XMFLOAT3(7.0f, 0.2f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f)))); // 여기에 모델 이름, 주소 추가

	files.push_back(new File("Enemy", L"./data/Spino.obj", L"./data/Spino.dds",
		ModelData(XMFLOAT3(7.0f, 0.2f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f)))); // 여기에 모델 이름, 주소 추가

	files.push_back(new File("Enemy", L"./data/Spino.obj", L"./data/Spino.dds",
		ModelData(XMFLOAT3(7.0f, 0.2f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f)))); // 여기에 모델 이름, 주소 추가

	files.push_back(new File("Enemy", L"./data/Spino.obj", L"./data/Spino.dds",
		ModelData(XMFLOAT3(7.0f, 0.2f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f)))); // 여기에 모델 이름, 주소 추가

	files.push_back(new File("Enemy", L"./data/Spino.obj", L"./data/Spino.dds",
		ModelData(XMFLOAT3(7.0f, 0.2f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f)))); // 여기에 모델 이름, 주소 추가

	files.push_back(new File("Enemy", L"./data/Spino.obj", L"./data/Spino.dds",
		ModelData(XMFLOAT3(7.0f, 0.2f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f)))); // 여기에 모델 이름, 주소 추가

	files.push_back(new File("Enemy", L"./data/Spino.obj", L"./data/Spino.dds",
		ModelData(XMFLOAT3(7.0f, 0.2f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f)))); // 여기에 모델 이름, 주소 추가

	files.push_back(new File("Enemy", L"./data/Spino.obj", L"./data/Spino.dds",
		ModelData(XMFLOAT3(7.0f, 0.2f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f)))); // 여기에 모델 이름, 주소 추가

	files.push_back(new File("Enemy", L"./data/Spino.obj", L"./data/Spino.dds",
		ModelData(XMFLOAT3(7.0f, 0.2f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f)))); // 여기에 모델 이름, 주소 추가

	files.push_back(new File("Enemy", L"./data/Spino.obj", L"./data/Spino.dds",
		ModelData(XMFLOAT3(7.0f, 0.2f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f)))); // 여기에 모델 이름, 주소 추가

	files.push_back(new File("Enemy", L"./data/Spino.obj", L"./data/Spino.dds",
		ModelData(XMFLOAT3(7.0f, 0.2f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f)))); // 여기에 모델 이름, 주소 추가

	files.push_back(new File("Enemy", L"./data/Spino.obj", L"./data/Spino.dds",
		ModelData(XMFLOAT3(7.0f, 0.2f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f)))); // 여기에 모델 이름, 주소 추가

	files.push_back(new File("Enemy", L"./data/Spino.obj", L"./data/Spino.dds",
		ModelData(XMFLOAT3(7.0f, 0.2f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f)))); // 여기에 모델 이름, 주소 추가

	files.push_back(new File("Enemy", L"./data/Spino.obj", L"./data/Spino.dds",
		ModelData(XMFLOAT3(7.0f, 0.2f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f)))); // 여기에 모델 이름, 주소 추가

	files.push_back(new File("Enemy", L"./data/Spino.obj", L"./data/Spino.dds",
		ModelData(XMFLOAT3(7.0f, 0.2f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f)))); // 여기에 모델 이름, 주소 추가

	int size = files.size();

	for (int i = 0; i < size; i++)
	{
		EnemyClass* tmpModel = new EnemyClass(m_Camera, m_D3D, hwnd);
		if (!tmpModel) return false;

		tmpModel->SetActive(false);

		// Initialize the model object.
		result = tmpModel->Initialize(m_D3D->GetDevice(), files[i]->GetModelName(), files[i]->GetFileName(), files[i]->GetFilePath(), files[i]->GetModelData());
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}

		m_InteractableModels.push_back(tmpModel);
	}

	return true;
}

bool GraphicsClass::InitAllCars(HWND hwnd)
{
	bool result;

	vector<File*> files;

	// Block1

	vector<vector<XMFLOAT3>> points;
	vector<vector<XMFLOAT3>> rotations;

	vector<XMFLOAT3> point;
	vector<XMFLOAT3> rotation;
	vector<float> speeds;

	files.push_back(new File("portal1", L"./data/NewCar/BlueTruck/Car15.obj", L"./data/NewCar/BlueTruck/Car15.dds",
		ModelData(XMFLOAT3(0.0f, 0.17f, 2.0f), XMFLOAT3(0.0f, -90.0f, 0.0f), XMFLOAT3(0.3f, 0.3f, 0.3f)))); // 여기에 모델 이름, 주소 추가


	point.push_back(XMFLOAT3(0.3f, 0.17f, 0.3f));
	point.push_back(XMFLOAT3(11.7f, 0.17f, 0.3f));
	point.push_back(XMFLOAT3(11.7f, 0.17f, 7.7f));
	point.push_back(XMFLOAT3(0.3f, 0.17f, 7.7f));

	rotation.push_back(XMFLOAT3(0, -90, 0));
	rotation.push_back(XMFLOAT3(0, -180, 0));
	rotation.push_back(XMFLOAT3(0, -270, 0));
	rotation.push_back(XMFLOAT3(0, -360, 0));

	points.push_back(point);
	rotations.push_back(rotation);
	speeds.push_back(0.3f);

	point.clear();
	rotation.clear();







	files.push_back(new File("portal1", L"./data/NewCar/BlueCar/Car7.obj", L"./data/NewCar/BlueCar/Car8.dds",
		ModelData(XMFLOAT3(0.0f, 0.17f, 2.0f), XMFLOAT3(0.0f, 90.0f, 0.0f), XMFLOAT3(0.16f, 0.16f, 0.16f)))); // 여기에 모델 이름, 주소 추가


	point.push_back(XMFLOAT3(-0.3f, 0.17f, 0.3f));
	point.push_back(XMFLOAT3(-11.7f, 0.17f, 0.3f));
	point.push_back(XMFLOAT3(-11.7f, 0.17f, 7.7f));
	point.push_back(XMFLOAT3(-0.3f, 0.17f, 7.7f));

	rotation.push_back(XMFLOAT3(0, 90, 0));
	rotation.push_back(XMFLOAT3(0, 180, 0));
	rotation.push_back(XMFLOAT3(0, 270, 0));
	rotation.push_back(XMFLOAT3(0, 360, 0));

	points.push_back(point);
	rotations.push_back(rotation);
	speeds.push_back(0.2f);

	point.clear();
	rotation.clear();





	files.push_back(new File("portal1", L"./data/NewCar/BrownCar/Car13.obj", L"./data/NewCar/BrownCar/Car13.dds",
		ModelData(XMFLOAT3(0.0f, 0.17f, 2.0f), XMFLOAT3(0.0f, 90.0f, 0.0f), XMFLOAT3(0.35f, 0.35f, 0.35f)))); // 여기에 모델 이름, 주소 추가


	point.push_back(XMFLOAT3(-0.3f, 0.17f, -0.3f));
	point.push_back(XMFLOAT3(-11.7f, 0.17f, -0.3f));
	point.push_back(XMFLOAT3(-11.7f, 0.17f, -7.7f));
	point.push_back(XMFLOAT3(-0.3f, 0.17f, -7.7f));

	rotation.push_back(XMFLOAT3(0, 90, 0));
	rotation.push_back(XMFLOAT3(0, 0, 0));
	rotation.push_back(XMFLOAT3(0, -90, 0));
	rotation.push_back(XMFLOAT3(0, 180, 0));

	points.push_back(point);
	rotations.push_back(rotation);
	speeds.push_back(0.6f);

	point.clear();
	rotation.clear();





	files.push_back(new File("portal1", L"./data/NewCar/WhiteCar/Car12.obj", L"./data/NewCar/WhiteCar/Car12.dds",
		ModelData(XMFLOAT3(0.0f, 0.17f, 2.0f), XMFLOAT3(0.0f, 270.0f, 0.0f), XMFLOAT3(0.19f, 0.19f, 0.19f)))); // 여기에 모델 이름, 주소 추가


	point.push_back(XMFLOAT3(0.3f, 0.17f, -0.3f));
	point.push_back(XMFLOAT3(11.7f, 0.17f, -0.3f));
	point.push_back(XMFLOAT3(11.7f, 0.17f, -7.7f));
	point.push_back(XMFLOAT3(0.3f, 0.17f, -7.7f));

	rotation.push_back(XMFLOAT3(0, 270, 0));
	rotation.push_back(XMFLOAT3(0, 0, 0));
	rotation.push_back(XMFLOAT3(0, 90, 0));
	rotation.push_back(XMFLOAT3(0, 180, 0));

	points.push_back(point);
	rotations.push_back(rotation);
	speeds.push_back(0.2f);

	point.clear();
	rotation.clear();



	files.push_back(new File("portal1", L"./data/NewCar/GreenCar/Car11.obj", L"./data/NewCar/GreenCar/Car11.dds",
		ModelData(XMFLOAT3(12.3f, 0.17f, 0.3f), XMFLOAT3(0.0f, 90.0f, 0.0f), XMFLOAT3(0.35f, 0.35f, 0.35f)))); // 여기에 모델 이름, 주소 추가


	point.push_back(XMFLOAT3(12.3f, 0.17f, 0.3f));
	point.push_back(XMFLOAT3(11.7f * 2 + 0.3f, 0.17f, 0.3f));
	point.push_back(XMFLOAT3(11.7f * 2 + 0.3f, 0.17f, 7.7f));
	point.push_back(XMFLOAT3(12.3f, 0.17f, 7.7f));

	rotation.push_back(XMFLOAT3(0, 90, 0));
	rotation.push_back(XMFLOAT3(0, 0, 0));
	rotation.push_back(XMFLOAT3(0, 270, 0));
	rotation.push_back(XMFLOAT3(0, 180, 0));

	points.push_back(point);
	rotations.push_back(rotation);
	speeds.push_back(0.15f);

	point.clear();
	rotation.clear();



	files.push_back(new File("portal1", L"./data/NewCar/OrangeCar/Car14.obj", L"./data/NewCar/OrangeCar/Car14.dds",
		ModelData(XMFLOAT3(12.3f, 0.17f, -0.3f), XMFLOAT3(0.0f, -90.0f, 0.0f), XMFLOAT3(0.15f, 0.15f, 0.15f)))); // 여기에 모델 이름, 주소 추가


	point.push_back(XMFLOAT3(12.3f, 0.17f, -0.3f));
	point.push_back(XMFLOAT3(11.7f * 2 + 0.3f, 0.17f, -0.3f));
	point.push_back(XMFLOAT3(11.7f * 2 + 0.3f, 0.17f, -7.7f));
	point.push_back(XMFLOAT3(12.3f, 0.17f, -7.7f));

	rotation.push_back(XMFLOAT3(0, -90, 0));
	rotation.push_back(XMFLOAT3(0, 0, 0));
	rotation.push_back(XMFLOAT3(0, 90, 0));
	rotation.push_back(XMFLOAT3(0, 180, 0));

	points.push_back(point);
	rotations.push_back(rotation);
	speeds.push_back(0.7f);

	point.clear();
	rotation.clear();


	files.push_back(new File("portal1", L"./data/NewCar/PoliceCar/Car10.obj", L"./data/NewCar/PoliceCar/Car11.dds",
		ModelData(XMFLOAT3(-12.0f, 0.27f, 0.3f), XMFLOAT3(0.0f, 270.0f, 0.0f), XMFLOAT3(0.2f, 0.2f, 0.2f)))); // 여기에 모델 이름, 주소 추가


	point.push_back(XMFLOAT3(-12.3f, 0.27f, -8.3f));
	point.push_back(XMFLOAT3(-12.3f, 0.27f, 8.3f));
	point.push_back(XMFLOAT3(12.0f * 2 + 0.3f, 0.27f, 8.3f));
	point.push_back(XMFLOAT3(12.0f * 2 + 0.3f, 0.27f, -8.3f));

	rotation.push_back(XMFLOAT3(0, 270, 0));
	rotation.push_back(XMFLOAT3(0, 0, 0));
	rotation.push_back(XMFLOAT3(0, 90, 0));
	rotation.push_back(XMFLOAT3(0, 180, 0));

	points.push_back(point);
	rotations.push_back(rotation);
	speeds.push_back(0.4f);

	point.clear();
	rotation.clear();


	int size = files.size();

	for (int i = 0; i < size; i++)
	{
		Car* tmpModel = new Car(points[i], rotations[i], speeds[i]);
		if (!tmpModel)
		{
			return false;
		}

		// Initialize the model object.
		result = tmpModel->Initialize(m_D3D->GetDevice(), files[i]->GetModelName(), files[i]->GetFileName(), files[i]->GetFilePath(), files[i]->GetModelData());
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}

		m_InteractableModels.push_back(tmpModel);
	}

	return true;
}

void GraphicsClass::ChangeTime(Day time)
{
	m_day = time;
}

bool GraphicsClass::InitAllBitmap(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	XMMATRIX baseViewMatrix;

	m_Camera->GetViewMatrix(baseViewMatrix);

	// Create the bitmap object.
	m_lineOfSight = new BitmapClass;
	if (!m_lineOfSight)
	{
		return false;
	}

	// Initialize the bitmap object.
	result = m_lineOfSight->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"./data/Aim.dds", 25, 25, baseViewMatrix);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	// Create the bitmap object.
	m_Start = new BitmapClass;
	if (!m_Start)
	{
		return false;
	}

	// Initialize the bitmap object.
	result = m_Start->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"./data/start.dds", 1290, 720, baseViewMatrix);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	return true;
}

bool GraphicsClass::InitAllInstanceModels(HWND hwnd)
{
	bool result;

	vector<File*> files;
	vector<ModelData> datas;

	// center
	datas.push_back(ModelData(XMFLOAT3(0.0f, 0.0f, 0.0f)));
	datas.push_back(ModelData(XMFLOAT3(12.0f, 0.0f, 0.0f)));

	files.push_back(new File("centerLoad", L"./data/NewNewRoad/NewRoadCenter.obj", L"./data/NewNewRoad/NewRoad.dds", datas)); // 여기에 모델 이름, 주소 추가
	datas.clear();

	datas.push_back(ModelData(XMFLOAT3(4.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 90.0f, 0.0f)));
	datas.push_back(ModelData(XMFLOAT3(8.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 90.0f, 0.0f)));

	datas.push_back(ModelData(XMFLOAT3(-4.0f, 0.0f, 8.0f), XMFLOAT3(0.0f, 90.0f, 0.0f)));
	datas.push_back(ModelData(XMFLOAT3(-8.0f, 0.0f, 8.0f), XMFLOAT3(0.0f, 90.0f, 0.0f)));

	datas.push_back(ModelData(XMFLOAT3(-4.0f, 0.0f, -8.0f), XMFLOAT3(0.0f, 90.0f, 0.0f)));
	datas.push_back(ModelData(XMFLOAT3(-8.0f, 0.0f, -8.0f), XMFLOAT3(0.0f, 90.0f, 0.0f)));

	datas.push_back(ModelData(XMFLOAT3(16.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 90.0f, 0.0f)));
	datas.push_back(ModelData(XMFLOAT3(20.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 90.0f, 0.0f)));

	datas.push_back(ModelData(XMFLOAT3(16.0f, 0.0f, -8.0f), XMFLOAT3(0.0f, 90.0f, 0.0f)));
	datas.push_back(ModelData(XMFLOAT3(20.0f, 0.0f, -8.0f), XMFLOAT3(0.0f, 90.0f, 0.0f)));

	datas.push_back(ModelData(XMFLOAT3(16.0f, 0.0f, 8.0f), XMFLOAT3(0.0f, 90.0f, 0.0f)));
	datas.push_back(ModelData(XMFLOAT3(20.0f, 0.0f, 8.0f), XMFLOAT3(0.0f, 90.0f, 0.0f)));

	datas.push_back(ModelData(XMFLOAT3(-4.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 90.0f, 0.0f)));
	datas.push_back(ModelData(XMFLOAT3(-8.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 90.0f, 0.0f)));

	datas.push_back(ModelData(XMFLOAT3(4.0f, 0.0f, 8.0f), XMFLOAT3(0.0f, 90.0f, 0.0f)));
	datas.push_back(ModelData(XMFLOAT3(8.0f, 0.0f, 8.0f), XMFLOAT3(0.0f, 90.0f, 0.0f)));

	datas.push_back(ModelData(XMFLOAT3(4.0f, 0.0f, -8.0f), XMFLOAT3(0.0f, 90.0f, 0.0f)));
	datas.push_back(ModelData(XMFLOAT3(8.0f, 0.0f, -8.0f), XMFLOAT3(0.0f, 90.0f, 0.0f)));

	datas.push_back(ModelData(XMFLOAT3(0.0f, 0.0f, 4.0f), XMFLOAT3(0.0f, 0.0f, 0.0f)));
	datas.push_back(ModelData(XMFLOAT3(-12.0f, 0.0f, 4.0f), XMFLOAT3(0.0f, 0.0f, 0.0f)));

	datas.push_back(ModelData(XMFLOAT3(12.0f, 0.0f, 4.0f), XMFLOAT3(0.0f, 0.0f, 0.0f)));
	datas.push_back(ModelData(XMFLOAT3(24.0f, 0.0f, 4.0f), XMFLOAT3(0.0f, 0.0f, 0.0f)));

	datas.push_back(ModelData(XMFLOAT3(0.0f, 0.0f, -4.0f), XMFLOAT3(0.0f, 0.0f, 0.0f)));
	datas.push_back(ModelData(XMFLOAT3(-12.0f, 0.0f, -4.0f), XMFLOAT3(0.0f, 0.0f, 0.0f)));

	datas.push_back(ModelData(XMFLOAT3(12.0f, 0.0f, -4.0f), XMFLOAT3(0.0f, 0.0f, 0.0f)));
	datas.push_back(ModelData(XMFLOAT3(24.0f, 0.0f, -4.0f), XMFLOAT3(0.0f, 0.0f, 0.0f)));

	files.push_back(new File("centerLoad", L"./data/NewNewRoad/NewRoadSide.obj", L"./data/NewNewRoad/NewRoad.dds", datas)); // 여기에 모델 이름, 주소 추가
	datas.clear();

	datas.push_back(ModelData(XMFLOAT3(24.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 90.0f, 0.0f)));
	datas.push_back(ModelData(XMFLOAT3(-12.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, -90.0f, 0.0f)));

	datas.push_back(ModelData(XMFLOAT3(0.0f, 0.0f, 8.0f), XMFLOAT3(0.0f, 0.0f, 0.0f)));
	datas.push_back(ModelData(XMFLOAT3(12.0f, 0.0f, 8.0f), XMFLOAT3(0.0f, 0.0f, 0.0f)));

	datas.push_back(ModelData(XMFLOAT3(0.0f, 0.0f, -8.0f), XMFLOAT3(0.0f, 180.0f, 0.0f)));
	datas.push_back(ModelData(XMFLOAT3(12.0f, 0.0f, -8.0f), XMFLOAT3(0.0f, 180.0f, 0.0f)));

	files.push_back(new File("centerLoad", L"./data/NewNewRoad/NewRoadEdgeConer.obj", L"./data/NewNewRoad/NewRoad.dds", datas)); // 여기에 모델 이름, 주소 추가
	datas.clear();


	datas.push_back(ModelData(XMFLOAT3(24.0f, 0.0f, -8.0f), XMFLOAT3(0.0f, 180.0f, 0.0f)));
	datas.push_back(ModelData(XMFLOAT3(-12.0f, 0.0f, -8.0f), XMFLOAT3(0.0f, -90.0f, 0.0f)));

	datas.push_back(ModelData(XMFLOAT3(-12.0f, 0.0f, 8.0f), XMFLOAT3(0.0f, 0.0f, 0.0f)));
	datas.push_back(ModelData(XMFLOAT3(24.0f, 0.0f, 8.0f), XMFLOAT3(0.0f, 90.0f, 0.0f)));

	files.push_back(new File("centerLoad", L"./data/NewNewRoad/NewRoadConer.obj", L"./data/NewNewRoad/NewRoad.dds", datas)); // 여기에 모델 이름, 주소 추가
	datas.clear();


	// floor


	datas.push_back(ModelData(XMFLOAT3(3.5f, 0.0f, 4.0f), XMFLOAT3(0.0f, 0.0f, 0.0f)));
	datas.push_back(ModelData(XMFLOAT3(8.5f, 0.0f, 4.0f), XMFLOAT3(0.0f, 0.0f, 0.0f)));

	datas.push_back(ModelData(XMFLOAT3(-3.5f, 0.0f, 4.0f), XMFLOAT3(0.0f, 0.0f, 0.0f)));
	datas.push_back(ModelData(XMFLOAT3(-8.5f, 0.0f, 4.0f), XMFLOAT3(0.0f, 0.0f, 0.0f)));

	datas.push_back(ModelData(XMFLOAT3(3.5f, 0.0f, -4.0f), XMFLOAT3(0.0f, 0.0f, 0.0f)));
	datas.push_back(ModelData(XMFLOAT3(8.5f, 0.0f, -4.0f), XMFLOAT3(0.0f, 0.0f, 0.0f)));

	datas.push_back(ModelData(XMFLOAT3(-3.5f, 0.0f, -4.0f), XMFLOAT3(0.0f, 0.0f, 0.0f)));
	datas.push_back(ModelData(XMFLOAT3(-8.5f, 0.0f, -4.0f), XMFLOAT3(0.0f, 0.0f, 0.0f)));

	datas.push_back(ModelData(XMFLOAT3(15.5f, 0.0f, 4.0f), XMFLOAT3(0.0f, 0.0f, 0.0f)));
	datas.push_back(ModelData(XMFLOAT3(20.5f, 0.0f, 4.0f), XMFLOAT3(0.0f, 0.0f, 0.0f)));

	datas.push_back(ModelData(XMFLOAT3(20.5f, 0.0f, -4.0f), XMFLOAT3(0.0f, 0.0f, 0.0f)));

	files.push_back(new File("floor14", L"./data/floor14.obj", L"./data/roadDDS.dds", datas)); // 여기에 모델 이름, 주소 추가
	datas.clear();

	datas.push_back(ModelData(XMFLOAT3(15.5f, 0.0f, -4.0f), XMFLOAT3(0.0f, 0.0f, 0.0f)));

	files.push_back(new File("floor14", L"./data/floor14.obj", L"./data/gressDDS.dds", datas)); // 여기에 모델 이름, 주소 추가
	datas.clear();

	//datas.push_back(ModelData(XMFLOAT3(-7.0f, 0.18, 3.8f), XMFLOAT3(0.0f, 0.0f, 0.0f)));
	//datas.push_back(ModelData(XMFLOAT3(-5.5f, 0.18, 3.8f), XMFLOAT3(0.0f, 0.0f, 0.0f)));

	//datas.push_back(ModelData(XMFLOAT3(-6.5f, 0.18, 2.5f), XMFLOAT3(0.0f, 0.0f, 0.0f)));
	//datas.push_back(ModelData(XMFLOAT3(-5.0f, 0.18, 2.5f), XMFLOAT3(0.0f, 0.0f, 0.0f)));

	//files.push_back(new File("centerLoad", L"./data/NewBuilding/Type3/table2.obj", L"./data/NewBuilding/Type3/building.dds", datas)); // 여기에 모델 이름, 주소 추가
	//datas.clear();


	// table1


	//datas.push_back(ModelData(XMFLOAT3(-7.0f, 0.18, 3.8f), XMFLOAT3(0.0f, 0.0f, 0.0f)));
	//datas.push_back(ModelData(XMFLOAT3(-5.5f, 0.18, 3.8f), XMFLOAT3(0.0f, 0.0f, 0.0f)));

	//datas.push_back(ModelData(XMFLOAT3(-6.5f, 0.18, 2.5f), XMFLOAT3(0.0f, 0.0f, 0.0f)));
	//datas.push_back(ModelData(XMFLOAT3(-5.0f, 0.18, 2.5f), XMFLOAT3(0.0f, 0.0f, 0.0f)));

	//files.push_back(new File("centerLoad", L"./data/NewBuilding/Type3/table2.obj", L"./data/NewBuilding/Type3/building.dds", datas)); // 여기에 모델 이름, 주소 추가
	//datas.clear();

	//// table2

	//datas.push_back(ModelData(XMFLOAT3(-10.5f, 0.18, 3.5f), XMFLOAT3(0.0f, 0.0f, 0.0f)));
	//datas.push_back(ModelData(XMFLOAT3(-10.5f, 0.18, 2.5f), XMFLOAT3(0.0f, 0.0f, 0.0f)));

	//datas.push_back(ModelData(XMFLOAT3(-9.3f, 0.18, 3.5f), XMFLOAT3(0.0f, 0.0f, 0.0f)));
	//datas.push_back(ModelData(XMFLOAT3(-9.3f, 0.18, 2.5f), XMFLOAT3(0.0f, 0.0f, 0.0f)));

	//files.push_back(new File("centerLoad", L"./data/NewBuilding/Type3/table3.obj", L"./data/NewBuilding/Type3/building.dds", datas)); // 여기에 모델 이름, 주소 추가
	//datas.clear();


	//datas.push_back(ModelData(XMFLOAT3(-10.7f, 0.18, 1.3f), XMFLOAT3(0.0f, 0.0f, 0.0f)));
	//datas.push_back(ModelData(XMFLOAT3(-10.7f, 0.18, -1.3f), XMFLOAT3(0.0f, 0.0f, 0.0f)));

	//datas.push_back(ModelData(XMFLOAT3(1.2f, 0.18, 1.3f), XMFLOAT3(0.0f, 0.0f, 0.0f)));
	//datas.push_back(ModelData(XMFLOAT3(1.2f, 0.18, -1.3f), XMFLOAT3(0.0f, 0.0f, 0.0f)));

	//datas.push_back(ModelData(XMFLOAT3(13.2f, 0.18, 1.3f), XMFLOAT3(0.0f, 0.0f, 0.0f)));

	//files.push_back(new File("centerLoad", L"./data/NewBuilding/Type3/StreetLight1.obj", L"./data/NewBuilding/Type3/building.dds", datas)); // 여기에 모델 이름, 주소 추가
	//datas.clear();

	//datas.push_back(ModelData(XMFLOAT3(5.9f, 0.18, -3.5f), XMFLOAT3(0.0f, 0.0f, 0.0f)));
	//datas.push_back(ModelData(XMFLOAT3(5.1f, 0.18, -3.5f), XMFLOAT3(0.0f, 0.0f, 0.0f)));

	//datas.push_back(ModelData(XMFLOAT3(5.9f, 0.18, -2.5f), XMFLOAT3(0.0f, 0.0f, 0.0f)));
	//datas.push_back(ModelData(XMFLOAT3(5.1f, 0.18, -2.5f), XMFLOAT3(0.0f, 0.0f, 0.0f)));

	//files.push_back(new File("centerLoad", L"./data/NewBuilding/Type3/table.obj", L"./data/NewBuilding/Type3/building.dds", datas)); // 여기에 모델 이름, 주소 추가
	//datas.clear();

	//datas.push_back(ModelData(XMFLOAT3(-7.6, 0.2, 4.2), XMFLOAT3(0.0f, 0.0f, 0.0f)));
	//datas.push_back(ModelData(XMFLOAT3(-7.76, 0.2, 4.2), XMFLOAT3(0.0f, 0.0f, 0.0f)));

	//files.push_back(new File("centerLoad", L"./data/NewObject/Caffe/Caffe.obj", L"./data/NewObject/Caffe/Caffe.dds", datas)); // 여기에 모델 이름, 주소 추가
	//datas.clear();

	// car


	datas.push_back(ModelData(XMFLOAT3(2.0f, 0.21f, 2.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.28f, 0.28f, 0.28f)));
	datas.push_back(ModelData(XMFLOAT3(2.0f, 0.21f, 2.9f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.28f, 0.28f, 0.28f)));
	datas.push_back(ModelData(XMFLOAT3(2.0f, 0.21f, 3.8f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.28f, 0.28f, 0.28f)));

	datas.push_back(ModelData(XMFLOAT3(-2, 0.21f, 1.7f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.28f, 0.28f, 0.28f)));
	datas.push_back(ModelData(XMFLOAT3(-2, 0.21f, 2.6f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.28f, 0.28f, 0.28f)));
	datas.push_back(ModelData(XMFLOAT3(-2, 0.21f, 3.5f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.28f, 0.28f, 0.28f)));

	datas.push_back(ModelData(XMFLOAT3(13.7f, 0.21f, 4.7f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.28f, 0.28f, 0.28f)));
	datas.push_back(ModelData(XMFLOAT3(13.7f, 0.21f, 5.6f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.28f, 0.28f, 0.28f)));
	datas.push_back(ModelData(XMFLOAT3(13.7f, 0.21f, 6.5f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.28f, 0.28f, 0.28f)));

	datas.push_back(ModelData(XMFLOAT3(10.2f, 0.21f, -1.8f), XMFLOAT3(0.0f, -90.0f, 0.0f), XMFLOAT3(0.28f, 0.28f, 0.28f)));
	datas.push_back(ModelData(XMFLOAT3(9.3f, 0.21f, -1.8f), XMFLOAT3(0.0f, -90.0f, 0.0f), XMFLOAT3(0.28f, 0.28f, 0.28f)));
	datas.push_back(ModelData(XMFLOAT3(8.4f, 0.21f, -1.8f), XMFLOAT3(0.0f, -90.0f, 0.0f), XMFLOAT3(0.28f, 0.28f, 0.28f)));

	files.push_back(new File("centerLoad", L"./data/NewBuilding/Type3/parkingArea.obj", L"./data/NewBuilding/Type3/ParkingArea.dds", datas)); // 여기에 모델 이름, 주소 추가
	datas.clear();

	//datas.push_back(ModelData(XMFLOAT3(2, 0.26, 2), XMFLOAT3(0.0f, 90.0f, 0.0f), XMFLOAT3(0.15f, 0.15f, 0.15f)));
	//datas.push_back(ModelData(XMFLOAT3(8.4, 0.26, -1.8), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.15f, 0.15f, 0.15f)));

	//files.push_back(new File("centerLoad", L"./data/NewCar/BlueCar/Car7.obj", L"./data/NewCar/BlueCar/Car8.dds", datas)); // 여기에 모델 이름, 주소 추가
	//datas.clear();

	//datas.push_back(ModelData(XMFLOAT3(2, 0.2f, 2.9f), XMFLOAT3(0.0f, 90.0f, 0.0f), XMFLOAT3(0.15f, 0.15f, 0.15f)));
	//datas.push_back(ModelData(XMFLOAT3(13.7f, 0.2f, 5.6f), XMFLOAT3(0.0f, -90.0f, 0.0f), XMFLOAT3(0.15f, 0.15f, 0.15f)));

	//files.push_back(new File("centerLoad", L"./data/NewCar/WhiteCar/Car12.obj", L"./data/NewCar/WhiteCar/Car12.dds", datas)); // 여기에 모델 이름, 주소 추가
	//datas.clear();

	//datas.push_back(ModelData(XMFLOAT3(2, 0.32f, 3.8f), XMFLOAT3(0.0f, 180.0f, 0.0f), XMFLOAT3(0.2f, 0.2f, 0.2f)));
	//datas.push_back(ModelData(XMFLOAT3(13.7f, 0.32f, 6.5f), XMFLOAT3(0.0f, -180.0f, 0.0f), XMFLOAT3(0.2f, 0.2f, 0.2f)));

	//files.push_back(new File("centerLoad", L"./data/NewCar/PoliceCar/Car10.obj", L"./data/NewCar/PoliceCar/Car10.dds", datas)); // 여기에 모델 이름, 주소 추가
	//datas.clear();


	//datas.push_back(ModelData(XMFLOAT3(-2, 0.2, 3.5f), XMFLOAT3(0.0f, 90.0f, 0.0f), XMFLOAT3(0.15f, 0.15f, 0.15f)));
	//datas.push_back(ModelData(XMFLOAT3(-2, 0.2, 1.7f), XMFLOAT3(0.0f, 90.0f, 0.0f), XMFLOAT3(0.15f, 0.15f, 0.15f)));

	//files.push_back(new File("centerLoad", L"./data/NewCar/OrangeCar/Car14.obj", L"./data/NewCar/OrangeCar/Car14.dds", datas)); // 여기에 모델 이름, 주소 추가
	//datas.clear();

	//datas.push_back(ModelData(XMFLOAT3(9.3, 0.2, -1.8), XMFLOAT3(0.0f, 180.0f, 0.0f), XMFLOAT3(0.3f, 0.3f, 0.3f)));
	//datas.push_back(ModelData(XMFLOAT3(-2, 0.2f, 2.6f), XMFLOAT3(0.0f, 90.0f, 0.0f), XMFLOAT3(0.3f, 0.3f, 0.3f)));

	//files.push_back(new File("centerLoad", L"./data/NewCar/GreenCar/Car11.obj", L"./data/NewCar/GreenCar/Car11.dds", datas)); // 여기에 모델 이름, 주소 추가
	//datas.clear();

	//datas.push_back(ModelData(XMFLOAT3(13.7f, 0.2, 4.7f), XMFLOAT3(0.0f, -90.0f, 0.0f), XMFLOAT3(0.3f, 0.3f, 0.3f)));
	//datas.push_back(ModelData(XMFLOAT3(10.2, 0.2, -1.8), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.3f, 0.3f, 0.3f)));

	//files.push_back(new File("centerLoad", L"./data/NewCar/BrownCar/Car13.obj", L"./data/NewCar/BrownCar/Car13.dds", datas)); // 여기에 모델 이름, 주소 추가
	//datas.clear();


	int size = files.size();

	for (int i = 0; i < size; i++)
	{
		InstanceModelClass* tmpModel = new InstanceModelClass;
		if (!tmpModel)
		{
			return false;
		}

		// Initialize the model object.
		result = tmpModel->Initialize(m_D3D->GetDevice(), files[i]->GetFileName(), files[i]->GetFilePath(), files[i]->GetInstanceData());
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}

		m_InstanceModels.push_back(tmpModel);
	}

	return true;
}

void GraphicsClass::Shutdown()
{
	int size;

	size = m_InstanceModels.size();
	for (int i = 0; i < size; i++)
	{
		m_InstanceModels[i]->Shutdown();
		delete m_InstanceModels[i];
		m_InstanceModels[i] = 0;
	}

	size = m_Models.size();
	for (int i = 0; i < size; i++)
	{
		m_Models[i]->Shutdown();
		delete m_Models[i];
		m_Models[i] = 0;
	}

	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the D3D object.
	if(m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	// Release the light object.
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

	// Release the texture shader object.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	for (unsigned int i = 0; i < m_Texts.size(); i++)
	{
		// Release the bitmap object.
		if (m_Texts[i])
		{
			m_Texts[i]->Shutdown();
			delete m_Texts[i];
			m_Texts[i] = 0;
		}
	}

	// particle shader 객체를 해제한다.
	if (m_ParticleShader)
	{
		m_ParticleShader->Shutdown();
		delete m_ParticleShader;
		m_ParticleShader = 0;
	}

	m_Skybox->CleanUp();
	
	return;
}

bool GraphicsClass::Frame(int fps, int cpu, float frameTime)
{
	bool result;

	if (m_start == true) {

		// Set the frames per second.
		result = m_Texts[0]->SetFPS(fps, 30, 30, m_D3D->GetDeviceContext()); // settext
		if (!result)
		{
			return false;
		}
		// Set the cpu usage.
		result = m_Texts[1]->SetCPU(cpu, 30, 60, m_D3D->GetDeviceContext()); // settext
		if (!result)
		{
			return false;
		}

		// Set the cpu usage.
		result = m_Texts[2]->SetText("NumOfObject", GetNumOfObjects(), 30, 90, m_D3D->GetDeviceContext()); // settext
		if (!result)
		{
			return false;
		}

		// Set the cpu usage.
		result = m_Texts[3]->SetText("NumOfPolygons", GetNumOfPolygons(), 30, 120, m_D3D->GetDeviceContext()); // settext
		if (!result)
		{
			return false;
		}

		result = m_Texts[4]->SetScreenResoluton(30, 150, m_D3D->GetDeviceContext()); // settext
		if (!result)
		{
			return false;
		}

		// Set the cpu usage.
		result = m_Texts[5]->SetText("Move: W A S D Q E | Time: 6 7 8", 30, 180, m_D3D->GetDeviceContext()); // settext
		if (!result)
		{
			return false;
		}

		// Set the cpu usage.
		result = m_Texts[6]->SetText("HP: ", gun->ReturnHP(), 1010, 60, m_D3D->GetDeviceContext()); // settext
		if (!result)
		{
			return false;
		}


		if (gun->GetNowReload())
		{
			result = m_Texts[7]->SetText("Reloading!", 1010, 90, m_D3D->GetDeviceContext()); // settext
			if (!result)
			{
				return false;
			}
		}
		else
		{
			result = m_Texts[7]->SetText("BulletCount: ", gun->ReturnBulletCount(), 1010, 90, m_D3D->GetDeviceContext()); // settext
			if (!result)
			{
				return false;
			}
		}

		result = m_Texts[8]->SetText("Score: ", gun->ReturnRealScore(), 1010, 30, m_D3D->GetDeviceContext()); // settext
		if (!result)
		{
			return false;
		}


		ResetNumOfObjects();
		ResetNumOfPolygons();

		m_rotation += 1;
		if (m_rotation == 360)
		{
			m_rotation = 0;
		}

		m_Skybox->UpdateScene(frameTime, m_Camera->GetPositionVector());



		int sizeOfList = m_InteractableModels.size();
		for (int i = 0; i < sizeOfList; i++)
		{
			m_InteractableModels[i]->Frame(frameTime);
		}

		CheckCollision();

		SpawnEnemy(frameTime);
	}

	// Render the graphics scene.
	result = Render();
	if(!result)
	{
		return false;
	}

	return true;
}

bool GraphicsClass::RenderAllCars()
{

	return true;
}

void GraphicsClass::SpawnEnemy(float time)
{
	storedSpawnDelay += time;

	if (spawnDelay < storedSpawnDelay)
	{
		int tmpRand = rand() % spawnPoints.size();

		//Enemy
		int sizeOfList = m_InteractableModels.size();
		for (int i = 0; i < sizeOfList; i++)
		{
			if (m_InteractableModels[i]->GetName() == "Enemy" && m_InteractableModels[i]->IsActive() == false)
			{
				m_InteractableModels[i]->SetActive(true);
				m_InteractableModels[i]->SetPosition(spawnPoints[tmpRand]);
				break;
			}
		}
		storedSpawnDelay = 0;
	}
}

bool GraphicsClass::RenderAllInteractableModels()
{
	bool result;
	XMMATRIX viewMatrix, projectionMatrix, worldMatrix;


	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	int size = m_InteractableModels.size();

	for (int i = 0; i < size; i++)
	{
		if (m_InteractableModels[i]->IsActive())
		{
			if (m_InteractableModels[i]->GetName() == "Player")
			{
				m_InteractableModels[i]->Render(m_D3D->GetDeviceContext());

				m_D3D->GetWorldMatrix(worldMatrix);

				ModelData data = m_InteractableModels[i]->GetModelData();
				XMFLOAT3 rotation = m_InteractableModels[i]->SetAngleToEuler(data.GetRotation());

				float yaw = m_Camera->ReturnCamYaw();
				float pitch = m_Camera->ReturnCamPitch();

				worldMatrix *= XMMatrixRotationX(pitch);
				worldMatrix *= XMMatrixRotationY(yaw);
				worldMatrix *= XMMatrixRotationZ(rotation.z);
				worldMatrix *= XMMatrixScaling(data.GetScale().x, data.GetScale().y, data.GetScale().z);

				worldMatrix *= XMMatrixTranslation(m_Camera->GetPosition().x + data.GetOffsetPos().x, m_Camera->GetPosition().y + data.GetOffsetPos().y, m_Camera->GetPosition().z + data.GetOffsetPos().z);

				// Render the model using the light shader.
				result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_InteractableModels[i]->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
					m_InteractableModels[i]->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower(),
					m_Light->GetPointDiffuseColor(), m_Light->GetPosition(), m_Light->GetAmbientState(), m_Light->GetDiffuseState(), m_Light->GetSpecularState());
				if (!result)
				{
					return false;
				}

				continue;
			}

			m_InteractableModels[i]->Render(m_D3D->GetDeviceContext());

			m_D3D->GetWorldMatrix(worldMatrix);

			ModelData data = m_InteractableModels[i]->GetModelData();
			XMFLOAT3 rotation = m_InteractableModels[i]->SetAngleToEuler(data.GetRotation());

			worldMatrix *= XMMatrixRotationX(rotation.x);
			worldMatrix *= XMMatrixRotationY(rotation.y);
			worldMatrix *= XMMatrixRotationZ(rotation.z);
			worldMatrix *= XMMatrixScaling(data.GetScale().x, data.GetScale().y, data.GetScale().z);

			worldMatrix *= XMMatrixTranslation(data.GetPos().x, data.GetPos().y, data.GetPos().z);

			// Render the model using the light shader.
			result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_InteractableModels[i]->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
				m_InteractableModels[i]->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower(),
				m_Light->GetPointDiffuseColor(), m_Light->GetPosition(), m_Light->GetAmbientState(), m_Light->GetDiffuseState(), m_Light->GetSpecularState());
			if (!result)
			{
				return false;
			}
		}
	}

	for (int i = 0; i < size; i++)
	{
		m_InteractableModels[i]->RenderParticle(m_ParticleShader);
	}

	return true;
}

bool GraphicsClass::RenderAllModels()
{
	bool result;
	XMMATRIX viewMatrix, projectionMatrix, worldMatrix;


	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	int size = m_Models.size();

	for (int i = 0; i < size; i++)
	{
		m_Models[i]->Render(m_D3D->GetDeviceContext());

		m_D3D->GetWorldMatrix(worldMatrix);

		ModelData data = m_Models[i]->GetModelData();

		


		if (m_Models[i]->GetName() == "Wheel")
		{
			m_D3D->GetWorldMatrix(worldMatrix);
			worldMatrix *= XMMatrixRotationZ(m_rotation * euler);
			worldMatrix *= XMMatrixScaling(0.2f, 0.2f, 0.2f);
			worldMatrix *= XMMatrixTranslation(15.87f, 1.9f, -5.655f);



			result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Models[i]->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
				m_Models[i]->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower(),
				m_Light->GetPointDiffuseColor(), m_Light->GetPosition(), m_Light->GetAmbientState(), m_Light->GetDiffuseState(), m_Light->GetSpecularState());
			if (!result)
			{
				return false;
			}

			continue;
		}
		if (m_Models[i]->GetName() == "Around")
		{
			m_D3D->GetWorldMatrix(worldMatrix);
			worldMatrix *= XMMatrixRotationY(m_rotation * euler);
			worldMatrix *= XMMatrixScaling(1.6f, 1.6f, 1.6f);
			worldMatrix *= XMMatrixTranslation(16.5f, 0.3f, 5.5f);

			result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Models[i]->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
				m_Models[i]->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower(),
				m_Light->GetPointDiffuseColor(), m_Light->GetPosition(), m_Light->GetAmbientState(), m_Light->GetDiffuseState(), m_Light->GetSpecularState());
			if (!result)
			{
				return false;
			}

			continue;
		}

		float offset = 1.3f;

		float xPos = (offset / 2) * (float)sqrt(3);
		float yPos = offset / 2;

		// y --> offset / 2
		// x --> (offset / 2) * sqrt(3)

		if (m_Models[i]->GetName() == "Cart1")
		{
			m_D3D->GetWorldMatrix(worldMatrix);
			worldMatrix *= XMMatrixRotationZ(-m_rotation * euler);           // right
			worldMatrix *= XMMatrixScaling(0.2f, 0.2f, 0.2f);
			worldMatrix *= XMMatrixTranslation(0, offset, 0.0f);
			worldMatrix *= XMMatrixRotationZ(m_rotation * euler);
			worldMatrix *= XMMatrixTranslation(15.87f, 1.9f, -5.655f);

			result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Models[i]->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
				m_Models[i]->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower(),
				m_Light->GetPointDiffuseColor(), m_Light->GetPosition(), m_Light->GetAmbientState(), m_Light->GetDiffuseState(), m_Light->GetSpecularState());
			if (!result)
			{
				return false;
			}

			continue;
		}
		if (m_Models[i]->GetName() == "Cart2")
		{
			m_D3D->GetWorldMatrix(worldMatrix);
			worldMatrix *= XMMatrixRotationZ(-m_rotation * euler);           // left
			worldMatrix *= XMMatrixScaling(0.2f, 0.2f, 0.2f);
			worldMatrix *= XMMatrixTranslation(xPos, yPos, 0.0f);
			worldMatrix *= XMMatrixRotationZ(m_rotation * euler);
			worldMatrix *= XMMatrixTranslation(15.87f, 1.9f, -5.655f);

			result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Models[i]->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
				m_Models[i]->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower(),
				m_Light->GetPointDiffuseColor(), m_Light->GetPosition(), m_Light->GetAmbientState(), m_Light->GetDiffuseState(), m_Light->GetSpecularState());
			if (!result)
			{
				return false;
			}

			continue;
		}
		if (m_Models[i]->GetName() == "Cart3")
		{
			m_D3D->GetWorldMatrix(worldMatrix);
			worldMatrix *= XMMatrixRotationZ(-m_rotation * euler);           // top
			worldMatrix *= XMMatrixScaling(0.2f, 0.2f, 0.2f);
			worldMatrix *= XMMatrixTranslation(xPos, -yPos, 0.0f);
			worldMatrix *= XMMatrixRotationZ(m_rotation * euler);
			worldMatrix *= XMMatrixTranslation(15.87f, 1.9f, -5.655f);

			result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Models[i]->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
				m_Models[i]->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower(),
				m_Light->GetPointDiffuseColor(), m_Light->GetPosition(), m_Light->GetAmbientState(), m_Light->GetDiffuseState(), m_Light->GetSpecularState());
			if (!result)
			{
				return false;
			}

			continue;
		}
		if (m_Models[i]->GetName() == "Cart4")
		{
			m_D3D->GetWorldMatrix(worldMatrix);
			worldMatrix *= XMMatrixRotationZ(-m_rotation * euler);           // down
			worldMatrix *= XMMatrixScaling(0.2f, 0.2f, 0.2f);
			worldMatrix *= XMMatrixTranslation(0.0f, -offset, 0.0f);
			worldMatrix *= XMMatrixRotationZ(m_rotation * euler);
			worldMatrix *= XMMatrixTranslation(15.87f, 1.9f, -5.655f);

			result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Models[i]->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
				m_Models[i]->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower(),
				m_Light->GetPointDiffuseColor(), m_Light->GetPosition(), m_Light->GetAmbientState(), m_Light->GetDiffuseState(), m_Light->GetSpecularState());
			if (!result)
			{
				return false;
			}

			continue;
		}
		if (m_Models[i]->GetName() == "Cart5")
		{
			m_D3D->GetWorldMatrix(worldMatrix);
			worldMatrix *= XMMatrixRotationZ(-m_rotation * euler);           // right down
			worldMatrix *= XMMatrixScaling(0.2f, 0.2f, 0.2f);
			worldMatrix *= XMMatrixTranslation(-xPos, -yPos, 0.0f);
			worldMatrix *= XMMatrixRotationZ(m_rotation * euler);
			worldMatrix *= XMMatrixTranslation(15.87f, 1.9f, -5.655f);

			result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Models[i]->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
				m_Models[i]->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower(),
				m_Light->GetPointDiffuseColor(), m_Light->GetPosition(), m_Light->GetAmbientState(), m_Light->GetDiffuseState(), m_Light->GetSpecularState());
			if (!result)
			{
				return false;
			}

			continue;
		}
		if (m_Models[i]->GetName() == "Cart6")
		{
			m_D3D->GetWorldMatrix(worldMatrix);
			worldMatrix *= XMMatrixRotationZ(-m_rotation * euler);           // left down
			worldMatrix *= XMMatrixScaling(0.2f, 0.2f, 0.2f);
			worldMatrix *= XMMatrixTranslation(-xPos, yPos, 0.0f);
			worldMatrix *= XMMatrixRotationZ(m_rotation * euler);
			worldMatrix *= XMMatrixTranslation(15.87f, 1.9f, -5.655f);

			result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Models[i]->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
				m_Models[i]->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower(),
				m_Light->GetPointDiffuseColor(), m_Light->GetPosition(), m_Light->GetAmbientState(), m_Light->GetDiffuseState(), m_Light->GetSpecularState());
			if (!result)
			{
				return false;
			}

			continue;
		}



		
		XMFLOAT3 rotation = m_Models[i]->SetAngleToEuler(data.GetRotation());

		worldMatrix *= XMMatrixRotationX(rotation.x);
		worldMatrix *= XMMatrixRotationY(rotation.y);
		worldMatrix *= XMMatrixRotationZ(rotation.z);
		worldMatrix *= XMMatrixScaling(data.GetScale().x, data.GetScale().y, data.GetScale().z);

		worldMatrix *= XMMatrixTranslation(data.GetPos().x, data.GetPos().y, data.GetPos().z);

		// Render the model using the light shader.
		result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Models[i]->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
			m_Models[i]->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower(),
			m_Light->GetPointDiffuseColor(), m_Light->GetPosition(), m_Light->GetAmbientState(), m_Light->GetDiffuseState(), m_Light->GetSpecularState());
		if (!result)
		{
			return false;
		}
	}

	return true;
}

bool GraphicsClass::RenderAllInstanceModels()
{
	bool result;
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	int size = m_InstanceModels.size();

	for (int i = 0; i < size; i++)
	{
		// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
		m_InstanceModels[i]->Render(m_D3D->GetDeviceContext());

		// Render the model using the light shader.
		result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_InstanceModels[i]->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
			m_InstanceModels[i]->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower(),
			m_Light->GetPointDiffuseColor(), m_Light->GetPosition(), m_Light->GetAmbientState(), m_Light->GetDiffuseState(), m_Light->GetSpecularState(), m_InstanceModels[i]->GetInstanceCount(), false);
		if (!result)
		{
			return false;
		}
	}

	return true;
}

bool GraphicsClass::RenderAllBillboardModels()
{
	bool result;
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	XMFLOAT3 cameraPosition;

	cameraPosition = m_Camera->GetPosition();
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);

	m_D3D->TurnOnAlphaBlending();

	int size = m_BillboardModels.size();

	for (int i = 0; i < size; i++)
	{
		// model pos 받아오기
		ModelData modelData = m_BillboardModels[i]->GetModelData();
		XMFLOAT3 modelPosition = modelData.GetPos();

		m_BillboardModels[i]->Render(m_D3D->GetDeviceContext());

		// Render the model using the light shader.
		result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_BillboardModels[i]->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
			m_BillboardModels[i]->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower(),
			m_Light->GetPointDiffuseColor(), m_Light->GetPosition(), m_Light->GetAmbientState(), m_Light->GetDiffuseState(), m_Light->GetSpecularState(), m_BillboardModels[i]->GetInstanceCount(), true);
		if (!result)
		{
			return false;
		}
	}

	m_D3D->TurnOffAlphaBlending();

	return true;
}

bool GraphicsClass::RenderUI()
{

	bool result;
	XMMATRIX worldMatrix, orthoMatrix;

	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	// Turn off the Z buffer to begin all 2D rendering.
	m_D3D->TurnZBufferOff();

	m_D3D->TurnOnAlphaBlending();

	// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
	result = m_lineOfSight->Render(m_D3D->GetDeviceContext(), (int)(1280 / 2 - 12.5f), (int)(720 / 2 - 12.5f));
	if (!result)
	{
		return false;
	}

	// Render the bitmap with the texture shader.
	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_lineOfSight->GetIndexCount(), worldMatrix, m_lineOfSight->GetViewMatrix(), orthoMatrix, m_lineOfSight->GetTexture());
	if (!result)
	{
		return false;
	}
	

	m_D3D->TurnOffAlphaBlending();

	//if (m_start == false) return true;

	if (m_start == false)
	{
		// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
		result = m_Start->Render(m_D3D->GetDeviceContext(), 0, 0);
		if (!result)
		{
			return false;
		}

		// Render the bitmap with the texture shader.
		result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Start->GetIndexCount(), worldMatrix, m_Start->GetViewMatrix(), orthoMatrix, m_Start->GetTexture());
		if (!result)
		{
			return false;
		}
	}

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_D3D->TurnZBufferOn();

	return true;
}

bool GraphicsClass::RenderBackground()
{
	XMMATRIX worldMatrix, orthoMatrix;

	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	// Turn off the Z buffer to begin all 2D rendering.
	m_D3D->TurnZBufferOff();


	if (m_day == Day::Afternoon)
	{
		vector<XMFLOAT4> pointDiffuseColors;
		vector<XMFLOAT4> pointDiffusePositions;

		float intencity = 1.0f;

		pointDiffuseColors.push_back(XMFLOAT4(intencity, intencity, intencity, 1.0f));
		pointDiffuseColors.push_back(XMFLOAT4(intencity * 0.2f, intencity * 0.2f, intencity, 1.0f));
		pointDiffuseColors.push_back(XMFLOAT4(intencity, intencity, intencity * 0.2f, 1.0f));
		pointDiffuseColors.push_back(XMFLOAT4(intencity, 0, 0, 1.0f));
		pointDiffuseColors.push_back(XMFLOAT4(intencity, 0, 0, 1.0f));
		pointDiffuseColors.push_back(XMFLOAT4(intencity, 0, 0, 1.0f));

		float height = 2.3f;
		float height1 = 0.7f;

		pointDiffusePositions.push_back(XMFLOAT4(-10.7f, height, 0.0f, 0.0f));
		pointDiffusePositions.push_back(XMFLOAT4(13.2f, height, 0.0f, 0.0f));
		pointDiffusePositions.push_back(XMFLOAT4(18.0f, height1, 4.05f, 0.0f));

		pointDiffusePositions.push_back(XMFLOAT4(1.2f, height, 0.0f, 0.0f));
		pointDiffusePositions.push_back(XMFLOAT4(1.2f, height, 7, 0.0f));
		pointDiffusePositions.push_back(XMFLOAT4(13.2f, height, 0, 0.0f));
		pointDiffusePositions.push_back(XMFLOAT4(13.2f, height, 8, 0.0f));

		m_Light->SetPointDiffuseColor(pointDiffuseColors);
		m_Light->SetPosition(pointDiffusePositions);
		m_Light->SetSpecularPower(64);
	}
	else if (m_day == Day::SunSet)
	{
		vector<XMFLOAT4> pointDiffuseColors;
		vector<XMFLOAT4> pointDiffusePositions;

		float intencity = 0.4f;

		pointDiffuseColors.push_back(XMFLOAT4(intencity * 1.8f, intencity, intencity * 0.6f, 1.0f));
		pointDiffuseColors.push_back(XMFLOAT4(intencity, intencity * 1.8f, intencity, 1.0f));
		pointDiffuseColors.push_back(XMFLOAT4(intencity * 1.8f, intencity, intencity * 0.6f, 1.0f));
		pointDiffuseColors.push_back(XMFLOAT4(intencity, 0, 0, 1.0f));
		pointDiffuseColors.push_back(XMFLOAT4(intencity, 0, 0, 1.0f));
		pointDiffuseColors.push_back(XMFLOAT4(intencity, 0, 0, 1.0f));

		float height = 2.3f;
		float height1 = 0.7f;

		pointDiffusePositions.push_back(XMFLOAT4(-10.7f, height, 0.0f, 0.0f));
		pointDiffusePositions.push_back(XMFLOAT4(13.2f, height, 0.0f, 0.0f));
		pointDiffusePositions.push_back(XMFLOAT4(18.0f, height1, 4.05f, 0.0f));

		pointDiffusePositions.push_back(XMFLOAT4(1.2f, height, 0.0f, 0.0f));
		pointDiffusePositions.push_back(XMFLOAT4(1.2f, height, 7, 0.0f));
		pointDiffusePositions.push_back(XMFLOAT4(13.2f, height, 0, 0.0f));
		pointDiffusePositions.push_back(XMFLOAT4(13.2f, height, 8, 0.0f));

		m_Light->SetPointDiffuseColor(pointDiffuseColors);
		m_Light->SetPosition(pointDiffusePositions);
		m_Light->SetSpecularPower(64);
	}
	else if (m_day == Day::Night)
	{
		vector<XMFLOAT4> pointDiffuseColors;
		vector<XMFLOAT4> pointDiffusePositions;

		float intencity = 0.7f;

		pointDiffuseColors.push_back(XMFLOAT4(intencity, intencity, intencity * 0.8f, 1.0f));
		pointDiffuseColors.push_back(XMFLOAT4(intencity, intencity, intencity * 0.8f, 1.0f));
		pointDiffuseColors.push_back(XMFLOAT4(0, 0, intencity * 2, 1.0f));
		pointDiffuseColors.push_back(XMFLOAT4(intencity, 0, 0, 1.0f));
		pointDiffuseColors.push_back(XMFLOAT4(intencity, 0, 0, 1.0f));
		pointDiffuseColors.push_back(XMFLOAT4(intencity, 0, 0, 1.0f));

		float height = 2.3f;
		float height1 = 0.7f;

		pointDiffusePositions.push_back(XMFLOAT4(-10.7f, height, 0.0f, 0.0f));
		pointDiffusePositions.push_back(XMFLOAT4(13.2f, height, 0.0f, 0.0f));
		pointDiffusePositions.push_back(XMFLOAT4(18.0f, height1, 4.05f, 0.0f));

		pointDiffusePositions.push_back(XMFLOAT4(1.2f, height, 0.0f, 0.0f));
		pointDiffusePositions.push_back(XMFLOAT4(1.2f, height, 7, 0.0f));
		pointDiffusePositions.push_back(XMFLOAT4(13.2f, height, 0, 0.0f));
		pointDiffusePositions.push_back(XMFLOAT4(13.2f, height, 8, 0.0f));

		m_Light->SetPointDiffuseColor(pointDiffuseColors);
		m_Light->SetPosition(pointDiffusePositions);
		m_Light->SetSpecularPower(128);
	}



	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_D3D->TurnZBufferOn();

	return true;
}

int GraphicsClass::GetNumOfObjects()
{
	int num = 0;

	int sizeOfList = m_Texts.size();
	for (int i = 0; i < sizeOfList; i++)
	{
		num += m_Texts[i]->GetFontShader()->GetNumOfObject();
	}
	num += m_LightShader->GetNumOfObject();
	num += m_TextureShader->GetNumOfObject();

	return num;
}

void GraphicsClass::ResetNumOfObjects()
{
	int num = 0;

	int sizeOfList = m_Texts.size();
	for (int i = 0; i < sizeOfList; i++)
	{
		m_Texts[i]->GetFontShader()->SetNumOfObject(0);
	}
	m_LightShader->SetNumOfObject(0);
	m_TextureShader->SetNumOfObject(0);
}

int GraphicsClass::GetNumOfPolygons()
{
	int num = 0;

	int sizeOfList = m_Texts.size();
	for (int i = 0; i < sizeOfList; i++)
	{
		num += m_Texts[i]->GetFontShader()->GetNumOfPolygon();
	}
	num += m_LightShader->GetNumOfPolygon();
	num += m_TextureShader->GetNumOfPolygon();

	return num;
}

void GraphicsClass::ResetNumOfPolygons()
{
	int num = 0;

	int sizeOfList = m_Texts.size();
	for (int i = 0; i < sizeOfList; i++)
	{
		m_Texts[i]->GetFontShader()->SetNumOfPolygon(0);
	}
	m_LightShader->SetNumOfPolygon(0);
	m_TextureShader->SetNumOfPolygon(0);
}

void GraphicsClass::ChangeLightSetting()
{
	vector<XMFLOAT4> pointDiffuseColors;
	vector<XMFLOAT4> pointDiffusePositions;

	float intencity = 1.0f;
	float height = 1.0f;

	if (m_day == Day::Afternoon)
	{
		pointDiffuseColors.push_back(XMFLOAT4(intencity, intencity, intencity, 1.0f));
		pointDiffuseColors.push_back(XMFLOAT4(intencity * 0.2f, intencity * 2, intencity * 0.2f, 1.0f));
		pointDiffuseColors.push_back(XMFLOAT4(intencity * 2, intencity * 2, intencity * 0.1f, 1.0f));

		pointDiffusePositions.push_back(XMFLOAT4(-10.7f, height, 0.0f, 0.0f));
		pointDiffusePositions.push_back(XMFLOAT4(13.2f, height, 0.0f, 0.0f));
		pointDiffusePositions.push_back(XMFLOAT4(18.0f, height * 2, 4.05f, 0.0f));

		m_Light->SetSpecularPower(64);
	}
	else if (m_day == Day::SunSet)
	{
		pointDiffuseColors.push_back(XMFLOAT4(intencity * 1.8f, intencity, intencity * 0.6f, 1.0f));
		pointDiffuseColors.push_back(XMFLOAT4(intencity * 0.2f, intencity * 3.0f, intencity * 0.2f, 1.0f));
		pointDiffuseColors.push_back(XMFLOAT4(intencity * 3, 0, 0, 1.0f));

		pointDiffusePositions.push_back(XMFLOAT4(-10.7f, height, 0.0f, 0.0f));
		pointDiffusePositions.push_back(XMFLOAT4(13.2f, height, 0.0f, 0.0f));
		pointDiffusePositions.push_back(XMFLOAT4(18.0f, height * 2, 4.05f, 0.0f));

		m_Light->SetSpecularPower(128);
	}
	else if (m_day == Day::Night)
	{
		pointDiffuseColors.push_back(XMFLOAT4(intencity * 3, intencity * 3, intencity * 0.2f, 1.0f));
		pointDiffuseColors.push_back(XMFLOAT4(intencity, intencity, intencity * 0.8f, 1.0f));
		pointDiffuseColors.push_back(XMFLOAT4(0, 0, intencity * 2, 1.0f));

		pointDiffusePositions.push_back(XMFLOAT4(-10.7f, height * 1.6f, 0.0f, 0.0f));
		pointDiffusePositions.push_back(XMFLOAT4(13.2f, height, 0.0f, 0.0f));
		pointDiffusePositions.push_back(XMFLOAT4(18.0f, height * 2, 4.05f, 0.0f));

		m_Light->SetSpecularPower(128);
	}

	float Directionintencity = 0.1f;

	// Initialize the light object.
	m_Light->SetAmbientColor(Directionintencity, Directionintencity, Directionintencity, 1.0f);
	m_Light->SetDiffuseColor(Directionintencity, Directionintencity, Directionintencity, 1.0f);
	m_Light->SetDirection(1.0f, -1.4f, 1.0f);
	m_Light->SetSpecularColor(Directionintencity, Directionintencity, Directionintencity, 1.0f);
	m_Light->SetSpecularPower(128.0f);

	m_Light->SetPointDiffuseColor(pointDiffuseColors);
	m_Light->SetPosition(pointDiffusePositions);
}

bool GraphicsClass::GetStart()
{
	return m_start;
}

void GraphicsClass::SetStart(bool start)
{
	m_start = start;
}

vector<InteractableModelClass*>& GraphicsClass::ReturnInteractableModel()
{
	// TODO: 여기에 return 문을 삽입합니다.

	return m_InteractableModels;
}

void GraphicsClass::CheckCollision()
{
	int sizeOfList = m_InteractableModels.size();

	for (int i = 0; i < sizeOfList; i++)
	{
		XMVECTOR itmpMinVertex = m_InteractableModels[i]->ReturnBoundingBoxMinVertex();
		XMVECTOR itmpMaxVertex = m_InteractableModels[i]->ReturnBoundingBoxMaxVertex();

		//float iBoundingSphere = m_InteractableModels[i]->ReturnBoundingSphere();
		//XMVECTOR iCenterOffset = m_InteractableModels[i]->ReturnCenterOffset();
		//XMMATRIX iWorld = m_InteractableModels[i]->ReturnModelMatrix();

		bool canHit = false;

		for (int j = 0; j < sizeOfList; j++)
		{
			if (i == j) continue;
			if (m_InteractableModels[i]->IsActive() == false || m_InteractableModels[j]->IsActive() == false) continue;

			XMVECTOR jtmpMinVertex = m_InteractableModels[j]->ReturnBoundingBoxMinVertex();
			XMVECTOR jtmpMaxVertex = m_InteractableModels[j]->ReturnBoundingBoxMaxVertex();

			//float jBoundingSphere = m_InteractableModels[j]->ReturnBoundingSphere();
			//XMVECTOR jCenterOffset = m_InteractableModels[j]->ReturnCenterOffset();
			//XMMATRIX jWorld = m_InteractableModels[j]->ReturnModelMatrix();

			if (m_InteractableModels[i]->BoundingBoxCollision(itmpMinVertex, itmpMaxVertex, jtmpMinVertex, jtmpMaxVertex))
			{
				canHit = true;
				m_InteractableModels[i]->Hit(m_InteractableModels[i], m_InteractableModels[j]);
				break;
			}
		}

		if (canHit == false)
		{
			m_InteractableModels[i]->NotHit();
		}
	}
}

bool GraphicsClass::Render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	bool result;
	
	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	// Rotate the world matrix by the rotation value so that the triangle will spin.
	//worldMatrix = XMMatrixRotationY(rotation);

	if (m_start == true)
	{
		m_Skybox->DrawScene(m_D3D->GetDeviceContext(), m_Camera->ReturnViewMatrix(), m_Camera->ReturnProjectionMatrix());


		RenderBackground(); /// ////////////////////////////////////////////////////////////////////////////////////// 배경 넣기

	/////////////////////////////////////////////////////////////////////////////////// Render Portal
	
		result = RenderAllCars();
		if (!result)
		{
			return false;
		}

		result = RenderAllInstanceModels();
		if (!result)
		{
			return false;
		}



		result = RenderAllModels();
		if (!result)
		{
			return false;
		}

		result = RenderAllBillboardModels();
		if (!result)
		{
			return false;
		}


		result = RenderAllInteractableModels();
		if (!result)
		{
			return false;
		}
	}



	m_D3D->GetWorldMatrix(worldMatrix);

	//// Turn on the alpha blending before rendering the text.
	m_D3D->TurnZBufferOff();
	m_D3D->TurnOnAlphaBlending();

	if (m_start == true)
	{

		int sizeOfList = m_Texts.size();

		for (int i = 0; i < sizeOfList; i++)
		{
			result = m_Texts[i]->Render(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);
			if (!result)
			{
				return false;
			}
		}
	}

	RenderUI(); /// ////////////////////////////////////////////////////////////////////////////////////// UI 넣기

	//// Turn off alpha blending after rendering the text.
	m_D3D->TurnOffAlphaBlending();

	m_D3D->TurnZBufferOn();

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}

float GraphicsClass::GetEulerAngle(float angle)
{
	return angle * euler;
}