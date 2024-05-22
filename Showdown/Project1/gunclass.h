#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: interactablemodelclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GUNCLASS_H_
#define _GUNCLASS_H_


//////////////
// INCLUDES //
//////////////

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "modelclass.h"

#include "cameraclass.h"

#include "ModelData.h"
#include "interactablemodelclass.h"

#include "pickclass.h"
#include "soundclass.h"

#include <vector>
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Class name: InstanceModelClass
////////////////////////////////////////////////////////////////////////////////
class GunClass : public InteractableModelClass
{
private:
	vector<XMFLOAT3> points;
	float t;
	float speed;
	int nowIndex;

	XMVECTOR m_DefaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR positionVector;

	CameraClass* camera;
	D3DClass* d3d;

	float storedTime;
	bool goBackward;

	bool isFinish;

	int hp;
	int maxHp = 3;
	int maxBulletCount;
	int bulletCount;

	bool canFire;

	float fireDelay;
	float storedFireDelay;

	float reloadDelay;
	float storedReloadDelay;

	bool nowReload;

	PickClass* m_pick;
	SoundClass* m_Sound;
	SoundClass* m_ReloadSound;

	XMVECTOR pushVector;

	HWND m_hwnd;

public:
	GunClass(CameraClass*, D3DClass*, HWND);
	~GunClass();

	bool GetNowReload();

	virtual bool Initialize(ID3D11Device* device, string name, const WCHAR* v1, const WCHAR* v2, ModelData v3) override;
	
	int ReturnHP();
	int ReturnBulletCount();

	int ReturnRealScore();

	virtual void Frame(float time) override;
	void Recoil();

	virtual void GetDamage() override;

	virtual void Hit(InteractableModelClass* myModel, InteractableModelClass* otherModel) override;
	virtual void NotHit() override;

	virtual void OnClickDown(vector<InteractableModelClass*>&) override;
	virtual void OnClickUp() override;
};

#endif