////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_


//////////////
// INCLUDES //
//////////////
#include <directxmath.h>
#include <dinput.h>

#include "AlignedAllocationPolicy.h"

using namespace DirectX;

////////////////////////////////////////////////////////////////////////////////
// Class name: CameraClass
////////////////////////////////////////////////////////////////////////////////
class CameraClass : public AlignedAllocationPolicy<16>
{
public:
	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();

	void SetPosition(float, float, float);
	void AddPosition(XMVECTOR vector);
	void SetRotation(float, float, float);
	XMFLOAT3 GetPosition();
	XMVECTOR GetPositionVector();

	void Render();
	void GetViewMatrix(XMMATRIX&);

	void MoveHorizontal(bool);
	void MoveVertical(bool);
	void MoveMouse(DIMOUSESTATE, DIMOUSESTATE);

	void SetCanMove(bool move);
	bool GetCanMove();

	XMMATRIX ReturnViewMatrix();
	XMMATRIX ReturnProjectionMatrix();
	XMMATRIX ReturnRotationMatrix();

	float ReturnCamYaw();
	float ReturnCamPitch();

private:
	/*XMFLOAT3 m_position;
	XMFLOAT3 m_rotation;
	XMMATRIX m_viewMatrix;*/
	float m_speed = 0.01f;

	bool m_canMove = true;

	XMVECTOR m_camPosition;
	XMVECTOR m_camTarget;
	XMVECTOR m_camUp;

	XMVECTOR m_DefaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR m_DefaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR m_camForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR m_camRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	XMMATRIX m_camView;
	XMMATRIX m_camProjection;

	XMMATRIX m_camRotationMatrix;
	XMMATRIX m_groundWorld;

	float m_moveLeftRight = 0.0f; // 속도를 지정하기 위해 필요함
	float m_moveBackForward = 0.0f;

	float m_camYaw = 0.0f;
	float m_camPitch = 0.0f;
};

#endif