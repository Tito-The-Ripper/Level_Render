
#pragma pack_matrix(row_major)
// TODO: Part 2b

struct OBJ_ATTRIBUTES
{
	float3    Kd; // diffuse reflectivity
	float	    d; // dissolve (transparency) 
	float3    Ks; // specular reflectivity
	float       Ns; // specular exponent
	float3    Ka; // ambient reflectivity
	float       sharpness; // local reflection map sharpness
	float3    Tf; // transmission filter
	float       Ni; // optical density (index of refraction)
	float3    Ke; // emissive reflectivity
	uint   illum; // illumination model
};

struct OUTPUT_TO_RASTERIZER
{
	float4 posH : SV_POSITION;
	float3 nrmW : NORMAL;
	float3 posW : WORLD;
};

struct SCENE_DATA
{

	matrix viewMatrix;
	matrix projectionMatrix;
	float4 cameraPos;

	float4  passing[7];
};

struct MESH_DATA
{

	matrix worldMatrix;
	OBJ_ATTRIBUTES material;
	uint passing[28];
};

struct World
{
	matrix WorldMatrixs;
};

struct Mats
{
	OBJ_ATTRIBUTES material;
};


struct Lights
{
	float4 sunDirection;
	float4 sunColor;
	float4 sunAmbient;
	

};




ConstantBuffer<SCENE_DATA> camera : register(b0, Space0);
ConstantBuffer<MESH_DATA> meshInfo : register(b1, Space0);

StructuredBuffer <Mats> Materials: register(t1, Space0);
StructuredBuffer<World> WorldMatrices : register(t0, Space0);
//StructuredBuffer<Lights> Lighting;

// TODO: Part 1f



struct VS_Input
{
	float4 Pos : POSITION;
	float3 Tex : TEXTURE;
	float3 Norm : NORM;
};



// TODO: Part 4b
float4 main(VS_Input inputVertex) : SV_POSITION
{
	// TODO: Part 1h


	// TODO: Part 4b
	// TODO: Part 2i

	OUTPUT_TO_RASTERIZER Output = (OUTPUT_TO_RASTERIZER)0;
    
    matrix TEMP;
    for (int i = 0; i < 3; i++)
    {
		 TEMP = WorldMatrices[i].WorldMatrixs;
    }
       

    inputVertex.Pos = mul(inputVertex.Pos, TEMP);
	inputVertex.Pos = mul(inputVertex.Pos,camera.viewMatrix);
	inputVertex.Pos = mul(inputVertex.Pos,camera.projectionMatrix);

	Output.posH = inputVertex.Pos;
	//Output.nrmW = mul( float4( inputVertex.Norm, 1 ), meshInfo.worldMatrix).xyz;
	//Output.posW =  mul(inputVertex.Pos,meshInfo.worldMatrix).xyz;
	return inputVertex.Pos;

}