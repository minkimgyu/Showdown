#pragma once
#include "cameraclass.h"
#include "interactablemodelclass.h"

class Car : public InteractableModelClass
{
private:
	vector<XMFLOAT3> points;
	vector<XMFLOAT3> rotations;
	float t;
	float speed;
	int nowIndex;

public:
	Car(vector<XMFLOAT3>, vector<XMFLOAT3>, float);
	~Car();
	
	virtual void Frame(float time) override;

	void LerpBetweenPoints();
};