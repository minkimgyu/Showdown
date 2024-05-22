#pragma once

#include "d3dclass.h"

#include <string>
#include <vector>
using namespace std;

struct ModelData
{
	ModelData();
	~ModelData();

	ModelData(XMFLOAT3 pos);
	ModelData(XMFLOAT3 pos, XMFLOAT3 rotation);
	ModelData(XMFLOAT3 pos, XMFLOAT3 rotation, XMFLOAT3 scale);

private:
	XMFLOAT3 pos;
	XMFLOAT3 offsetPos;
	XMFLOAT3 rotation;
	XMFLOAT3 scale;

public:
	XMFLOAT3 GetPos();
	XMFLOAT3 GetOffsetPos();
	void SetPos(XMFLOAT3 pos);
	void SetOffsetPos(XMFLOAT3 pos);
	void SetRotation(XMFLOAT3 rotation);
	XMFLOAT3 GetRotation();
	XMFLOAT3 GetScale();

	XMVECTOR ReturnPositionVector();
};

struct File
{
	File(const string modelName, const WCHAR* name, const WCHAR* path);
	~File();
	File(const string modelName, const WCHAR* name, const WCHAR* path, vector<ModelData> instanceData);
	File(const string modelName, const WCHAR* name, const WCHAR* path, ModelData modelData);

public:
	const string GetModelName();
	const WCHAR* GetFileName();
	const WCHAR* GetFilePath();
	vector<ModelData> GetInstanceData();
	ModelData GetModelData();

private:
	const string modelName;
	const WCHAR* name;
	const WCHAR* path;
	vector<ModelData> instanceData;
	ModelData modelData;
};