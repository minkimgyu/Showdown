#include "enemyclass.h"

EnemyClass::EnemyClass(CameraClass* _camera, D3DClass* _d3d, HWND hwnd)
{
	m_hwnd = hwnd;

	camera = _camera;
	d3d = _d3d;

	SetMove(true);
	
}

EnemyClass::~EnemyClass()
{
}

void EnemyClass::Hit(InteractableModelClass* myModel, InteractableModelClass* otherModel)
{
	InteractableModelClass::Hit(myModel, otherModel);

	if (otherModel->GetName() == "Player")
	{
		otherModel->GetDamage();
		active = false;
		m_Sound->Play();
		m_ParticleSystem->SetSpawn(true);

		return;
	}

	XMVECTOR dirVector = myModel->GetModelData().ReturnPositionVector() - otherModel->GetModelData().ReturnPositionVector();
	XMVECTOR nomalDir = XMVector3Normalize(dirVector);
	hitNomalVector = nomalDir;

	pushVector = nomalDir * followSpeed;
}

void EnemyClass::NotHit()
{
	InteractableModelClass::NotHit();
	pushVector = XMVECTOR();
}


void EnemyClass::Pick(int* score)
{
	if (active == false) return;

	*score += 50;
	m_Sound->Play();

	active = false;
	m_ParticleSystem->SetSpawn(true);
}

bool EnemyClass::Initialize(ID3D11Device* device, string name, const WCHAR* v1, const WCHAR* v2, ModelData v3)
{
	bool result;

	InteractableModelClass::Initialize(device, name, v1, v2, v3);

	// 파티클 시스템 객체를 만듭니다.
	m_ParticleSystem = new ParticleSystemClass;
	if (!m_ParticleSystem)
	{
		return false;
	}

	// Create the sound object.
	m_Sound = new SoundClass;
	if (!m_Sound)
	{
		return false;
	}

	// Initialize the sound object.
	result = m_Sound->Initialize(m_hwnd, "./data/Die.wav");
	if (!result)
	{
		MessageBox(m_hwnd, L"Could not initialize Direct Sound.", L"Error", MB_OK);
		return false;
	}

	// 파티클 시스템 객체를 초기화합니다.
	if (!m_ParticleSystem->Initialize(device, L"./data/star.dds"))
	{
		return false;
	}

	return true;
}

void EnemyClass::Shutdown()
{
	ModelClass::Shutdown();

	// particle shader 객체를 해제한다.
	if (m_ParticleSystem)
	{
		m_ParticleSystem->Shutdown();
		delete m_ParticleSystem;
		m_ParticleSystem = 0;
	}
}

void EnemyClass::RenderParticle(ParticleShaderClass* shader)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	// Get the world, view, and projection matrices from the camera and d3d objects.
	camera->GetViewMatrix(viewMatrix);
	d3d->GetWorldMatrix(worldMatrix);
	d3d->GetProjectionMatrix(projectionMatrix);

	d3d->TurnOnParticleAlphaBlending();



	// atan2 함수를 이용하여 빌보드 모델에 적용될 회전값을 계산합니다. 이렇게 하여 빌보드 모델이 현재 카메라 위치를 바라보게 합니다.
	float angle = atan2(m_modelData.GetPos().x - camera->GetPosition().x, m_modelData.GetPos().z - camera->GetPosition().z) * (180.0f / 3.141592f);

	// 회전각도를 라디안으로 변환합니다.
	float rotation = (float)angle * 0.0174532925f;

	// 월드 행렬을 이용하여 원점에서의 빌보드의 회전값을 설정합니다.
	worldMatrix *= XMMatrixRotationY(rotation);

	XMMATRIX translateMatrix = XMMatrixTranslation(m_modelData.GetPos().x, m_modelData.GetPos().y, m_modelData.GetPos().z);

	// 마지막으로 회전 행렬와 이동 행렬을 조합하여 빌보드 모델의 최종 월드 행렬을 계산합니다.
	worldMatrix = XMMatrixMultiply(worldMatrix, translateMatrix);



	// 파티클 시스템 버텍스와 인덱스 버퍼를 그래픽 파이프 라인에 배치하여 그리기를 준비합니다.
	m_ParticleSystem->Render(d3d->GetDeviceContext());

	// 텍스처 셰이더를 사용하여 모델을 렌더링합니다.
	if (!shader->Render(d3d->GetDeviceContext(), m_ParticleSystem->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_ParticleSystem->GetTexture(), camera->GetPosition()))
	{
		return;
	}

	d3d->TurnOffParticleAlphaBlending();
}

void EnemyClass::Frame(float time)
{
	InteractableModelClass::Frame(time);
	FollowCamera(time);

	m_ParticleSystem->SetPosition(m_modelData.GetPos().x, m_modelData.GetPos().y, m_modelData.GetPos().z);
	// 파티클 시스템에 대한 프레임 처리를 실행합니다.
	m_ParticleSystem->Frame(time, d3d->GetDeviceContext());
}

void EnemyClass::FollowCamera(float time)
{
	if (active == false) return;

	XMVECTOR myVector = m_modelData.ReturnPositionVector();
	XMVECTOR camVector = camera->GetPositionVector();

	XMVECTOR dirVector = camVector - myVector;
	XMVECTOR nomalDir = XMVector3Normalize(dirVector);

	dirVector = nomalDir * followSpeed + pushVector;

	XMFLOAT3 pos;
	XMStoreFloat3(&pos, dirVector);

	XMFLOAT3 tmpPos = XMFLOAT3(m_modelData.GetPos().x + pos.x, m_modelData.GetPos().y, m_modelData.GetPos().z + pos.z);
	m_modelData.SetPos(tmpPos);


	XMFLOAT3 directionVector;
	XMStoreFloat3(&directionVector, -nomalDir);
	
	// 방향 벡터를 각도로 변환
	float angle = atan2(directionVector.x, directionVector.z) * (180.0f / XM_PI);
	m_modelData.SetRotation(XMFLOAT3(0, angle, 0));
}