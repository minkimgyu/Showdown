#include "pickclass.h"

PickClass::PickClass(int width, int height)
	:clientWidth(width), clientHeight(height)
{
}


void PickClass::OnMouseDown(CameraClass& camera, vector<InteractableModelClass*>& models) // bottle이 아닌 모델 클레스로 변경
{
	if (isShoot == false)
	{
		int mousex = 0;
		int mousey = 0;

		float tempDist;
		float closestDist = FLT_MAX;
		int hitIndex;

		XMVECTOR prwsPos, prwsDir;
		pickRayVector((float)mousex, (float)mousey, camera, prwsPos, prwsDir);

		vector<vector<XMFLOAT3>> tmpVertPosArray;
		vector<vector<DWORD>> tmpVertIndexArray;
		vector<XMMATRIX> tmpWorldArray;

		/*for (int i = 0; i < models.size(); i++)
		{
			if (models[i]->GetName() == "GunAndArm")
			{
				models[i]->OnClickDown();
			}
		}*/

		int sizeOfModel = (int)models.size();

		for (int i = 0; i < sizeOfModel; i++)
		{
			tmpVertPosArray.push_back(models[i]->ReturnVertPosArray());
			tmpVertIndexArray.push_back(models[i]->ReturnVertIndexArray());
			tmpWorldArray.push_back(models[i]->ReturnModelMatrix());

			if (models[i]->IsHit() == false) //No need to check bottles already hit
			{
				if (pickMode == 0)
				{
					float pRToPointDist = 0.0f; // Closest distance from the pick ray to the objects center

					XMVECTOR bottlePos = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
					XMVECTOR pOnLineNearBottle = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

					// For the Bounding Sphere to work correctly, we need to make sure we are testing
					// the distance from the objects "actual" center and the pick ray. We have stored
					// the distance from (0, 0, 0) in the objects model space to the object "actual"
					// center in bottleCenterOffset. So now we just need to add that difference to
					// the bottles world space position, this way the bounding sphere will be centered
					// on the object real center.

					
					bottlePos = XMVector3TransformCoord(bottlePos, tmpWorldArray[i]) + models[i]->ReturnCenterOffset();

					// This equation gets the point on the pick ray which is closest to bottlePos
					pOnLineNearBottle = prwsPos + XMVector3Dot((bottlePos - prwsPos), prwsDir) / XMVector3Dot(prwsDir, prwsDir) * prwsDir;

					// Now we get the distance between bottlePos and pOnLineNearBottle
					// This line is slightly less accurate, but it offers a performance increase by
					// estimating the distance using XMVector3LengthEst()
					//pRToPointDist = XMVectorGetX(XMVector3LengthEst(pOnLineNearBottle - bottlePos));				
					pRToPointDist = XMVectorGetX(XMVector3Length(pOnLineNearBottle - bottlePos));

					// If the distance between the closest point on the pick ray (pOnLineNearBottle) to bottlePos
					// is less than the bottles bounding sphere (represented by a float called bottleBoundingSphere)
					// then we know the pick ray has intersected with the bottles bounding sphere, and we can move on
					// to testing if the pick ray has actually intersected with the bottle itself.
					//if (pRToPointDist < models[i]->ReturnBoundingSphere())
					//{
					//	// This line is the distance to the pick ray intersection with the sphere
					//	//tempDist = XMVectorGetX(XMVector3Length(pOnLineNearBottle - prwsPos));

					//	// Check for picking with the actual model now
					//	tempDist = pick(prwsPos, prwsDir, tmpVertPosArray[i], tmpVertIndexArray[i], tmpWorldArray[i]);
					//}

					tempDist = pick(prwsPos, prwsDir, tmpVertPosArray[i], tmpVertIndexArray[i], tmpWorldArray[i]);
				}
				// Bounding Box picking test
				else if (pickMode == 1)
				{
					vector<XMFLOAT3> vertPosArray = models[i]->ReturnBoundingBoxVertPosArray();
					vector<DWORD> vertIndexArray = models[i]->ReturnBoundingBoxVertIndexArray();

					tempDist = pick(prwsPos, prwsDir, vertPosArray, vertIndexArray, tmpWorldArray[i]);

				}
				// Check for picking directly with the model without bounding volumes testing first
				else
				{
					tempDist = pick(prwsPos, prwsDir, tmpVertPosArray[i], tmpVertIndexArray[i], tmpWorldArray[i]);
				}

				if (tempDist < closestDist)
				{
					closestDist = tempDist;
					hitIndex = i;
				}
			}
		}

		if (closestDist < FLT_MAX)
		{
			models[hitIndex]->Pick(&realScore);
			pickedDist = closestDist;
			score++;
		}

		isShoot = true;
	}
}

void PickClass::OnMouseUp()
{
	isShoot = false;
}

int PickClass::GetRealScore()
{
	return realScore;
}

void PickClass::SetRealScore(int score)
{
	realScore = score;
}

void PickClass::Frame(float time)
{

}

void PickClass::pickRayVector(float mouseX, float mouseY, CameraClass& camera, XMVECTOR& pickRayInWorldSpacePos, XMVECTOR& pickRayInWorldSpaceDir)
{
	XMVECTOR pickRayInViewSpaceDir = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR pickRayInViewSpacePos = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	float PRVecX, PRVecY, PRVecZ;

	//Transform 2D pick position on screen space to 3D ray in View space
//	PRVecX =  ((( 2.0f * mouseX) / ClientWidth ) - 1 ) / camProjection(0,0);
//	PRVecY = -((( 2.0f * mouseY) / ClientHeight) - 1 ) / camProjection(1,1);
	XMFLOAT3X3 camProjection4;

	XMStoreFloat3x3(&camProjection4, camera.ReturnProjectionMatrix());
	PRVecX = (((2.0f * mouseX) / clientWidth) - 1) / camProjection4._11;
	PRVecY = -(((2.0f * mouseY) / clientHeight) - 1) / camProjection4._22;
	PRVecZ = 1.0f;	//View space's Z direction ranges from 0 to 1, so we set 1 since the ray goes "into" the screen

	//pickRayInViewSpaceDir = XMVectorSet(PRVecX, PRVecY, PRVecZ, 0.0f);

	//Uncomment this line if you want to use the center of the screen (client area)
	//to be the point that creates the picking ray (eg. first person shooter)
	pickRayInViewSpaceDir = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	// Transform 3D Ray from View space to 3D ray in World space
	XMMATRIX pickRayToWorldSpaceMatrix;
	XMVECTOR matInvDeter;	//We don't use this, but the xna matrix inverse function requires the first parameter to not be null

	pickRayToWorldSpaceMatrix = XMMatrixInverse(&matInvDeter, camera.ReturnViewMatrix());	//Inverse of View Space matrix is World space matrix

	pickRayInWorldSpacePos = XMVector3TransformCoord(pickRayInViewSpacePos, pickRayToWorldSpaceMatrix);
	pickRayInWorldSpaceDir = XMVector3TransformNormal(pickRayInViewSpaceDir, pickRayToWorldSpaceMatrix);
}



float PickClass::pick(XMVECTOR pickRayInWorldSpacePos,
	XMVECTOR pickRayInWorldSpaceDir,
	std::vector<XMFLOAT3>& vertPosArray,
	std::vector<DWORD>& indexPosArray,
	XMMATRIX& worldSpace)
{
	//Loop through each triangle in the object
	for (int i = 0; i < (int)indexPosArray.size() / 3; i++)
	{
		//Triangle's vertices V1, V2, V3
		XMVECTOR tri1V1 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		XMVECTOR tri1V2 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		XMVECTOR tri1V3 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

		//Temporary 3d floats for each vertex
		XMFLOAT3 tV1, tV2, tV3;

		//Get triangle 
		tV1 = vertPosArray[indexPosArray[(i * 3) + 0]];
		tV2 = vertPosArray[indexPosArray[(i * 3) + 1]];
		tV3 = vertPosArray[indexPosArray[(i * 3) + 2]];

		tri1V1 = XMVectorSet(tV1.x, tV1.y, tV1.z, 0.0f);
		tri1V2 = XMVectorSet(tV2.x, tV2.y, tV2.z, 0.0f);
		tri1V3 = XMVectorSet(tV3.x, tV3.y, tV3.z, 0.0f);

		//Transform the vertices to world space
		tri1V1 = XMVector3TransformCoord(tri1V1, worldSpace);
		tri1V2 = XMVector3TransformCoord(tri1V2, worldSpace);
		tri1V3 = XMVector3TransformCoord(tri1V3, worldSpace);

		//Find the normal using U, V coordinates (two edges)
		XMVECTOR U = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		XMVECTOR V = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		XMVECTOR faceNormal = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

		U = tri1V2 - tri1V1;
		V = tri1V3 - tri1V1;

		//Compute face normal by crossing U, V
		faceNormal = XMVector3Cross(U, V);

		faceNormal = XMVector3Normalize(faceNormal);

		//Calculate a point on the triangle for the plane equation
		XMVECTOR triPoint = tri1V1;

		//Get plane equation ("Ax + By + Cz + D = 0") Variables
		float tri1A = XMVectorGetX(faceNormal);
		float tri1B = XMVectorGetY(faceNormal);
		float tri1C = XMVectorGetZ(faceNormal);
		float tri1D = (-tri1A * XMVectorGetX(triPoint) - tri1B * XMVectorGetY(triPoint) - tri1C * XMVectorGetZ(triPoint));

		//Now we find where (on the ray) the ray intersects with the triangles plane
		float ep1, ep2, t = 0.0f;
		float planeIntersectX, planeIntersectY, planeIntersectZ = 0.0f;
		XMVECTOR pointInPlane = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

		ep1 = (XMVectorGetX(pickRayInWorldSpacePos) * tri1A) + (XMVectorGetY(pickRayInWorldSpacePos) * tri1B) + (XMVectorGetZ(pickRayInWorldSpacePos) * tri1C);
		ep2 = (XMVectorGetX(pickRayInWorldSpaceDir) * tri1A) + (XMVectorGetY(pickRayInWorldSpaceDir) * tri1B) + (XMVectorGetZ(pickRayInWorldSpaceDir) * tri1C);

		//Make sure there are no divide-by-zeros
		if (ep2 != 0.0f)
			t = -(ep1 + tri1D) / (ep2);

		if (t > 0.0f)    //Make sure you don't pick objects behind the camera
		{
			//Get the point on the plane
			planeIntersectX = XMVectorGetX(pickRayInWorldSpacePos) + XMVectorGetX(pickRayInWorldSpaceDir) * t;
			planeIntersectY = XMVectorGetY(pickRayInWorldSpacePos) + XMVectorGetY(pickRayInWorldSpaceDir) * t;
			planeIntersectZ = XMVectorGetZ(pickRayInWorldSpacePos) + XMVectorGetZ(pickRayInWorldSpaceDir) * t;

			pointInPlane = XMVectorSet(planeIntersectX, planeIntersectY, planeIntersectZ, 0.0f);

			//Call function to check if point is in the triangle
			if (PointInTriangle(tri1V1, tri1V2, tri1V3, pointInPlane))
			{
				//Return the distance to the hit, so you can check all the other pickable objects in your scene
				//and choose whichever object is closest to the camera
				return t / 2.0f;
			}
		}
	}
	//return the max float value (near infinity) if an object was not picked
	return FLT_MAX;
}

bool PickClass::PointInTriangle(XMVECTOR& triV1, XMVECTOR& triV2, XMVECTOR& triV3, XMVECTOR& point)
{
	//To find out if the point is inside the triangle, we will check to see if the point
	//is on the correct side of each of the triangles edges.

	XMVECTOR cp1 = XMVector3Cross((triV3 - triV2), (point - triV2));
	XMVECTOR cp2 = XMVector3Cross((triV3 - triV2), (triV1 - triV2));
	if (XMVectorGetX(XMVector3Dot(cp1, cp2)) >= 0)
	{
		cp1 = XMVector3Cross((triV3 - triV1), (point - triV1));
		cp2 = XMVector3Cross((triV3 - triV1), (triV2 - triV1));
		if (XMVectorGetX(XMVector3Dot(cp1, cp2)) >= 0)
		{
			cp1 = XMVector3Cross((triV2 - triV1), (point - triV1));
			cp2 = XMVector3Cross((triV2 - triV1), (triV3 - triV1));
			if (XMVectorGetX(XMVector3Dot(cp1, cp2)) >= 0)
			{
				return true;
			}
			else
				return false;
		}
		else
			return false;
	}
	return false;
}