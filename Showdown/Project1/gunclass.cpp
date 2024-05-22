#include "gunclass.h"

GunClass::GunClass(CameraClass* _camera, D3DClass* _d3d, HWND hwnd)
{
	camera = _camera;
	d3d = _d3d;

	m_hwnd = hwnd;

	points.push_back(XMFLOAT3(0, 0, 0));
	points.push_back(XMFLOAT3(0, 0, -0.02f));

	speed = 0.003f;

	goBackward = true;
	isFinish = false;
	nowReload = false;

	hp = maxHp;
	maxBulletCount = 10;

	fireDelay = 1000;
	reloadDelay = 3000;
	canFire = true;

	bulletCount = maxBulletCount;

	m_pick = 0;

	SetMove(true);
}


GunClass::~GunClass()
{

}

void GunClass::Hit(InteractableModelClass* myModel, InteractableModelClass* otherModel)
{
	InteractableModelClass::Hit(myModel, otherModel);

	XMVECTOR dirVector = myModel->GetModelData().ReturnPositionVector() - otherModel->GetModelData().ReturnPositionVector();
	XMVECTOR nomalDir = XMVector3Normalize(dirVector);
	XMVECTOR hitNomalVector = nomalDir;
	
	pushVector = XMVectorSet(XMVectorGetX(hitNomalVector), 0, XMVectorGetZ(hitNomalVector), 0.0f) * 0.05f;
}

void GunClass::NotHit()
{
	InteractableModelClass::NotHit();
	pushVector = XMVECTOR();
}

void GunClass::Frame(float time)
{
	InteractableModelClass::Frame(time);

	camera->AddPosition(pushVector);

	m_pick->Frame(time);

	if (fireDelay < storedFireDelay)
	{
		canFire = true;
	}
	else
	{
		storedFireDelay += time;
	}

	SetPosition(camera->GetPosition());

	if (bulletCount <= 0)
	{
		if (reloadDelay < storedReloadDelay)
		{
			nowReload = false;
			bulletCount = maxBulletCount;
			storedReloadDelay = 0;
		}
		else
		{
			if (nowReload == false)
			{
				nowReload = true;
				m_ReloadSound->Play();
			}
			storedReloadDelay += time;
		}
	}

	Recoil();
}

bool GunClass::GetNowReload()
{
	return nowReload;
}

bool GunClass::Initialize(ID3D11Device* device, string name, const WCHAR* v1, const WCHAR* v2, ModelData v3)
{
	bool result;

	InteractableModelClass::Initialize(device, name, v1, v2, v3);

	// Create the sound object.
	m_Sound = new SoundClass;
	if (!m_Sound)
	{
		return false;
	}

	// Initialize the sound object.
	result = m_Sound->Initialize(m_hwnd, "./data/gunSound1.wav");
	if (!result)
	{
		MessageBox(m_hwnd, L"Could not initialize Direct Sound.", L"Error", MB_OK);
		return false;
	}

	m_ReloadSound = new SoundClass;
	if (!m_ReloadSound)
	{
		return false;
	}

	// Initialize the sound object.
	result = m_ReloadSound->Initialize(m_hwnd, "./data/gunSound2.wav");
	if (!result)
	{
		MessageBox(m_hwnd, L"Could not initialize Direct Sound.", L"Error", MB_OK);
		return false;
	}

	// Create the input object.  This object will be used to handle reading the keyboard input from the user.
	m_pick = new PickClass(1280, 720);
	if (!m_pick)
	{
		return false;
	}

	return true;
}

void GunClass::OnClickDown(vector<InteractableModelClass*>& models)
{
	if (canFire == false) return;

	if (bulletCount <= 0) return;


	// Initialize the sound object.
	m_Sound->Play();


	m_pick->OnMouseDown(*camera, models);

	bulletCount -= 1;

	storedFireDelay = 0;
	canFire = false;

	float yaw = camera->ReturnCamYaw();
	float pitch = camera->ReturnCamPitch();

	XMMATRIX RotateYTempMatrix;
	RotateYTempMatrix = XMMatrixIdentity();

	RotateYTempMatrix *= XMMatrixRotationX(pitch); // 위의 변수를 이용해서 돌림
	RotateYTempMatrix *= XMMatrixRotationY(yaw); // 위의 변수를 이용해서 돌림

	XMVECTOR forward = XMVector3TransformCoord(m_DefaultForward, RotateYTempMatrix); // 카메라 앞 백터

	isFinish = false;
}

void GunClass::OnClickUp()
{
	m_pick->OnMouseUp();
}

int GunClass::ReturnHP()
{
	return hp;
}

int GunClass::ReturnBulletCount()
{
	return bulletCount;
}

int GunClass::ReturnRealScore()
{
	return m_pick->GetRealScore();
}

void GunClass::Recoil()
{
	if (isFinish == true) return;

	float yaw = camera->ReturnCamYaw();
	float pitch = camera->ReturnCamPitch();

	XMMATRIX RotateYTempMatrix;
	RotateYTempMatrix = XMMatrixRotationX(pitch); // 위의 변수를 이용해서 돌림
	RotateYTempMatrix *= XMMatrixRotationY(yaw); // 위의 변수를 이용해서 돌림

	XMVECTOR forward = XMVector3TransformCoord(m_DefaultForward, RotateYTempMatrix); // 카메라 앞 백터

	if (goBackward)
	{
		positionVector += -speed * forward;
	}
	else
	{
		positionVector += speed * forward;
	}
	
	storedTime += 0.1f;

	if (storedTime > 1)
	{
		goBackward = !goBackward;
		storedTime = 0;

		if (goBackward == true)
		{
			isFinish = true;
		}
	}

	XMFLOAT3 tmp;
	XMStoreFloat3(&tmp, positionVector);

	m_modelData.SetOffsetPos(tmp);
}

void GunClass::GetDamage()
{
	hp -= 1;
	if (hp <= 0)
	{
		SetPosition(XMFLOAT3(0.0f, 0.5f, -7.0f));
		camera->SetPosition(0.0f, 0.5f, -7.0f);

		m_pick->SetRealScore(0);
		hp = maxHp;
		bulletCount = maxBulletCount;
		storedReloadDelay = 0;
		nowReload = false;
		canFire = true;
		storedFireDelay = 0;
	}
}
