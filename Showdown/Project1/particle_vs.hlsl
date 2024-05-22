////////////////////////////////////////////////////////////////////////////////
// Filename: particle_vs.hlsl
////////////////////////////////////////////////////////////////////////////////

#define XM_PI 3.14159265358
#define EULER 0.0174532925

/////////////
// GLOBALS //
/////////////
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer CameraBuffer
{
    float3 cameraPosition;
    float padding;
};

//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
	float4 color : COLOR;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float4 color : COLOR;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType ParticleVertexShader(VertexInputType input)
{
    PixelInputType output;

	// 적절한 행렬 계산을 위해 위치 벡터를 4 단위로 변경합니다.
    input.position.w = 1.0f;

    ////matrix <float, 4, 4> composition;

    //// 아크 탄젠트 함수를 사용하여 현재 카메라 위치를 향하도록 빌보드 모델에 적용해야하는 회전을 계산합니다.
    //double angle = atan2(input.position.x - cameraPosition.x, input.position.z - cameraPosition.z) * (180.0 / XM_PI);

    //float rotation = (float)angle * EULER;

    //matrix <float, 4, 4> rotationAroundY = {
    //           cos(rotation), 0.0f, -sin(rotation), 0.0f,
    //           0.0f, 1.0f, 0.0f, 0.0f,
    //           sin(rotation), 0.0f, cos(rotation), 0.0f,
    //           0.0f, 0.0f, 0.0f, 1.0f
    //};


    //// 회전을 라디안으로 변환합니다.

    ////composition = mul(rotationAroundY, input.position);

    //i apply the transformation to the vertex input.position
    //output.position = mul(input.position, rotationAroundY);

    //i apply the transformation to the vertex input.position

    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
	// 픽셀 쉐이더의 텍스처 좌표를 저장한다.
	output.tex = input.tex;



    
	// 픽셀 쉐이더의 입자 색상을 저장합니다. 
    output.color = input.color;

    return output;
}