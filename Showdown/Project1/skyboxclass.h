#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: skyboxclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _SKYBOXCLASS_H_
#define _SKYBOXCLASS_H_

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

#pragma comment(lib, "d3dcompiler.lib")
#include <d3dcompiler.h>
#include "DDSTextureLoader.h"

using namespace std;

class Skybox : public AlignedAllocationPolicy<16>
{
	struct Vertex	//Overloaded Vertex Structure
	{
		Vertex() {}
		Vertex(float x, float y, float z,
			float u, float v,
			float nx, float ny, float nz)
			: pos(x, y, z), texCoord(u, v), normal(nx, ny, nz) {}

		XMFLOAT3 pos;
		XMFLOAT2 texCoord;
		XMFLOAT3 normal;
	};

	//Create effects constant buffer's structure//
	struct cbPerObject
	{
		XMMATRIX  WVP;
		XMMATRIX World;
	};

	

private:
	XMMATRIX Rotationx;
	XMMATRIX Rotationy;
	XMMATRIX Rotationz;

	XMMATRIX WVP;

	XMMATRIX Scale;
	XMMATRIX Translation;

	ID3D11Buffer* sphereIndexBuffer;
	ID3D11Buffer* sphereVertBuffer;

	ID3D11VertexShader* SKYMAP_VS;
	ID3D11PixelShader* SKYMAP_PS;
	ID3D10Blob* SKYMAP_VS_Buffer;
	ID3D10Blob* SKYMAP_PS_Buffer;

	ID3D11ShaderResourceView* smrv;

	ID3D11DepthStencilState* DSLessEqual;
	ID3D11RasterizerState* RSCullNone;

	int NumSphereVertices;
	int NumSphereFaces;

	XMMATRIX sphereWorld;

	ID3D11SamplerState* CubesTexSamplerState;

	cbPerObject cbPerObj;
	ID3D11Buffer* cbPerObjectBuffer;

	HRESULT hr;

public:
	Skybox();
	Skybox(const Skybox&);
	~Skybox();

	void CreateSphere(ID3D11Device* device, int LatLines, int LongLines);
	void CleanUp();
	void InitScene(ID3D11Device* device);
	void UpdateScene(double time, XMVECTOR camPosition);
	void DrawScene(ID3D11DeviceContext* deviceContext, XMMATRIX camView, XMMATRIX camProjection);
};
#endif