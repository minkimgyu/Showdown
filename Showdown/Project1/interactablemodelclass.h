#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: interactablemodelclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _INTERACTABLEMODELCLASS_H_
#define _INTERACTABLEMODELCLASS_H_


//////////////
// INCLUDES //
//////////////

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "modelclass.h"

#include "ModelData.h"

#include "ParticleShaderClass.h"
#include "ParticleSystemClass.h"

#include <vector>
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Class name: InstanceModelClass
////////////////////////////////////////////////////////////////////////////////
class InteractableModelClass : public ModelClass
{

protected:
	bool nowHit;
	bool active;

private:
	bool isMove;

	XMVECTOR centerOffset;
	float boundingSphere = 0.0f;

	vector<XMFLOAT3> boundingBoxVertPosArray;
	vector<DWORD> boundingBoxVertIndexArray;

	XMVECTOR boundingBoxMinVertex;
	XMVECTOR boundingBoxMaxVertex;

public:
	InteractableModelClass();
	~InteractableModelClass();

	void SetMove(bool);

	XMVECTOR ReturnBoundingBoxMinVertex();
	XMVECTOR ReturnBoundingBoxMaxVertex();

	XMVECTOR ReturnCenterOffset();
	float ReturnBoundingSphere();

	vector<XMFLOAT3> ReturnBoundingBoxVertPosArray();
	vector<DWORD> ReturnBoundingBoxVertIndexArray();

	virtual bool Initialize(ID3D11Device* device, string name, const WCHAR*, const WCHAR*, ModelData) override;

	virtual void Frame(float time) override;

	///////////////**************new**************////////////////////
	void CreateBoundingVolumes(std::vector<XMFLOAT3>& vertPosArray,	// The array containing our models vertex positions
		std::vector<XMFLOAT3>& boundingBoxVerts,							// Array we want to store the bounding box's vertex positions
		std::vector<DWORD>& boundingBoxIndex,							// This is our bounding box's index array
		float& boundingSphere,											// The float containing the radius of our bounding sphere
		XMVECTOR& objectCenterOffset);									// A vector containing the distance between the models actual center and (0, 0, 0) in model space
	///////////////**************new**************////////////////////

	///////////////**************new**************////////////////////
	bool BoundingSphereCollision(float firstObjBoundingSphere,
		XMVECTOR firstObjCenterOffset,
		XMMATRIX& firstObjWorldSpace,
		float secondObjBoundingSphere,
		XMVECTOR secondObjCenterOffset,
		XMMATRIX& secondObjWorldSpace);

	bool BoundingBoxCollision(XMVECTOR& firstObjBoundingBoxMinVertex,
		XMVECTOR& firstObjBoundingBoxMaxVertex,
		XMVECTOR& secondObjBoundingBoxMinVertex,
		XMVECTOR& secondObjBoundingBoxMaxVertex);

	void CalculateAABB(std::vector<XMFLOAT3> boundingBoxVerts,
		XMMATRIX& worldSpace,
		XMVECTOR& boundingBoxMin,
		XMVECTOR& boundingBoxMax);
	///////////////**************new**************////////////////////

	virtual void PushWhenCollisionRequested(XMVECTOR myVector, XMVECTOR otherVector);

	bool IsActive();

	void SetActive(bool);

	virtual void Pick(int* score);


	bool IsHit();
	virtual void Hit(InteractableModelClass* myModel, InteractableModelClass* otherModel);
	virtual void NotHit();

	virtual void GetDamage();

	virtual void RenderParticle(ParticleShaderClass* shader);

	virtual void OnClickDown(vector<InteractableModelClass*>&);
	virtual void OnClickUp();
};

#endif

