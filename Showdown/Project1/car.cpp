#include "car.h"

Car::Car(vector<XMFLOAT3> position, vector<XMFLOAT3> rotation, float carSpeed)
{
	points = position;
	rotations = rotation;
	speed = carSpeed;

	t = 0;
	nowIndex = 0;

	SetMove(true);
}

Car::~Car()
{

}

void Car::Frame(float time)
{
	InteractableModelClass::Frame(time);
	LerpBetweenPoints();
}

void Car::LerpBetweenPoints()
{
	t += 0.001f * speed;

	int start = nowIndex;
	int end;

	end = start + 1;
	if (points.size() == end) end = 0;

	XMVECTOR startVec = XMLoadFloat3(&points[start]);
	XMVECTOR endtVec = XMLoadFloat3(&points[end]);

	XMFLOAT3 pos;

	XMStoreFloat3(&pos, XMVectorLerp(startVec, endtVec, t));

	m_modelData.SetPos(pos);

	if (t >= 1)
	{
		nowIndex += 1;
		if (points.size() == nowIndex) 
			nowIndex = 0;

		m_modelData.SetRotation(rotations[nowIndex]);

		t = 0;
	}
}