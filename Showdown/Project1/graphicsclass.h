////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "instancemodelclass.h"
#include "lightshaderclass.h"
#include "lightclass.h"

#include "textureshaderclass.h"
#include "bitmapclass.h"
#include "textclass.h"

#include "enemyclass.h"
#include "gunclass.h"

#include "ModelData.h"

#include "car.h"

#include "Day.h"
#include"skyboxclass.h"
#include "bottleclass.h"

#include "interactablemodelclass.h"

#include "ParticleShaderClass.h"
#include "ParticleSystemClass.h"

#include <vector>

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame(int, int, float);

	D3DClass* GetD3DClass();
	CameraClass* GetCameraClass();
	LightClass* GetLight();

	void ChangeTime(Day); // 렌더, 조명 색상 변경
	bool GetStart();
	void SetStart(bool);

	vector<InteractableModelClass*>& ReturnInteractableModel();

	void CheckCollision();

	void OnMouseDown();
	void OnMouseUp();

private:
	bool Render();

	float GetEulerAngle(float);

	bool InitAllModels(HWND hwnd);

	bool InitGunModels(HWND hwnd);

	bool InitAllInstanceModels(HWND hwnd);
	bool InitAllBillboardModels(HWND hwnd);

	bool InitAllCars(HWND hwnd);

	bool InitAllEnemys(HWND hwnd);


	bool InitAllBitmap(int screenWidth, int screenHeight, HWND hwnd);

	bool RenderAllCars();

	void SpawnEnemy(float time);


	bool InitAllInteractableModels(HWND hwnd);

	bool RenderAllInteractableModels();




	bool RenderAllModels();
	bool RenderAllInstanceModels();
	bool RenderAllBillboardModels();

	bool RenderUI();
	bool RenderBackground();

	int GetNumOfObjects();
	void ResetNumOfObjects();

	int GetNumOfPolygons();
	void ResetNumOfPolygons();

	void ChangeLightSetting();

private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;

	//BottleClass* m_Bottle;

	vector<InteractableModelClass*> m_InteractableModels;

	float euler = 0.0174532925f;

	Day m_day;
	Skybox* m_Skybox;


	vector<ModelClass*> m_Models;
	vector<InstanceModelClass*> m_InstanceModels;
	vector<InstanceModelClass*> m_BillboardModels;

	vector<Car*> m_Cars;

	LightShaderClass* m_LightShader;
	LightClass* m_Light;

	TextureShaderClass* m_TextureShader;

	BitmapClass* m_Start;

	BitmapClass* m_lineOfSight;

	vector<XMFLOAT3> spawnPoints;
	float spawnDelay;
	float storedSpawnDelay;

	float score;

	// bullet

	GunClass* gun;
	

	// bullet





	vector<TextClass*> m_Texts;

	float m_rotation = 0.0f;
	bool m_start = false;

	ParticleShaderClass* m_ParticleShader = nullptr;
};

#endif