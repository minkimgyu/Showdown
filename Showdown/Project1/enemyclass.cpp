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

	// ��ƼŬ �ý��� ��ü�� ����ϴ�.
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

	// ��ƼŬ �ý��� ��ü�� �ʱ�ȭ�մϴ�.
	if (!m_ParticleSystem->Initialize(device, L"./data/star.dds"))
	{
		return false;
	}

	return true;
}

void EnemyClass::Shutdown()
{
	ModelClass::Shutdown();

	// particle shader ��ü�� �����Ѵ�.
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



	// atan2 �Լ��� �̿��Ͽ� ������ �𵨿� ����� ȸ������ ����մϴ�. �̷��� �Ͽ� ������ ���� ���� ī�޶� ��ġ�� �ٶ󺸰� �մϴ�.
	float angle = atan2(m_modelData.GetPos().x - camera->GetPosition().x, m_modelData.GetPos().z - camera->GetPosition().z) * (180.0f / 3.141592f);

	// ȸ�������� �������� ��ȯ�մϴ�.
	float rotation = (float)angle * 0.0174532925f;

	// ���� ����� �̿��Ͽ� ���������� �������� ȸ������ �����մϴ�.
	worldMatrix *= XMMatrixRotationY(rotation);

	XMMATRIX translateMatrix = XMMatrixTranslation(m_modelData.GetPos().x, m_modelData.GetPos().y, m_modelData.GetPos().z);

	// ���������� ȸ�� ��Ŀ� �̵� ����� �����Ͽ� ������ ���� ���� ���� ����� ����մϴ�.
	worldMatrix = XMMatrixMultiply(worldMatrix, translateMatrix);



	// ��ƼŬ �ý��� ���ؽ��� �ε��� ���۸� �׷��� ������ ���ο� ��ġ�Ͽ� �׸��⸦ �غ��մϴ�.
	m_ParticleSystem->Render(d3d->GetDeviceContext());

	// �ؽ�ó ���̴��� ����Ͽ� ���� �������մϴ�.
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
	// ��ƼŬ �ý��ۿ� ���� ������ ó���� �����մϴ�.
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
	
	// ���� ���͸� ������ ��ȯ
	float angle = atan2(directionVector.x, directionVector.z) * (180.0f / XM_PI);
	m_modelData.SetRotation(XMFLOAT3(0, angle, 0));
}