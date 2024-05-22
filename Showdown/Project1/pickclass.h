#pragma once
// Filename: skyboxclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _PICKCLASS_H_
#define _PICKCLASS_H_

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

#include "bottleclass.h"
#include "interactablemodelclass.h"
#include "cameraclass.h"

#pragma comment(lib, "d3dcompiler.lib")
#include <d3dcompiler.h>
#include "DDSTextureLoader.h"

using namespace std;

class PickClass
{

public:
	PickClass(int width, int height);

private:
	///////////////**************new**************////////////////////
	bool isShoot = false;

	int clientWidth;
	int clientHeight;

	int realScore = 0;

	int score = 0;
	float pickedDist = 0.0f;

	float pickMode = 0;
	///////////////**************new**************////////////////////


public:
	void OnMouseDown(CameraClass&, vector<InteractableModelClass*>&);
	void OnMouseUp();

	int GetRealScore();
	void SetRealScore(int score);
	
	void Frame(float time);

	///////////////**************new**************////////////////////
	void pickRayVector(float, float, CameraClass&, XMVECTOR&, XMVECTOR&);
	float pick(XMVECTOR, XMVECTOR, std::vector<XMFLOAT3>&, std::vector<DWORD>&, XMMATRIX&);
	bool PointInTriangle(XMVECTOR&, XMVECTOR&, XMVECTOR&, XMVECTOR&);
	///////////////**************new**************////////////////////
};
#endif
