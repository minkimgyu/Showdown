#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: skyboxclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _BOTTLECLASS_H_
#define _BOTTLECLASS_H_

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

#include <sstream>

#include "cameraclass.h"

#pragma comment(lib, "d3dcompiler.lib")
#include <d3dcompiler.h>
#include "DDSTextureLoader.h"

using namespace std;

class BottleClass
{
	struct Vertex	//Overloaded Vertex Structure
	{
		Vertex() {}
		Vertex(float x, float y, float z,
			float u, float v,
			float nx, float ny, float nz,
			float tx, float ty, float tz)
			: pos(x, y, z), texCoord(u, v), normal(nx, ny, nz),
			tangent(tx, ty, tz) {}

		XMFLOAT3 pos;
		XMFLOAT2 texCoord;
		XMFLOAT3 normal;
		XMFLOAT3 tangent;
		XMFLOAT3 biTangent;
	};

	//Create material structure
	struct SurfaceMaterial
	{
		std::wstring matName;
		XMFLOAT4 difColor;
		int texArrayIndex;
		int normMapTexArrayIndex;
		bool hasNormMap;
		bool hasTexture;
		bool transparent;
	};

	//Create effects constant buffer's structure//
	struct cbPerObject
	{
		XMMATRIX  WVP;
		XMMATRIX World;

		//These will be used for the pixel shader
		XMFLOAT4 difColor;
		BOOL hasTexture;
		//Because of HLSL structure packing, we will use windows BOOL
		//instead of bool because HLSL packs things into 4 bytes, and
		//bool is only one byte, where BOOL is 4 bytes
		BOOL hasNormMap;
	};

	

public:
	int* ReturnBottleHit();
	std::vector<XMFLOAT3> ReturnBottleVertPosArray();
	std::vector<DWORD> ReturnBottleVertIndexArray();
	XMMATRIX* ReturnBottleWorld();
	int ReturnNumBottles();
	void CleanUp();
	void InitScene(ID3D11Device*, IDXGISwapChain*);
	void DrawScene(ID3D11DeviceContext*, XMMATRIX, XMMATRIX);

private:
	HRESULT hr;

	ID3D11BlendState* Transparency;

	XMMATRIX WVP;

	ID3D11SamplerState* CubesTexSamplerState;

	ID3D11RasterizerState* RSCullNone;
	ID3D11Buffer* cbPerObjectBuffer;
	cbPerObject cbPerObj;

	///////////////**************new**************////////////////////
	std::vector<XMFLOAT3> groundVertPosArray;
	std::vector<DWORD> groundVertIndexArray;
	///////////////**************new**************////////////////////

	XMMATRIX Rotation;
	XMMATRIX Scale;
	XMMATRIX Translation;

	///////////////**************new**************////////////////////
	//Bottle mesh variables//
	ID3D11Buffer* bottleVertBuff;
	ID3D11Buffer* bottleIndexBuff;
	std::vector<XMFLOAT3> bottleVertPosArray;
	std::vector<DWORD> bottleVertIndexArray;
	int bottleSubsets = 0;
	std::vector<int> bottleSubsetIndexStart;
	std::vector<int> bottleSubsetTexture;
	XMMATRIX bottleWorld[20];
	int* bottleHit = new int[20];
	int numBottles = 20;


	int meshSubsets = 0;

	///////////////**************new**************////////////////////
	
	std::vector<SurfaceMaterial> material;

	///////////////**************new**************////////////////////

//Textures and material variables, used for all mesh's loaded
	std::vector<ID3D11ShaderResourceView*> meshSRV;
	std::vector<std::wstring> textureNameArray;

	/////////////////**************new**************////////////////////
	//bool isShoot = false;

	//int score = 0;
	//float pickedDist = 0.0f;
	/////////////////**************new**************////////////////////

public:
	BottleClass();
	BottleClass(const BottleClass&);
	~BottleClass();

	/////////////////**************new**************////////////////////
	//void pickRayVector(float mouseX, float mouseY, XMVECTOR& pickRayInWorldSpacePos, XMVECTOR& pickRayInWorldSpaceDir);
	//float pick(XMVECTOR pickRayInWorldSpacePos,
	//	XMVECTOR pickRayInWorldSpaceDir,
	//	std::vector<XMFLOAT3>& vertPosArray,
	//	std::vector<DWORD>& indexPosArray,
	//	XMMATRIX& worldSpace);
	//bool PointInTriangle(XMVECTOR& triV1, XMVECTOR& triV2, XMVECTOR& triV3, XMVECTOR& point);
	/////////////////**************new**************////////////////////

	//Define LoadObjModel function after we create surfaceMaterial structure
	bool LoadObjModel(std::wstring filename,			//.obj filename
		ID3D11Buffer** vertBuff,					//mesh vertex buffer
		ID3D11Buffer** indexBuff,					//mesh index buffer
		std::vector<int>& subsetIndexStart,			//start index of each subset
		std::vector<int>& subsetMaterialArray,		//index value of material for each subset
		std::vector<SurfaceMaterial>& material,		//vector of material structures
		int& subsetCount,							//Number of subsets in mesh
		bool isRHCoordSys,							//true if model was created in right hand coord system
		bool computeNormals,						//true to compute the normals, false to use the files normals
		///////////////**************new**************////////////////////
		std::vector<XMFLOAT3>& vertPosArray,		//Used for CPU to do calculations on the Geometry
		std::vector<DWORD>& vertIndexArray
		,ID3D11Device*, IDXGISwapChain*);		//Also used for CPU caculations on geometry
	///////////////**************new**************////////////////////
};
#endif