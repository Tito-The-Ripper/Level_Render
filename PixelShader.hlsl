

// an ultra simple hlsl pixel shader
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

    uint MeshID;
    uint MatsID;
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

//StructuredBuffer <Mats> : register(t0,Space0);
//StructuredBuffer <World> meshInfo : register(s0,Space0);
//StructuredBuffer<Lights> cameraAndLights : register(s1,Space0);



// TODO: Part 4f
// TODO: Part 4b
float4 main(OUTPUT_TO_RASTERIZER input) : SV_TARGET
{
	// TODO: Part 3a

	// TODO: Part 4c
	// TODO: Part 4g

	/*float3 Light =  normalize(cameraAndLights.sunDirection).xyz;
	float3 SurfacePos = normalize(input.nrmW).xyz;
	float4 Diffuse = float4(meshInfo.material.Kd,0);
	float4 Emissive = float4(meshInfo.material.Ke,0);
	float3 SurfaceNormal = normalize(input.nrmW);


	float3 LightingW = normalize(input.posW - Light);
	float3 view = normalize(cameraAndLights.cameraPos - input.posW  );

	float3 reflectedlight = reflect(-(Light) , SurfaceNormal) ;


	float intesity = pow(   max( saturate( dot(reflectedlight ,view) ) , 0 ) , meshInfo.material.sharpness ) ;
	float LightRatio = saturate(dot(-cameraAndLights.sunDirection.xyz,SurfaceNormal) );

	float4 totalReflected = cameraAndLights.sunColor *  float4(meshInfo.material.Ks,0) * intesity ;




	float totalDirect = LightRatio  * cameraAndLights.sunColor;
	float totalIndirect = cameraAndLights.sunAmbient *  float4(meshInfo.material.Ka,0);*/

	//return saturate(totalDirect + totalIndirect ) *  Diffuse + totalReflected +  Emissive;
	return  float4(0.5f,0.25f,0.75f,0);

}