#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _INSTANCEMODELCLASS_H_
#define _INSTANCEMODELCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <directxmath.h>

using namespace DirectX;

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "modelclass.h"

#include "ModelData.h"

#include <vector>
#include <fstream>
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Class name: InstanceModelClass
////////////////////////////////////////////////////////////////////////////////
class InstanceModelClass : public ModelClass
{
private:
	struct InstanceType
	{
		XMFLOAT3 position;
		XMFLOAT3 rotation;
		XMFLOAT3 scale;
	};

public:
	InstanceModelClass();
	InstanceModelClass(const InstanceModelClass&);
	~InstanceModelClass();

	bool Initialize(ID3D11Device*, const WCHAR*, const WCHAR*, vector<ModelData>);
	void Shutdown();
	void Render(ID3D11DeviceContext*);
	int GetInstanceCount();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);
	

private:
	ID3D11Buffer* m_instanceBuffer;
	vector<ModelData> m_modelDatas;
};

#endif