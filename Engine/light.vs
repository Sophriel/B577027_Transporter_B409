////////////////////////////////////////////////////////////////////////////////
// Filename: light.vs
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

// The position of the camera is used for specular calculations
cbuffer CameraBuffer
{
    float3 cameraPosition;
	float padding;
};

cbuffer LightPositionBuffer 
{ 
	float4 lightPosition[4]; 
};


//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	//float3 viewDirection : TEXCOORD1;

	float3 lightPos1 : TEXCOORD1; 
	float3 lightPos2 : TEXCOORD2; 
	float3 lightPos3 : TEXCOORD3; 
	float3 lightPos4 : TEXCOORD4;

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

	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
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

	//  직사광선
    //// Determine the viewing direction based on the position of the camera and the position of the vertex in the world.
    //output.viewDirection = cameraPosition.xyz - worldPosition.xyz;
	
    //// Normalize the viewing direction vector.
    //output.viewDirection = normalize(output.viewDirection);
	//  직사광선 끝
	
	//  포인트라이트
	// Determine the light positions based on the position of the lights and the position of the vertex in the world. 
	output.lightPos1.xyz = lightPosition[0].xyz - worldPosition.xyz; 
	output.lightPos2.xyz = lightPosition[1].xyz - worldPosition.xyz; 
	output.lightPos3.xyz = lightPosition[2].xyz - worldPosition.xyz; 
	output.lightPos4.xyz = lightPosition[3].xyz - worldPosition.xyz; 

	// Normalize the light position vectors. 
	output.lightPos1 = normalize(output.lightPos1); 
	output.lightPos2 = normalize(output.lightPos2); 
	output.lightPos3 = normalize(output.lightPos3); 
	output.lightPos4 = normalize(output.lightPos4);


    return output;
}