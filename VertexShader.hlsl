
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

//struct OUTPUT_TO_RASTERIZER
//{
//	float4 posH : SV_POSITION;
//	float3 nrmW : NORMAL;
//	float3 posW : WORLD;
//};
struct Lights
{
	
	float4 sunDirection;
	float4 sunColor;
	float4 sunAmbient;

};

struct SCENE_DATA
{
    Lights DirectionalLight;
	matrix viewMatrix;
	matrix projectionMatrix;
	float4 cameraPos;
	float4  passing[4];
};

struct MESH_DATA
{


    uint MatID;
    uint MeshID;
    unsigned int padding[62];
};

struct World
{
	matrix WorldMatrixs;
};

struct Mats_Mesh
{
	OBJ_ATTRIBUTES material;
};







ConstantBuffer<SCENE_DATA> camera : register(b0, Space0);
ConstantBuffer<MESH_DATA> meshInfo : register(b1, Space0);

StructuredBuffer<World> WorldMatrices : register(t0, Space0);
StructuredBuffer<Mats_Mesh> Materials : register(t1, Space0);
//StructuredBuffer<Lights> Lighting : register(t2, Space0);

// TODO: Part 1f



struct VS_Input
{
	float3 Pos : POSITION;
	float3 Tex : TEXTURE;
	float3 Norm : NORM;
    uint instamceID : SV_InstanceID;
};



// TODO: Part 4b
float4 main(VS_Input inputVertex) : SV_POSITION
{
	// TODO: Part 1h


	// TODO: Part 4b
	// TODO: Part 2i

	//OUTPUT_TO_RASTERIZER Output = (OUTPUT_TO_RASTERIZER)0;
    float4 Temp = float4(inputVertex.Pos, 1);
    Temp = mul(Temp, WorldMatrices[meshInfo.MeshID].WorldMatrixs);
    Temp = mul(Temp, camera.viewMatrix);
    Temp = mul(Temp, camera.projectionMatrix);

	//Output.posH = inputVertex.Pos;
	//Output.nrmW = mul( float4( inputVertex.Norm, 1 ), meshInfo.worldMatrix).xyz;
	//Output.posW =  mul(inputVertex.Pos,meshInfo.worldMatrix).xyz;
    return Temp;
    

}