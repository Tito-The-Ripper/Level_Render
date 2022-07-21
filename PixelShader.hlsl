

struct OBJ_ATTRIBUTES
{
    float3 Kd; // diffuse reflectivity
    float d; // dissolve (transparency) 
    float3 Ks; // specular reflectivity
    float Ns; // specular exponent
    float3 Ka; // ambient reflectivity
    float sharpness; // local reflection map sharpness
    float3 Tf; // transmission filter
    float Ni; // optical density (index of refraction)
    float3 Ke; // emissive reflectivity
    uint illum; // illumination model
};

struct OUTPUT_TO_RASTERIZER
{
    float4 posH : SV_POSITION;
    float3 nrmW : NORMAL;
    float3 posW : WORLD;
};
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
    float4 passing[4];
};

struct MESH_DATA
{

    unsigned int Mesh_ID ;
    unsigned int Materials_ID ;
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







ConstantBuffer<SCENE_DATA> cameraAndLight : register(b0, Space0);
ConstantBuffer<MESH_DATA> meshInfo : register(b1, Space0);

StructuredBuffer<World> WorldMatrices : register(t0, Space0);
StructuredBuffer<Mats_Mesh> Materials : register(t1, Space0);
//StructuredBuffer<Lights> Lighting : register(t2, Space0);




// TODO: Part 4f
// TODO: Part 4b
float4 main(OUTPUT_TO_RASTERIZER input) : SV_TARGET
{
	// TODO: Part 3a

	// TODO: Part 4c
	// TODO: Part 4g

    float3 Light = normalize(cameraAndLight.DirectionalLight.sunDirection).xyz;
    float3 SurfacePos = normalize(input.nrmW).xyz;
    float4 Diffuse = float4(Materials[meshInfo.Materials_ID].material.Kd, 0);
    float4 Emissive = float4(Materials[meshInfo.Materials_ID].material.Ke, 0);
    float3 SurfaceNormal = normalize(input.nrmW);


    float3 LightingW = normalize(input.posW - Light);
    float3 view = normalize(cameraAndLight.cameraPos.xyz - input.posW);

    float3 reflectedlight = reflect(-(Light), SurfaceNormal);


    float intesity = pow(max(saturate(dot(reflectedlight, view)), 0), Materials[meshInfo.Materials_ID].material.sharpness);
    float LightRatio = saturate(dot(-cameraAndLight.DirectionalLight.sunDirection.xyz, SurfaceNormal));

    float4 totalReflected = cameraAndLight.DirectionalLight.sunColor * float4(Materials[meshInfo.Materials_ID].material.Ks, 0) * intesity;




    float totalDirect = LightRatio * cameraAndLight.DirectionalLight.sunColor;
    float totalIndirect = cameraAndLight.DirectionalLight.sunAmbient * float4(Materials[meshInfo.Materials_ID].material.Ka, 0);

    return saturate(totalDirect + totalIndirect) * Diffuse + totalReflected + Emissive;
    


}