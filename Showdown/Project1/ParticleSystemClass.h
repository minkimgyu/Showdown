#pragma once


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <directxmath.h>

using namespace DirectX;

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "textureclass.h"
#include <string>
#include <fstream>
#include <vector>
#include "ModelData.h"
#include "AlignedAllocationPolicy.h"

class TextureClass;

class ParticleSystemClass : public AlignedAllocationPolicy<16>
{
private:
	struct ParticleType
	{
		float positionX, positionY, positionZ;
		float dirX, dirY, dirZ;
		float red, green, blue;
		float velocity;
		float lifeTime;
		bool active;
	};

	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT4 color;
	};

public:
	ParticleSystemClass();
	ParticleSystemClass(float size, float speed, float DeviationX, float DeviationY, float DeviationZ);
	ParticleSystemClass(const ParticleSystemClass&);
	~ParticleSystemClass();

	bool Initialize(ID3D11Device*, const WCHAR*);
	void Shutdown();
	bool Frame(float, ID3D11DeviceContext*);
	void Render(ID3D11DeviceContext*);

	ID3D11ShaderResourceView* GetTexture();
	int GetIndexCount();

	void SetPosition(float x, float y, float z);

	void SetSpawn(bool);
	void SetSpawn(bool, XMVECTOR);

private:
	bool LoadTexture(ID3D11Device*, const WCHAR*);
	void ReleaseTexture();

	bool InitializeParticleSystem();
	void ShutdownParticleSystem();

	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();

	void EmitParticles(float);
	int ReturnDir();
	void UpdateParticles(float);
	void KillParticles();

	bool UpdateBuffers(ID3D11DeviceContext*);
	void RenderBuffers(ID3D11DeviceContext*);

private:
	XMVECTOR direction;

	bool resetDirection;

	bool nowSpawn;
	float spawnDuration;
	float storedSpawnDuration;

	float m_particleDeviationX = 0;
	float m_particleDeviationY = 0;
	float m_particleDeviationZ = 0;
	float m_particleVelocity = 0;
	float m_particleVelocityVariation = 0;
	float m_particleSize = 0;
	float m_particlesPerSecond = 0;
	int m_maxParticles = 0;

	int m_currentParticleCount = 0;
	float m_accumulatedTime = 0;

	XMFLOAT3 pos;

	TextureClass* m_Texture = nullptr;
	ParticleType* m_particleList = nullptr;
	int m_vertexCount = 0;
	int m_indexCount = 0;
	VertexType* m_vertices = nullptr;
	ID3D11Buffer* m_vertexBuffer = nullptr;
	ID3D11Buffer* m_indexBuffer = nullptr;
};