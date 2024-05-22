#pragma once
class enemyclass
{
};

// Filename: skyboxclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _ENEMYCLASS_H_
#define _ENEMYCLASS_H_

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <directxmath.h>

using namespace DirectX;

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "textureclass.h"
#include <string>
#include <fstream>
#include <vector>
#include "ModelData.h"

#include "interactablemodelclass.h"
#include "cameraclass.h"

#include "soundclass.h"

#pragma comment(lib, "d3dcompiler.lib")
#include <d3dcompiler.h>
#include "DDSTextureLoader.h"

using namespace std;

class EnemyClass : public InteractableModelClass
{

public:
	EnemyClass(CameraClass*, D3DClass*, HWND hwnd);
	~EnemyClass();

private:
	CameraClass* camera;
	D3DClass* d3d;

	float followSpeed = 0.005f;
	float pushSpeed;
	XMVECTOR pushVector;

	XMVECTOR hitNomalVector;

	ParticleSystemClass* m_ParticleSystem = nullptr;

	SoundClass* m_Sound;
	HWND m_hwnd;

public:

	virtual bool Initialize(ID3D11Device* device, string name, const WCHAR*, const WCHAR*, ModelData) override;

	virtual void Shutdown() override;

	virtual void RenderParticle(ParticleShaderClass* shader) override;

	virtual void Frame(float time) override;
	void FollowCamera(float time);

	virtual void Hit(InteractableModelClass* myModel, InteractableModelClass* otherModel) override;
	virtual void NotHit() override;

	virtual void Pick(int* score) override;
};
#endif

