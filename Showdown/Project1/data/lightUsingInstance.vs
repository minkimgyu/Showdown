////////////////////////////////////////////////////////////////////////////////
// Filename: light.vs
////////////////////////////////////////////////////////////////////////////////

/////////////
// DEFINES //
/////////////
#define NUM_LIGHTS 6

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

cbuffer LightPositionBuffer
{
	float3 lightPosition[NUM_LIGHTS];
};

//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
    float3 instancePosition : TEXCOORD1;
    float3 instanceRotation : TEXCOORD2;
    float3 instanceScale : TEXCOORD3;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;

    float3 lightPos1 : TEXCOORD1;
    float3 lightPos2 : TEXCOORD2;
    float3 lightPos3 : TEXCOORD3;
    float3 lightPos4 : TEXCOORD4;
    float3 lightPos5 : TEXCOORD5;
    float3 lightPos6 : TEXCOORD6;

    float3 viewDirection : TEXCOORD7;
};

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType LightVertexShader(VertexInputType input)
{
    PixelInputType output;
    float4 worldPosition;

	// Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    //input.position.x += input.instancePosition.x;
    //input.position.y += input.instancePosition.y;
    //input.position.z += input.instancePosition.z;

    //input.position = mul(input.position, worldMatrix);


    //float angle = 180 * 0.0174532925f;

    matrix <float, 4, 4> translation =
    {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        input.instancePosition.x, input.instancePosition.y, input.instancePosition.z, 1.0f

    };

    matrix <float, 4, 4> rotationAroundX = {
                   1.0f, 0.0f, 0.0f, 0.0f,
                   0.0f, cos(input.instanceRotation.x), sin(input.instanceRotation.x), 0.0f,
                   0.0f, -sin(input.instanceRotation.x), cos(input.instanceRotation.x), 0.0f,
                   0.0f, 0.0f, 0.0f, 1.0f   
                               };

     matrix <float, 4, 4> rotationAroundY = {
                   cos(input.instanceRotation.y), 0.0f, -sin(input.instanceRotation.y), 0.0f,
                   0.0f, 1.0f, 0.0f, 0.0f,
                   sin(input.instanceRotation.y), 0.0f, cos(input.instanceRotation.y), 0.0f,
                   0.0f, 0.0f, 0.0f, 1.0f   
                               };

     matrix <float, 4, 4> rotationAroundZ = {
                   cos(input.instanceRotation.z), sin(input.instanceRotation.z), 0.0f, 0.0f,
                   -sin(input.instanceRotation.z), cos(input.instanceRotation.z), 0.0f, 0.0f,
                   0.0f, 0.0f, 1.0f, 0.0f,
                   0.0f, 0.0f, 0.0f, 1.0f   
                               };

     matrix <float, 4, 4> scaling = {
                   input.instanceScale.x, 0.0f, 0.0f, 0.0f,
                   0.0f, input.instanceScale.y, 0.0f, 0.0f,
                   0.0f, 0.0f, input.instanceScale.z, 0.0f,
                   0.0f, 0.0f, 0.0f, 1.0f   
                               };

    matrix <float, 4, 4> composition;

    //i compose together the rotation around y and the translation, the order is important!
    composition = mul(rotationAroundX, translation);
    composition = mul(rotationAroundY, composition);
    composition = mul(rotationAroundZ, composition);

    composition = mul(scaling, composition);



    //i apply the transformation to the vertex input.position
    output.position = mul(input.position,composition);

	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(output.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;
    
	// Calculate the normal vector against the world matrix only.
    output.normal = mul(input.normal, (float3x3)worldMatrix);
	
    // Normalize the normal vector.
    output.normal = normalize(output.normal);

    // Calculate the position of the vertex in the world.
    worldPosition = mul(input.position, worldMatrix);

    // Determine the viewing direction based on the position of the camera and the position of the vertex in the world.
    output.viewDirection = cameraPosition.xyz - worldPosition.xyz;
	
    // Normalize the viewing direction vector.
    output.viewDirection = normalize(output.viewDirection);




    // ------ point light ------ //

     // Calculate the position of the vertex in the world.
    worldPosition = mul(input.position, worldMatrix);

    // Determine the light positions based on the position of the lights and the position of the vertex in the world.
    output.lightPos1.xyz = lightPosition[0].xyz - worldPosition.xyz;
    output.lightPos2.xyz = lightPosition[1].xyz - worldPosition.xyz;
    output.lightPos3.xyz = lightPosition[2].xyz - worldPosition.xyz;
    output.lightPos4.xyz = lightPosition[3].xyz - worldPosition.xyz;
    output.lightPos5.xyz = lightPosition[4].xyz - worldPosition.xyz;
    output.lightPos6.xyz = lightPosition[5].xyz - worldPosition.xyz;

    // Normalize the light position vectors.
    output.lightPos1 = normalize(output.lightPos1);
    output.lightPos2 = normalize(output.lightPos2);
    output.lightPos3 = normalize(output.lightPos3);
    output.lightPos4 = normalize(output.lightPos4);
    output.lightPos5 = normalize(output.lightPos5);
    output.lightPos6 = normalize(output.lightPos6);

    return output;
}