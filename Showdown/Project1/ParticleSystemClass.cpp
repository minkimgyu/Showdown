#include "stdafx.h"
#include "TextureClass.h"
#include "particlesystemclass.h"


ParticleSystemClass::ParticleSystemClass()
{
	spawnDuration = 500.0f;
	nowSpawn = false;

	// ��ƼŬ�� ������ ũ�⸦ �����մϴ�.
	m_particleSize = 0.25f;
	m_particleVelocity = 0.5f;

	m_particleDeviationX = 0.1f;
	m_particleDeviationY = 0.1f;
	m_particleDeviationZ = 0.1f;
}


ParticleSystemClass::ParticleSystemClass(float size, float speed, float DeviationX, float DeviationY, float DeviationZ)
{
	spawnDuration = 500.0f;
	nowSpawn = false;

	// ��ƼŬ�� ������ ũ�⸦ �����մϴ�.
	m_particleSize = size;
	m_particleVelocity = speed;

	m_particleDeviationX = DeviationX;
	m_particleDeviationY = DeviationY;
	m_particleDeviationZ = DeviationZ;
}

ParticleSystemClass::ParticleSystemClass(const ParticleSystemClass& other)
{
}


ParticleSystemClass::~ParticleSystemClass()
{
}


bool ParticleSystemClass::Initialize(ID3D11Device* device, const WCHAR* textureFilename)
{
	// ��ƼŬ�� ���Ǵ� �ؽ�ó�� �ε��մϴ�.
	if(!LoadTexture(device, textureFilename))
	{
		return false;
	}

	// ��ƼŬ �ý����� �ʱ�ȭ�մϴ�.
	if(!InitializeParticleSystem())
	{
		return false;
	}

	// ��ƼŬ�� �������ϴ� �� ����� ���۸� ����ϴ�.
	if(!InitializeBuffers(device))
	{
		return false;
	}

	return true;
}


void ParticleSystemClass::Shutdown()
{
	// ���۸� �����մϴ�.
	ShutdownBuffers();

	// ��ƼŬ �ý����� �����մϴ�.
	ShutdownParticleSystem();

	// ��ƼŬ�� ���� �ؽ�ó�� �����մϴ�.
	ReleaseTexture();
}


bool ParticleSystemClass::Frame(float frameTime, ID3D11DeviceContext* deviceContext)
{
	if (spawnDuration < storedSpawnDuration)
	{
		nowSpawn = false;
	}

	// ������ ��ƼŬ�� �����մϴ�.
	KillParticles();

	// �� ��ƼŬ�� �����մϴ�.
	EmitParticles(frameTime);
	
	// ��ƼŬ ��ġ�� ������Ʈ �մϴ�.
	UpdateParticles(frameTime);

	// ���� ���� ���۸� �� ��ƼŬ�� �� ��ġ�� ������Ʈ�մϴ�.
	return UpdateBuffers(deviceContext);
}


void ParticleSystemClass::Render(ID3D11DeviceContext* deviceContext)
{
	// ���⸦ �غ��ϱ� ���� �׷��� ������ ���ο� �������� �ε��� ���۸� �����ϴ�.
	RenderBuffers(deviceContext);
}


ID3D11ShaderResourceView* ParticleSystemClass::GetTexture()
{
	return m_Texture->GetTexture();
}


int ParticleSystemClass::GetIndexCount()
{
	return m_indexCount;
}

void ParticleSystemClass::SetPosition(float x, float y, float z)
{
	pos = XMFLOAT3(x, y, z);
}

void ParticleSystemClass::SetSpawn(bool _nowSpawn)
{
	nowSpawn = _nowSpawn;
	storedSpawnDuration = 0;
	resetDirection = false;
}

void ParticleSystemClass::SetSpawn(bool _nowSpawn, XMVECTOR dir)
{
	nowSpawn = _nowSpawn;
	storedSpawnDuration = 0;
	direction = dir;
	resetDirection = true;
}

bool ParticleSystemClass::LoadTexture(ID3D11Device* device, const WCHAR* filename)
{
	// �ؽ�ó ������Ʈ�� �����Ѵ�.
	m_Texture = new TextureClass;
	if(!m_Texture)
	{
		return false;
	}

	// �ؽ�ó ������Ʈ�� �ʱ�ȭ�Ѵ�.
	return m_Texture->Initialize(device, filename);
}


void ParticleSystemClass::ReleaseTexture()
{
	// �ؽ�ó ������Ʈ�� �������Ѵ�.
	if(m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}
}


bool ParticleSystemClass::InitializeParticleSystem()
{
	// ��ƼŬ�� �ӵ��� �ӵ� ��ȭ�� �����մϴ�.
	
	m_particleVelocityVariation = 0.5f;

	

	// �ʴ� ���� �� ��ƼŬ ���� �����մϴ�.
	m_particlesPerSecond = 300.0f;

	// ��ƼŬ �ý��ۿ� ���Ǵ� �ִ� ��ƼŬ ���� �����մϴ�.
	m_maxParticles = 1000;

	// ��ƼŬ ����Ʈ�� �����մϴ�.
	m_particleList = new ParticleType[m_maxParticles];
	if(!m_particleList)
	{
		return false;
	}

	// ��ƼŬ ����Ʈ�� �ʱ�ȭ �մϴ�.
	for(int i=0; i<m_maxParticles; i++)
	{
		m_particleList[i].active = false;
	}

	// ���� ������� �����Ƿ� ���� ��ƼŬ ���� 0���� �ʱ�ȭ�մϴ�.
	m_currentParticleCount = 0;

	// �ʴ� ��ƼŬ ���� �ӵ��� �ʱ� ���� �ð��� ����ϴ�.
	m_accumulatedTime = 0.0f;

	return true;
}


void ParticleSystemClass::ShutdownParticleSystem()
{
	// ��ƼŬ ����� �����մϴ�.
	if(m_particleList)
	{
		delete [] m_particleList;
		m_particleList = 0;
	}
}


bool ParticleSystemClass::InitializeBuffers(ID3D11Device* device)
{
	// ���� �迭�� �ִ� ���� ���� �����մϴ�.
	m_vertexCount = m_maxParticles * 6;

	// �ε��� �迭�� �ִ� �ε��� ���� �����մϴ�.
	m_indexCount = m_vertexCount;

	// ������ �� ���ڿ� ���� ���� �迭�� ����ϴ�.
	m_vertices = new VertexType[m_vertexCount];
	if(!m_vertices)
	{
		return false;
	}

	// �ε��� �迭�� ����ϴ�.
	unsigned long* indices = new unsigned long[m_indexCount];
	if(!indices)
	{
		return false;
	}

	// ó������ ���� �迭�� 0���� �ʱ�ȭ�մϴ�.
	memset(m_vertices, 0, (sizeof(VertexType) * m_vertexCount));

	// �ε��� �迭�� �ʱ�ȭ�մϴ�.
	for(int i=0; i<m_indexCount; i++)
	{
		indices[i] = i;
	}

	// ���� ���� ������ ������ �����Ѵ�.
	D3D11_BUFFER_DESC vertexBufferDesc;
    vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// subresource ������ ���� �����Ϳ� ���� �����͸� �����մϴ�.
	D3D11_SUBRESOURCE_DATA vertexData;
    vertexData.pSysMem = m_vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// ���� ��ħ�� ���� ���۸� ����ϴ�.
	if(FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer)))
	{
		return false;
	}

	// ���� �ε��� ������ ������ �����մϴ�.
	D3D11_BUFFER_DESC indexBufferDesc;
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// ���� ���ҽ� ������ �ε��� �����Ϳ� ���� �����͸� �����մϴ�.
	D3D11_SUBRESOURCE_DATA indexData;
    indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// �ε��� ���۸� ����ϴ�.
	if(FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer)))
	{
		return false;
	}

	// �� �̻� �ʿ����� �����Ƿ� �ε��� �迭�� �����մϴ�.
	delete [] indices;
	indices = 0;

	return true;
}


void ParticleSystemClass::ShutdownBuffers()
{
	// �ε��� ���۸� �����մϴ�.
	if(m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// ���ؽ� ���۸� �����Ѵ�.
	if(m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}
}


void ParticleSystemClass::EmitParticles(float frameTime)
{
	if (nowSpawn == false) return;
	

	storedSpawnDuration += frameTime;

	// ������ �ð��� ������ŵ�ϴ�.
	m_accumulatedTime += frameTime;

	// ���� ��ƼŬ ����� false�� �����մϴ�.
	bool emitParticle = false;
	
	// �� ��ƼŬ�� ������ �ð����� Ȯ���մϴ�.
	if(m_accumulatedTime > (1000.0f / m_particlesPerSecond))
	{
		m_accumulatedTime = 0.0f;
		emitParticle = true;
	}

	// ���� �� ��ƼŬ�� ������ ������ �� �ϳ��� �����մϴ�.
	if((emitParticle == true) && (m_currentParticleCount < (m_maxParticles - 1)))
	{
		m_currentParticleCount++;

		// ���� ���� ȭ �� ��ƼŬ �Ӽ��� �����մϴ�.
		float positionX = (((float)rand() - (float)rand()) / RAND_MAX) * m_particleDeviationX;// +pos.x;
		float positionY = (((float)rand() - (float)rand()) / RAND_MAX) * m_particleDeviationY;// +pos.y;
		float positionZ = (((float)rand() - (float)rand()) / RAND_MAX) * m_particleDeviationZ;// +pos.z;

		float velocity = m_particleVelocity + (((float)rand()-(float)rand())/RAND_MAX) * m_particleVelocityVariation;

		float red   = (((float)rand()-(float)rand())/RAND_MAX) + 0.5f;
		float green = (((float)rand()-(float)rand())/RAND_MAX) + 0.5f;
		float blue  = (((float)rand()-(float)rand())/RAND_MAX) + 0.5f;

		// ���� ������ ���� ��ƼŬ�� �ڿ��� ������ �������ؾ��ϹǷ� ��ƼŬ �迭�� �����ؾ� �մϴ�.
		// Z ���̸� ����Ͽ� �����ϹǷ� ��Ͽ��� ��ƼŬ�� �����ؾ� �ϴ� ��ġ�� ã�ƾ��մϴ�.
		int index = 0;
		bool found = false;
		while(!found)
		{
			if((m_particleList[index].active == false) || (m_particleList[index].positionZ < positionZ))
			{
				found = true;
			}
			else
			{
				index++;
			}
		}

		// ���� �� ��ġ�� �˾����Ƿ� �ε������� �� ��ġ �� �迭�� �����Ͽ� �� ��ƼŬ�� ���� ������ �������մϴ�.
		int i = m_currentParticleCount;
		int j = i - 1;

		while(i != index)
		{
			m_particleList[i].positionX = m_particleList[j].positionX;
			m_particleList[i].positionY = m_particleList[j].positionY;
			m_particleList[i].positionZ = m_particleList[j].positionZ;
			m_particleList[i].red       = m_particleList[j].red;
			m_particleList[i].green     = m_particleList[j].green;
			m_particleList[i].blue      = m_particleList[j].blue;
			m_particleList[i].velocity  = m_particleList[j].velocity;
			m_particleList[i].active    = m_particleList[j].active;
			i--;
			j--;
		}

		// ���� ��Ȯ�� ���� ������ ��ƼŬ �迭�� �����Ͻʽÿ�.
		m_particleList[index].positionX = positionX;
		m_particleList[index].positionY = positionY;
		m_particleList[index].positionZ = positionZ;

		if (resetDirection)
		{
			XMFLOAT3 tmp;
			XMStoreFloat3(&tmp, direction);

			m_particleList[index].dirX = tmp.x;
			m_particleList[index].dirY = tmp.y;
			m_particleList[index].dirZ = tmp.z;
		}
		else
		{
			m_particleList[index].dirX = (float)ReturnDir();
			m_particleList[index].dirY = 1;
			m_particleList[index].dirZ = (float)ReturnDir();
		}

		m_particleList[index].lifeTime = 0;

		m_particleList[index].red       = red;
		m_particleList[index].green     = green;
		m_particleList[index].blue      = blue;
		m_particleList[index].velocity  = velocity;
		m_particleList[index].active    = true;
	}
}

int ParticleSystemClass::ReturnDir()
{
	int tmp = rand() % 2;
	if (tmp == 0) tmp = -1;
	else tmp = 1;

	return tmp;
}

void ParticleSystemClass::UpdateParticles(float frameTime)
{
	// �� �������� ��ġ, �ӵ� �� ������ �ð��� ����Ͽ� �Ʒ������� �̵��Ͽ� ��� ��ƼŬ�� ������Ʈ�մϴ�.
	for(int i=0; i<m_currentParticleCount; i++)
	{
		m_particleList[i].lifeTime += frameTime;

		m_particleList[i].positionX = m_particleList[i].positionX + (m_particleList[i].velocity * (frameTime * 0.005f) * m_particleList[i].dirX);
		m_particleList[i].positionY = m_particleList[i].positionY + (m_particleList[i].velocity * (frameTime * 0.005f) * m_particleList[i].dirY);
		m_particleList[i].positionZ = m_particleList[i].positionZ + (m_particleList[i].velocity * (frameTime * 0.005f) * m_particleList[i].dirZ);
	}
}


void ParticleSystemClass::KillParticles()
{
	// Ư�� ���� ������ ��� ��� ��ƼŬ�� �����մϴ�.
	for(int i=0; i<m_maxParticles; i++)
	{
		if((m_particleList[i].active == true) && (m_particleList[i].positionY < -2.0f || m_particleList[i].positionY > 3.0f || m_particleList[i].lifeTime >= 400))
		{
			m_particleList[i].active = false;
			m_particleList[i].lifeTime = 0;
			m_currentParticleCount--;

			// ���� ��� ����ִ� ��ƼŬ�� �迭���� �̵����� �ı��� ��ƼŬ�� ����� �迭�� �ùٸ��� ���ĵ� ���·� �����մϴ�.
			for(int j=i; j<m_maxParticles-1; j++)
			{
				m_particleList[j].positionX = m_particleList[j+1].positionX;
				m_particleList[j].positionY = m_particleList[j+1].positionY;
				m_particleList[j].positionZ = m_particleList[j+1].positionZ;
				m_particleList[j].red       = m_particleList[j+1].red;
				m_particleList[j].green     = m_particleList[j+1].green;
				m_particleList[j].blue      = m_particleList[j+1].blue;
				m_particleList[j].velocity  = m_particleList[j+1].velocity;
				m_particleList[j].active    = m_particleList[j+1].active;
			}
		}
	}
}


bool ParticleSystemClass::UpdateBuffers(ID3D11DeviceContext* deviceContext)
{
	// ó������ ���� �迭�� 0���� �ʱ�ȭ�մϴ�.
	VertexType* verticesPtr;
	memset(m_vertices, 0, (sizeof(VertexType) * m_vertexCount));

	// ���� ��ƼŬ ��� �迭���� ���� �迭�� ����ϴ�. �� ��ƼŬ�� �� ���� �ﰢ������ ������� �����Դϴ�.
	int index = 0;

	for(int i=0; i<m_currentParticleCount; i++)
	{
		// ���� �Ʒ�.
		m_vertices[index].position = XMFLOAT3(m_particleList[i].positionX - m_particleSize, m_particleList[i].positionY - m_particleSize, m_particleList[i].positionZ);
		m_vertices[index].texture = XMFLOAT2(0.0f, 1.0f);
		m_vertices[index].color = XMFLOAT4(m_particleList[i].red, m_particleList[i].green, m_particleList[i].blue, 1.0f);
		index++;

		// ���� ��.
		m_vertices[index].position = XMFLOAT3(m_particleList[i].positionX - m_particleSize, m_particleList[i].positionY + m_particleSize, m_particleList[i].positionZ);
		m_vertices[index].texture = XMFLOAT2(0.0f, 0.0f);
		m_vertices[index].color = XMFLOAT4(m_particleList[i].red, m_particleList[i].green, m_particleList[i].blue, 1.0f);
		index++;

		// ������ �Ʒ�.
		m_vertices[index].position = XMFLOAT3(m_particleList[i].positionX + m_particleSize, m_particleList[i].positionY - m_particleSize, m_particleList[i].positionZ);
		m_vertices[index].texture = XMFLOAT2(1.0f, 1.0f);
		m_vertices[index].color = XMFLOAT4(m_particleList[i].red, m_particleList[i].green, m_particleList[i].blue, 1.0f);
		index++;

		// ������ �Ʒ�.
		m_vertices[index].position = XMFLOAT3(m_particleList[i].positionX + m_particleSize, m_particleList[i].positionY - m_particleSize, m_particleList[i].positionZ);
		m_vertices[index].texture = XMFLOAT2(1.0f, 1.0f);
		m_vertices[index].color = XMFLOAT4(m_particleList[i].red, m_particleList[i].green, m_particleList[i].blue, 1.0f);
		index++;

		// ���� ��.
		m_vertices[index].position = XMFLOAT3(m_particleList[i].positionX - m_particleSize, m_particleList[i].positionY + m_particleSize, m_particleList[i].positionZ);
		m_vertices[index].texture = XMFLOAT2(0.0f, 0.0f);
		m_vertices[index].color = XMFLOAT4(m_particleList[i].red, m_particleList[i].green, m_particleList[i].blue, 1.0f);
		index++;

		// ������ ��.
		m_vertices[index].position = XMFLOAT3(m_particleList[i].positionX + m_particleSize, m_particleList[i].positionY + m_particleSize, m_particleList[i].positionZ);
		m_vertices[index].texture = XMFLOAT2(1.0f, 0.0f);
		m_vertices[index].color = XMFLOAT4(m_particleList[i].red, m_particleList[i].green, m_particleList[i].blue, 1.0f);
		index++;
	}
	
	// ���� ���۸� ��޴ϴ�.
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	if(FAILED(deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
	{
		return false;
	}

	// ���� ������ �����͸� ����Ű�� �����͸� ��´�.
	verticesPtr = (VertexType*)mappedResource.pData;

	// �����͸� ���� ���ۿ� �����մϴ�.
	memcpy(verticesPtr, (void*)m_vertices, (sizeof(VertexType) * m_vertexCount));

	// ���� ������ ����� �����մϴ�.
	deviceContext->Unmap(m_vertexBuffer, 0);

	return true;
}


void ParticleSystemClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	// ���� ���� ���� �� �������� �����մϴ�.
    unsigned int stride = sizeof(VertexType); 
	unsigned int offset = 0;
    
	// ������ �� �� �ֵ��� �Է� ��������� ���� ���۸� Ȱ������ �����մϴ�.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

    // ������ �� �� �ֵ��� �Է� ��������� �ε��� ���۸� Ȱ������ �����մϴ�.
    deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // �� ���� ���ۿ��� ������ �Ǿ�� �ϴ� ������Ƽ�� ������ �����մϴ�.
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}