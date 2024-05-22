#include "interactablemodelclass.h"

InteractableModelClass::InteractableModelClass()
{
	active = true;
}

InteractableModelClass::~InteractableModelClass()
{
}

void InteractableModelClass::SetActive(bool _isActive)
{
	active = _isActive;
}

void InteractableModelClass::SetMove(bool _isMove)
{
	isMove = _isMove;
}

XMVECTOR InteractableModelClass::ReturnBoundingBoxMinVertex()
{
	return boundingBoxMinVertex;
}

XMVECTOR InteractableModelClass::ReturnBoundingBoxMaxVertex()
{
	return boundingBoxMaxVertex;
}

XMVECTOR InteractableModelClass::ReturnCenterOffset()
{
	return centerOffset;
}

float InteractableModelClass::ReturnBoundingSphere()
{
	return boundingSphere;
}

vector<XMFLOAT3> InteractableModelClass::ReturnBoundingBoxVertPosArray()
{
	return boundingBoxVertPosArray;
}

vector<DWORD> InteractableModelClass::ReturnBoundingBoxVertIndexArray()
{
	return boundingBoxVertIndexArray;
}

bool InteractableModelClass::Initialize(ID3D11Device* device, string name, const WCHAR* modelFilename, const WCHAR* textureFilename, ModelData modelData)
{
	ModelClass::Initialize(device, name, modelFilename, textureFilename, modelData);
	vector<XMFLOAT3> tmpArray = ReturnVertPosArray();
	CreateBoundingVolumes(tmpArray, boundingBoxVertPosArray, boundingBoxVertIndexArray, boundingSphere, centerOffset);

	XMMATRIX matrix = ReturnModelMatrix();
	CalculateAABB(boundingBoxVertPosArray, matrix, boundingBoxMinVertex, boundingBoxMaxVertex);

	return true;
}


void InteractableModelClass::Frame(float time)
{
	if (isMove == false) return; // 움직일 때 적용

	XMMATRIX matrix = ReturnModelMatrix();
	CalculateAABB(boundingBoxVertPosArray, matrix, boundingBoxMinVertex, boundingBoxMaxVertex);
}

bool InteractableModelClass::IsActive()
{
	return active;
}

bool InteractableModelClass::IsHit()
{
	return nowHit;
}

void InteractableModelClass::PushWhenCollisionRequested(XMVECTOR myVector, XMVECTOR otherVector)
{
}

void InteractableModelClass::Pick(int* score)
{
}

void InteractableModelClass::Hit(InteractableModelClass* myModel, InteractableModelClass* otherModel)
{
	nowHit = true;
}

void InteractableModelClass::NotHit()
{
	nowHit = false;
}

void InteractableModelClass::RenderParticle(ParticleShaderClass* shader)
{
}

void InteractableModelClass::OnClickUp()
{

}

void InteractableModelClass::GetDamage()
{
}

void InteractableModelClass::OnClickDown(vector<InteractableModelClass*>& models)
{

}

///////////////**************new**************////////////////////
void InteractableModelClass::CreateBoundingVolumes(std::vector<XMFLOAT3>& vertPosArray,
	std::vector<XMFLOAT3>& boundingBoxVerts,
	std::vector<DWORD>& boundingBoxIndex,
	float& boundingSphere,
	XMVECTOR& objectCenterOffset)
{
	XMFLOAT3 minVertex = XMFLOAT3(FLT_MAX, FLT_MAX, FLT_MAX);
	XMFLOAT3 maxVertex = XMFLOAT3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	for (UINT i = 0; i < vertPosArray.size(); i++)
	{
		// The minVertex and maxVertex will most likely not be actual vertices in the model, but vertices
		// that use the smallest and largest x, y, and z values from the model to be sure ALL vertices are
		// covered by the bounding volume

		//Get the smallest vertex 
		minVertex.x = min(minVertex.x, vertPosArray[i].x);	// Find smallest x value in model
		minVertex.y = min(minVertex.y, vertPosArray[i].y);	// Find smallest y value in model
		minVertex.z = min(minVertex.z, vertPosArray[i].z);	// Find smallest z value in model

		//Get the largest vertex 
		maxVertex.x = max(maxVertex.x, vertPosArray[i].x);	// Find largest x value in model
		maxVertex.y = max(maxVertex.y, vertPosArray[i].y);	// Find largest y value in model
		maxVertex.z = max(maxVertex.z, vertPosArray[i].z);	// Find largest z value in model
	}

	// Compute distance between maxVertex and minVertex
	float distX = (maxVertex.x - minVertex.x) / 2.0f;
	float distY = (maxVertex.y - minVertex.y) / 2.0f;
	float distZ = (maxVertex.z - minVertex.z) / 2.0f;

	// Now store the distance between (0, 0, 0) in model space to the models real center
	objectCenterOffset = XMVectorSet(maxVertex.x - distX, maxVertex.y - distY, maxVertex.z - distZ, 0.0f);

	// Compute bounding sphere (distance between min and max bounding box vertices)
	// boundingSphere = sqrt(distX*distX + distY*distY + distZ*distZ) / 2.0f;
	boundingSphere = XMVectorGetX(XMVector3Length(XMVectorSet(distX, distY, distZ, 0.0f)));

	// Create bounding box	
	// Front Vertices
	boundingBoxVerts.push_back(XMFLOAT3(minVertex.x, minVertex.y, minVertex.z));
	boundingBoxVerts.push_back(XMFLOAT3(minVertex.x, maxVertex.y, minVertex.z));
	boundingBoxVerts.push_back(XMFLOAT3(maxVertex.x, maxVertex.y, minVertex.z));
	boundingBoxVerts.push_back(XMFLOAT3(maxVertex.x, minVertex.y, minVertex.z));

	// Back Vertices
	boundingBoxVerts.push_back(XMFLOAT3(minVertex.x, minVertex.y, maxVertex.z));
	boundingBoxVerts.push_back(XMFLOAT3(maxVertex.x, minVertex.y, maxVertex.z));
	boundingBoxVerts.push_back(XMFLOAT3(maxVertex.x, maxVertex.y, maxVertex.z));
	boundingBoxVerts.push_back(XMFLOAT3(minVertex.x, maxVertex.y, maxVertex.z));

	DWORD* i = new DWORD[36];

	// Front Face
	i[0] = 0; i[1] = 1; i[2] = 2;
	i[3] = 0; i[4] = 2; i[5] = 3;

	// Back Face
	i[6] = 4; i[7] = 5; i[8] = 6;
	i[9] = 4; i[10] = 6; i[11] = 7;

	// Top Face
	i[12] = 1; i[13] = 7; i[14] = 6;
	i[15] = 1; i[16] = 6; i[17] = 2;

	// Bottom Face
	i[18] = 0; i[19] = 4; i[20] = 5;
	i[21] = 0; i[22] = 5; i[23] = 3;

	// Left Face
	i[24] = 4; i[25] = 7; i[26] = 1;
	i[27] = 4; i[28] = 1; i[29] = 0;

	// Right Face
	i[30] = 3; i[31] = 2; i[32] = 6;
	i[33] = 3; i[34] = 6; i[35] = 5;

	for (int j = 0; j < 36; j++)
		boundingBoxIndex.push_back(i[j]);
}
///////////////**************new**************////////////////////


///////////////**************new**************////////////////////
bool InteractableModelClass::BoundingSphereCollision(float firstObjBoundingSphere,
	XMVECTOR firstObjCenterOffset,
	XMMATRIX& firstObjWorldSpace,
	float secondObjBoundingSphere,
	XMVECTOR secondObjCenterOffset,
	XMMATRIX& secondObjWorldSpace)
{
	//Declare local variables
	XMVECTOR world_1 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR world_2 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	float objectsDistance = 0.0f;

	//Transform the objects world space to objects REAL center in world space
	world_1 = XMVector3TransformCoord(firstObjCenterOffset, firstObjWorldSpace);
	world_2 = XMVector3TransformCoord(secondObjCenterOffset, secondObjWorldSpace);

	//Get the distance between the two objects
	objectsDistance = XMVectorGetX(XMVector3Length(world_1 - world_2));

	//If the distance between the two objects is less than the sum of their bounding spheres...
	if (objectsDistance <= (firstObjBoundingSphere + secondObjBoundingSphere))
		//Return true
		return true;

	//If the bounding spheres are not colliding, return false
	return false;
}

bool InteractableModelClass::BoundingBoxCollision(XMVECTOR& firstObjBoundingBoxMinVertex,
	XMVECTOR& firstObjBoundingBoxMaxVertex,
	XMVECTOR& secondObjBoundingBoxMinVertex,
	XMVECTOR& secondObjBoundingBoxMaxVertex)
{
	//Is obj1's max X greater than obj2's min X? If not, obj1 is to the LEFT of obj2
	if (XMVectorGetX(firstObjBoundingBoxMaxVertex) > XMVectorGetX(secondObjBoundingBoxMinVertex))

		//Is obj1's min X less than obj2's max X? If not, obj1 is to the RIGHT of obj2
		if (XMVectorGetX(firstObjBoundingBoxMinVertex) < XMVectorGetX(secondObjBoundingBoxMaxVertex))

			//Is obj1's max Y greater than obj2's min Y? If not, obj1 is UNDER obj2
			if (XMVectorGetY(firstObjBoundingBoxMaxVertex) > XMVectorGetY(secondObjBoundingBoxMinVertex))

				//Is obj1's min Y less than obj2's max Y? If not, obj1 is ABOVE obj2
				if (XMVectorGetY(firstObjBoundingBoxMinVertex) < XMVectorGetY(secondObjBoundingBoxMaxVertex))

					//Is obj1's max Z greater than obj2's min Z? If not, obj1 is IN FRONT OF obj2
					if (XMVectorGetZ(firstObjBoundingBoxMaxVertex) > XMVectorGetZ(secondObjBoundingBoxMinVertex))

						//Is obj1's min Z less than obj2's max Z? If not, obj1 is BEHIND obj2
						if (XMVectorGetZ(firstObjBoundingBoxMinVertex) < XMVectorGetZ(secondObjBoundingBoxMaxVertex))

							//If we've made it this far, then the two bounding boxes are colliding
							return true;

	//If the two bounding boxes are not colliding, then return false
	return false;
}

void InteractableModelClass::CalculateAABB(std::vector<XMFLOAT3> boundingBoxVerts,
	XMMATRIX& worldSpace,
	XMVECTOR& boundingBoxMin,
	XMVECTOR& boundingBoxMax)
{
	XMFLOAT3 minVertex = XMFLOAT3(FLT_MAX, FLT_MAX, FLT_MAX);
	XMFLOAT3 maxVertex = XMFLOAT3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	//Loop through the 8 vertices describing the bounding box
	for (UINT i = 0; i < 8; i++)
	{
		//Transform the bounding boxes vertices to the objects world space
		XMVECTOR Vert = XMVectorSet(boundingBoxVerts[i].x, boundingBoxVerts[i].y, boundingBoxVerts[i].z, 0.0f);
		Vert = XMVector3TransformCoord(Vert, worldSpace);

		//Get the smallest vertex 
		minVertex.x = min(minVertex.x, XMVectorGetX(Vert));	// Find smallest x value in model
		minVertex.y = min(minVertex.y, XMVectorGetY(Vert));	// Find smallest y value in model
		minVertex.z = min(minVertex.z, XMVectorGetZ(Vert));	// Find smallest z value in model

		//Get the largest vertex 
		maxVertex.x = max(maxVertex.x, XMVectorGetX(Vert));	// Find largest x value in model
		maxVertex.y = max(maxVertex.y, XMVectorGetY(Vert));	// Find largest y value in model
		maxVertex.z = max(maxVertex.z, XMVectorGetZ(Vert));	// Find largest z value in model
	}

	//Store Bounding Box's min and max vertices
	boundingBoxMin = XMVectorSet(minVertex.x, minVertex.y, minVertex.z, 0.0f);
	boundingBoxMax = XMVectorSet(maxVertex.x, maxVertex.y, maxVertex.z, 0.0f);
}
///////////////**************new**************////////////////////
