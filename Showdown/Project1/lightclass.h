////////////////////////////////////////////////////////////////////////////////
// Filename: lightclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _LIGHTCLASS_H_
#define _LIGHTCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <directxmath.h>
#include <d3d11.h>
#include <vector>

using namespace std;

using namespace DirectX;

////////////////////////////////////////////////////////////////////////////////
// Class name: LightClass
////////////////////////////////////////////////////////////////////////////////
class LightClass
{
public:
	LightClass();
	LightClass(const LightClass&);
	~LightClass();

	void SetAmbientColor(float, float, float, float);
	void SetDiffuseColor(float, float, float, float);
	void SetDirection(float, float, float);

	void SetSpecularColor(float, float, float, float);
	void SetSpecularPower(float);


	void SetAmbientState(BOOL);
	void SetDiffuseState(BOOL);
	void SetSpecularState(BOOL);


	void SetPointDiffuseColor(vector<XMFLOAT4>);
	void SetPosition(vector<XMFLOAT4>);

	XMFLOAT4 GetAmbientColor();
	XMFLOAT4 GetDiffuseColor();
	XMFLOAT3 GetDirection();

	XMFLOAT4 GetSpecularColor();
	float GetSpecularPower();



	BOOL GetAmbientState();
	BOOL GetDiffuseState();
	BOOL GetSpecularState();


	vector<XMFLOAT4> GetPointDiffuseColor();
	vector<XMFLOAT4> GetPosition();

private:
	XMFLOAT4 m_ambientColor;
	XMFLOAT4 m_diffuseColor;
	XMFLOAT3 m_direction;

	XMFLOAT4 m_specularColor;
	float m_specularPower;

	BOOL m_nowAmbientState;
	BOOL m_nowDiffuseState;
	BOOL m_nowSpecularState;

	vector<XMFLOAT4> m_pointDiffuseColors;
	vector<XMFLOAT4> m_positions;
};

#endif