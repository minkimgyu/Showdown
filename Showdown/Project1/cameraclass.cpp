////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "cameraclass.h"


CameraClass::CameraClass()
{
	m_camTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	m_camUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	//Set the View matrix
	m_camView = XMMatrixLookAtLH(m_camPosition, m_camTarget, m_camUp);

	//Set the Projection matrix
	m_camProjection = XMMatrixPerspectiveFovLH(0.4f * 3.14f, (float)800 / (float)600, 1.0f, 1000.0f);
}


CameraClass::CameraClass(const CameraClass& other)
{
}


CameraClass::~CameraClass()
{
}


void CameraClass::SetPosition(float x, float y, float z)
{
	m_camPosition = XMVectorSet(x, y, z, 0.0f);
}

void CameraClass::AddPosition(XMVECTOR vector)
{
	m_camPosition += vector;
}

void CameraClass::SetRotation(float, float, float)
{

}

XMFLOAT3 CameraClass::GetPosition()
{
	XMFLOAT3 tmp;
	XMStoreFloat3(&tmp, m_camPosition);

	return tmp;
}

XMVECTOR CameraClass::GetPositionVector()
{
	return m_camPosition;
}

// This uses the position and rotation of the camera to build and to update the view matrix.
void CameraClass::Render()
{
	m_camRotationMatrix = XMMatrixRotationRollPitchYaw(m_camPitch, m_camYaw, 0);

	m_camTarget = XMVector3TransformCoord(m_DefaultForward, m_camRotationMatrix); // 카메라 Y축 회전으로 움직일 방향을 지정해줌
	m_camTarget = XMVector3Normalize(m_camTarget); // 이후 nomalize 진행

	XMMATRIX RotateYTempMatrix;
	RotateYTempMatrix = XMMatrixRotationY(m_camYaw); // 위의 변수를 이용해서 돌림

	m_camRight = XMVector3TransformCoord(m_DefaultRight, RotateYTempMatrix); // 카메라 오른쪽 백터
	m_camUp = XMVector3TransformCoord(m_camUp, RotateYTempMatrix); //카메라 기준 위 백터
	m_camForward = XMVector3TransformCoord(m_DefaultForward, RotateYTempMatrix); // 카메라 앞 백터

	m_camPosition += m_moveLeftRight * m_camRight; // 위 내용을 바탕으로 카메라 위치 재지정
	m_camPosition += m_moveBackForward * m_camForward;

	m_moveLeftRight = 0.0f;
	m_moveBackForward = 0.0f;

	m_camTarget = m_camPosition + m_camTarget; // 최종적으로 camTarget을 지정해줌
	m_camView = XMMatrixLookAtLH(m_camPosition, m_camTarget, m_camUp);

	return;
}

void CameraClass::GetViewMatrix(XMMATRIX& viewMatrix)
{
	viewMatrix = m_camView;
}

XMMATRIX CameraClass::ReturnViewMatrix()
{
	return m_camView;
}

XMMATRIX CameraClass::ReturnProjectionMatrix()
{
	return m_camProjection;
}

XMMATRIX CameraClass::ReturnRotationMatrix()
{
	return m_camRotationMatrix;
}

float CameraClass::ReturnCamYaw()
{
	return m_camYaw;
}

float CameraClass::ReturnCamPitch()
{
	return m_camPitch;
}

void CameraClass::MoveHorizontal(bool isLeft)
{
	if (m_canMove == false) return;

	if (isLeft == true)
		m_moveLeftRight -= m_speed;
	else
		m_moveLeftRight += m_speed;
}

void CameraClass::MoveVertical(bool isUp)
{
	if (m_canMove == false) return;

	if (isUp == true)
		m_moveBackForward += m_speed;
	else
		m_moveBackForward -= m_speed;
}

void CameraClass::MoveMouse(DIMOUSESTATE mouseCurrState, DIMOUSESTATE mouseLastState)
{
	m_camYaw += mouseLastState.lX * 0.01f;
	m_camPitch += mouseCurrState.lY * 0.01f;
}

void CameraClass::SetCanMove(bool move)
{
	m_canMove = move;
}

bool CameraClass::GetCanMove()
{
	return m_canMove;
}
