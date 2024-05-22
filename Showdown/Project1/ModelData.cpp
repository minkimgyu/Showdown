#include "ModelData.h"

ModelData::ModelData()
	:pos(XMFLOAT3(0, 0, 0)), rotation(XMFLOAT3(0, 0, 0)), scale(XMFLOAT3(1, 1, 1)), offsetPos(XMFLOAT3(0, 0, 0))
{

}

ModelData::ModelData(XMFLOAT3 pos)
	:pos(pos), rotation(XMFLOAT3(0, 0, 0)), scale(XMFLOAT3(1, 1, 1))
{
	offsetPos = XMFLOAT3(0, 0, 0);
}

ModelData::ModelData(XMFLOAT3 pos, XMFLOAT3 rotation)
	:pos(pos), rotation(rotation), scale(XMFLOAT3(1, 1, 1))
{
	offsetPos = XMFLOAT3(0, 0, 0);
}

ModelData::ModelData(XMFLOAT3 pos, XMFLOAT3 rotation, XMFLOAT3 scale)
	:pos(pos), rotation(rotation), scale(scale)
{
	offsetPos = XMFLOAT3(0, 0, 0);
}

ModelData::~ModelData() {}

XMFLOAT3 ModelData::GetPos() { return pos; }

XMFLOAT3 ModelData::GetOffsetPos()
{
	return offsetPos;
}

void ModelData::SetPos(XMFLOAT3 position) { pos = position; }

void ModelData::SetOffsetPos(XMFLOAT3 pos)
{
	offsetPos = pos;
}

void ModelData::SetRotation(XMFLOAT3 ro)
{
	rotation = ro;
}

XMFLOAT3 ModelData::GetRotation() { return rotation; }

XMFLOAT3 ModelData::GetScale() { return scale; }

XMVECTOR ModelData::ReturnPositionVector()
{
	return XMLoadFloat3(&pos);
}


File::File(const string modelName, const WCHAR* name, const WCHAR* path)
	:modelName(modelName), name(name), path(path)
{

}

File::~File()
{
}

File::File(const string modelName, const WCHAR* name, const WCHAR* path, vector<ModelData> instanceData)
	:modelName(modelName), name(name), path(path), instanceData(instanceData)
{

}

File::File(const string modelName, const WCHAR* name, const WCHAR* path, ModelData modelData)
	:modelName(modelName), name(name), path(path), modelData(modelData)
{

}

const string File::GetModelName() { return modelName; }

const WCHAR* File::GetFileName() { return name; }

const WCHAR* File::GetFilePath() { return path; }


vector<ModelData> File::GetInstanceData() { return instanceData; }

ModelData File::GetModelData() { return modelData; }
