////////////////////////////////////////////////////////////////////////////////
// Filename: lightclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "lightclass.h"


LightClass::LightClass()
{
	m_nowAmbientState = true;
	m_nowDiffuseState = true;
	m_nowSpecularState = true;
}


LightClass::LightClass(const LightClass& other)
{
}


LightClass::~LightClass()
{
}

void LightClass::SetAmbientColor(float red, float green, float blue, float alpha)
{
	m_ambientColor = XMFLOAT4(red, green, blue, alpha);
	return;
}

void LightClass::SetDiffuseColor(float red, float green, float blue, float alpha)
{
	m_diffuseColor = XMFLOAT4(red, green, blue, alpha);
	return;
}


void LightClass::SetDirection(float x, float y, float z)
{
	m_direction = XMFLOAT3(x, y, z);
	return;
}

void LightClass::SetSpecularColor(float red, float green, float blue, float alpha)
{
	m_specularColor = XMFLOAT4(red, green, blue, alpha);
	return;
}

void LightClass::SetSpecularPower(float power)
{
	m_specularPower = power;
	return;
}



void LightClass::SetAmbientState(BOOL nowState)
{
	m_nowAmbientState = nowState;
}

void LightClass::SetDiffuseState(BOOL nowState)
{
	m_nowDiffuseState = nowState;
}

void LightClass::SetSpecularState(BOOL nowState)
{
	m_nowSpecularState = nowState;
}




XMFLOAT4 LightClass::GetAmbientColor()
{
	return m_ambientColor;
}

XMFLOAT4 LightClass::GetDiffuseColor()
{
	return m_diffuseColor;
}


XMFLOAT3 LightClass::GetDirection()
{
	return m_direction;
}

XMFLOAT4 LightClass::GetSpecularColor()
{
	return m_specularColor;
}

float LightClass::GetSpecularPower()
{
	return m_specularPower;
}




BOOL LightClass::GetAmbientState()
{
	return m_nowAmbientState;
}

BOOL LightClass::GetDiffuseState()
{
	return m_nowDiffuseState;
}

BOOL LightClass::GetSpecularState()
{
	return m_nowSpecularState;
}






void LightClass::SetPointDiffuseColor(vector<XMFLOAT4> pointDiffuseColor)
{
	m_pointDiffuseColors = pointDiffuseColor;
	return;
}

void LightClass::SetPosition(vector<XMFLOAT4> positions)
{
	m_positions = positions;
}

vector<XMFLOAT4> LightClass::GetPointDiffuseColor()
{
	return m_pointDiffuseColors;
}

vector<XMFLOAT4> LightClass::GetPosition()
{
	return m_positions;
}