////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_


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
#include "AlignedAllocationPolicy.h"


using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Class name: ModelClass
////////////////////////////////////////////////////////////////////////////////
class ModelClass : public AlignedAllocationPolicy<16>
{
protected:
	struct VertexType
	{
		XMFLOAT3 position;
	    XMFLOAT2 texture;
		XMFLOAT3 normal;
	};

	struct FaceType
	{
		int vIndex1, vIndex2, vIndex3;
		int tIndex1, tIndex2, tIndex3;
		int nIndex1, nIndex2, nIndex3;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(ID3D11Device*, string name, const WCHAR*, const WCHAR*);
	virtual bool Initialize(ID3D11Device* device, string name, const WCHAR* modelFilename, const WCHAR* textureFilename, ModelData modelData);
	virtual void Shutdown();
	virtual void Render(ID3D11DeviceContext*);
	virtual void Frame(float time);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

	bool LoadModel(const WCHAR*);
	void ReleaseModel();

	bool ReadFileCounts(const WCHAR*);
	bool LoadDataStructures(const WCHAR*, int, int, int, int);

	string GetName();
	void SetName(string);

	ModelData GetModelData();
	XMFLOAT3 SetAngleToEuler(XMFLOAT3);

	XMMATRIX ReturnModelMatrix();

	vector<XMFLOAT3> ReturnVertPosArray();
	vector<DWORD> ReturnVertIndexArray();

	void SetPosition(XMFLOAT3);

	XMFLOAT3 GetOffsetPosition();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);


private:
	string name;
	

protected:
	bool LoadTexture(ID3D11Device*, const WCHAR*);
	void ReleaseTexture();

protected:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount, m_textureCount, m_normalCount, m_faceCount;
	TextureClass* m_Texture;

	vector<XMFLOAT3> vertPosArray;
	vector<DWORD> vertIndexArray;

	float euler = 0.0174532925f;

	ModelType* m_model;
	ModelData m_modelData;
};

#endif