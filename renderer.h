// minimalistic code to draw a single triangle, this is not part of the API.
// required for compiling shaders on the fly, consider pre-compiling instead
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#include "d3dx12.h" // official helper file provided by microsoft
#include <chrono>
// Simple Vertex Shader
const char* vertexShaderSource = R"(
// an ultra simple hlsl vertex shader
// TODO: Part 2b

	cbuffer SHADER_VARS : register(b0)
	{
		matrix worldMatrix;
		matrix viewMatrix;
		matrix projectionMatrix;

	}

	// TODO: Part 2f
	// TODO: Part 3b
// TODO: Part 1c
float4 main(float4 inputVertex : POSITION) : SV_POSITION
{
	// TODO: Part 2d, Part 2f, Part 3b
	
	inputVertex = mul(worldMatrix, inputVertex);
	inputVertex = mul(viewMatrix, inputVertex);
	inputVertex = mul(projectionMatrix, inputVertex);

	return inputVertex;
}
)";
// Simple Pixel Shader
const char* pixelShaderSource = R"(
// an ultra simple hlsl pixel shader
float4 main() : SV_TARGET 
{	
	return float4(0.5f,0.25f,0.75f,0); // TODO: Part 1a
}
)";
// Creation, Rendering & Cleanup
class Renderer
{
	// proxy handles
	GW::SYSTEM::GWindow win;
	GW::GRAPHICS::GDirectX12Surface d3d;
	// TODO: Part 4a
	GW::INPUT::GInput PCcontrols;
	GW::INPUT::GController Consolecontrols;
	
	
	// TODO: Part 2a
	GW::MATH::GMATRIXF FWorldMatrix;
	// TODO: Part 3c
	 GW::MATH::GMATRIXF FWorldMatrix1;
	 GW::MATH::GMATRIXF FWorldMatrix2;
	 GW::MATH::GMATRIXF FWorldMatrix3;
	 GW::MATH::GMATRIXF FWorldMatrix4;
	 GW::MATH::GMATRIXF FWorldMatrix5;
	// TODO: Part 2e
	 GW::MATH::GMATRIXF FViewMatrix;
	 GW::MATH::GMATRIXF FCameraMatrix;
	// TODO: Part 3a
	 GW::MATH::GMATRIXF FProjectionMatrix;
	// what we need at a minimum to draw a triangle
	D3D12_VERTEX_BUFFER_VIEW					vertexView;
	Microsoft::WRL::ComPtr<ID3D12Resource>		vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D12RootSignature>	rootSignature;
	Microsoft::WRL::ComPtr<ID3D12PipelineState>	pipeline;
	
	// Private Temp Vars
	float OLD_MOUSE_X_DELTA = 0;
	float  OLD_MOUSE_Y_DELTA = 0;

public:
	// TODO: Part 1c
	struct Vertex
	{
		float x , y , z , w ;
	};
	// TODO: Part 2b
	struct SHADER_VARS
	{
		GW::MATH::GMATRIXF worldMatrix;
		GW::MATH::GMATRIXF viewMatrix;
		GW::MATH::GMATRIXF projectionMatrix;
		
	};





		// TODO: Part 2f
		// TODO: Part 3b
	Renderer(GW::SYSTEM::GWindow _win, GW::GRAPHICS::GDirectX12Surface _d3d)
	{
		win = _win;
		d3d = _d3d;
		unsigned int Width, Height;
		win.GetClientWidth(Width);
	    win.GetClientHeight(Height);
		ID3D12Device* creator;
		d3d.GetDevice((void**)&creator);
		// TODO: Part 4a
		PCcontrols.Create(_win);
		Consolecontrols.Create();
		// TODO: Part 2a
		GW::MATH::GMatrix GRMatrix4x4;
		GW::MATH::GVECTORF FVec;
		GRMatrix4x4.Create();

		// 1th matrix W/////////////////////// Bottom
		GRMatrix4x4.IdentityF(FWorldMatrix);
		GRMatrix4x4.RotateXGlobalF(FWorldMatrix, 1.5708f, FWorldMatrix);
		FVec.x = 0; FVec.y = -0.5; FVec.z = 0; FVec.w = 0;
		
		GRMatrix4x4.TranslateGlobalF(FWorldMatrix, FVec, FWorldMatrix);

		
		
		// TODO: Part 3c
		// // 2th matrix W/////////////////////// Top
		GW::MATH::GVECTORF FVec1;
		GRMatrix4x4.IdentityF(FWorldMatrix1);
		GRMatrix4x4.RotateXGlobalF(FWorldMatrix1, 1.5708f, FWorldMatrix1);
		FVec1.x = 0; FVec1.y = 0.5; FVec1.z = 0; FVec1.w = 0;

		GRMatrix4x4.TranslateGlobalF(FWorldMatrix1, FVec1, FWorldMatrix1);

		// 2th matrix W/////////////////////// Left Corners r
		GW::MATH::GVECTORF FVec2;
		GRMatrix4x4.IdentityF(FWorldMatrix2);
		GRMatrix4x4.RotateYGlobalF(FWorldMatrix2, 1.5708f, FWorldMatrix2);
		FVec2.x = -0.5; FVec2.y = 0; FVec2.z = 0.0; FVec2.w = 0;
		GRMatrix4x4.TranslateGlobalF(FWorldMatrix2, FVec2, FWorldMatrix2);

		
		// 3th matrix W/////////////////////// Right Left Corners r
		GW::MATH::GVECTORF FVec3;
		GRMatrix4x4.IdentityF(FWorldMatrix3);
		GRMatrix4x4.RotateYGlobalF(FWorldMatrix3, 0, FWorldMatrix3);
		FVec3.x = 0; FVec3.y = 0; FVec3.z = 0.5; FVec3.w = 0;
		GRMatrix4x4.TranslateGlobalF(FWorldMatrix3, FVec3, FWorldMatrix3);

		
		// 4th matrix W/////////////////////// Left Corners L
		GW::MATH::GVECTORF FVec4;
		GRMatrix4x4.IdentityF(FWorldMatrix4);
		GRMatrix4x4.RotateYGlobalF(FWorldMatrix4, 1.5708f, FWorldMatrix4);
		FVec4.x = 0.5; FVec4.y = 0; FVec4.z = 0; FVec4.w = 0;
		GRMatrix4x4.TranslateGlobalF(FWorldMatrix4, FVec4, FWorldMatrix4);
		
		// 5th matrix W///////////////////////Right Corners L
		GW::MATH::GVECTORF FVec5;
		GRMatrix4x4.IdentityF(FWorldMatrix5);
		GRMatrix4x4.RotateYGlobalF(FWorldMatrix5, 0, FWorldMatrix5);
		FVec5.x = 0; FVec5.y = 0; FVec5.z = -0.5; FVec5.w = 0;
		GRMatrix4x4.TranslateGlobalF(FWorldMatrix5, FVec5, FWorldMatrix5);

		// TODO: Part 2e
		GRMatrix4x4.IdentityF(FViewMatrix);
		GRMatrix4x4.RotateXGlobalF(FViewMatrix, -0.314159f, FViewMatrix);
		GW::MATH::GVECTORF FVecView;
		GW::MATH::GVECTORF FVecUP;
		FVecUP.x = 0;  FVecUP.y = 1; FVecUP.z = 0; FVecUP.w = 0;

		FVecView.x = 0.25; FVecView.y = -0.125; FVecView.z = -0.25f; FVecView.w = 0;

		GRMatrix4x4.TranslateGlobalF(FViewMatrix, FVecView, FViewMatrix);
		
		// CA: this function gives you and ALREADY-INVERSED view matrix. This means it can be used for rendering on the GPU,
		// but if you want the TRUE world matrix of the camera, you will need to inverse it after calling LookAt on it.
		
		GRMatrix4x4.LookAtLHF(FVecView, FVec,FVecUP, FViewMatrix);

		
		FCameraMatrix = FViewMatrix;
		
		GRMatrix4x4.InverseF(FCameraMatrix, FCameraMatrix);

		GRMatrix4x4.IdentityF(FProjectionMatrix);
		float ASPECRATIO = 0;
		
		_d3d.GetAspectRatio(ASPECRATIO);
		
		 GRMatrix4x4.ProjectionDirectXLHF(1.13446f, ASPECRATIO, 0.1f, 100.0, FProjectionMatrix);

		// TODO: Part 1b
		// TODO: Part 1c
		// Create Vertex Buffer
		const Vertex verts[] = {
			//Vertic
			{ -0.5f, 0.5f, 0, 1 }, { -0.5f, -0.5f, 0, 1 },
			{ -0.46f,0.5f, 0, 1 }, { -0.46f,-0.5f, 0, 1 },
			{ -0.42f,0.5f, 0, 1 }, { -0.42f,-0.5f, 0, 1 },
			{ -0.38f,0.5f, 0, 1 }, { -0.38f,-0.5f, 0, 1 },
			{ -0.34f,0.5f, 0, 1 }, { -0.34f,-0.5f, 0, 1 },
			{ -0.3f, 0.5f, 0, 1 }, { -0.3f, -0.5f, 0, 1 },
			{ -0.26f,0.5f, 0, 1 }, { -0.26f,-0.5f, 0, 1 },
			{ -0.22f,0.5f, 0, 1 }, { -0.22f,-0.5f, 0, 1 },
			{ -0.18f,0.5f, 0, 1 }, { -0.18f,-0.5f, 0, 1 },
			{ -0.14f,0.5f, 0, 1 }, { -0.14f,-0.5f, 0, 1 },
			{ -0.1f, 0.5f, 0, 1 }, { -0.1f, -0.5f, 0, 1 },
			{ -0.06f,0.5f, 0, 1 }, { -0.06f,-0.5f, 0, 1 },
			{ -0.02f,0.5f, 0, 1 }, { -0.02f,-0.5f, 0, 1 },
			{ 0.02f, 0.5f, 0, 1 }, { 0.02f, -0.5f, 0, 1 },
			{ 0.06f, 0.5f, 0, 1 }, { 0.06f, -0.5f, 0, 1 },
			{ 0.1f,  0.5f, 0, 1 }, { 0.1f,  -0.5f, 0, 1 },
			{ 0.14f, 0.5f, 0, 1 }, { 0.14f, -0.5f, 0, 1 },
			{ 0.18f, 0.5f, 0, 1 }, { 0.18f, -0.5f, 0, 1 },
			{ 0.22f, 0.5f, 0, 1 }, { 0.22f, -0.5f, 0, 1 },
			{ 0.26f, 0.5f, 0, 1 }, { 0.26f, -0.5f, 0, 1 },
			{ 0.3f,  0.5f, 0, 1 }, { 0.3f,  -0.5f, 0, 1 },
			{ 0.34f, 0.5f, 0, 1 }, { 0.34f, -0.5f, 0, 1 },
			{ 0.38f, 0.5f, 0, 1 }, { 0.38f, -0.5f, 0, 1 },
			{ 0.42f, 0.5f, 0, 1 }, { 0.42f, -0.5f, 0, 1 },
			{ 0.46f, 0.5f, 0, 1 }, { 0.46f, -0.5f, 0, 1 },
			{ 0.5f,  0.5f, 0, 1 }, { 0.5f,  -0.5f, 0, 1 },

			//Hor
			{ 0.5, -0.5, 0, 1 },{ -0.5, -0.5, 0, 1 },
			{ 0.5, -0.46, 0, 1 },{ -0.5, -0.46, 0, 1 },
			{ 0.5, -0.42, 0, 1 },{ -0.5, -0.42, 0, 1 },
			{ 0.5, -0.38, 0, 1 },{ -0.5, -0.38, 0, 1 },
			{ 0.5, -0.34, 0, 1 },{ -0.5, -0.34, 0, 1 },
			{ 0.5, -0.3, 0, 1 },{ -0.5, -0.3, 0, 1 },
			{ 0.5, -0.26, 0, 1 },{ -0.5, -0.26, 0, 1 },
			{ 0.5, -0.22, 0, 1 },{ -0.5, -0.22, 0, 1 },
			{ 0.5, -0.18, 0, 1 },{ -0.5, -0.18, 0, 1 },
			{ 0.5, -0.14, 0, 1 },{ -0.5, -0.14, 0, 1 },
			{ 0.5, -0.1, 0, 1 },{ -0.5, -0.1, 0, 1 },
			{ 0.5, -0.06, 0, 1 },{ -0.5, -0.06, 0, 1 },
			{ 0.5, -0.02, 0, 1 },{ -0.5, -0.02, 0, 1 },
			{ 0.5, 0.02, 0, 1 },{ -0.5, 0.02, 0, 1 },
			{ 0.5, 0.06, 0, 1 },{ -0.5, 0.06, 0, 1 },
			{ 0.5, 0.1, 0, 1 },{ -0.5, 0.1, 0, 1 },
			{ 0.5, 0.14, 0, 1 },{ -0.5, 0.14, 0, 1 },
			{ 0.5, 0.18, 0, 1 },{ -0.5, 0.18, 0, 1 },
			{ 0.5, 0.22, 0, 1 },{ -0.5, 0.22, 0, 1 },
			{ 0.5, 0.26, 0, 1 },{ -0.5, 0.26, 0, 1 },
			{ 0.5, 0.3, 0, 1 },{ -0.5, 0.3, 0, 1 },
			{ 0.5, 0.34, 0, 1 },{ -0.5, 0.34, 0, 1 },
			{ 0.5, 0.38, 0, 1 },{ -0.5, 0.38, 0, 1 },
			{ 0.5, 0.42, 0, 1 },{ -0.5, 0.42, 0, 1 },
			{ 0.5, 0.46, 0, 1 },{ -0.5, 0.46, 0, 1 },
			{ 0.5, 0.5, 0, 1 },{ -0.5, 0.5, 0, 1 },
		};
		// TODO: Part 1d
		creator->CreateCommittedResource( // using UPLOAD heap for simplicity
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), // DEFAULT recommend  
			D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(sizeof(verts)),
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertexBuffer));
		// Transfer triangle data to the vertex buffer.
		UINT8* transferMemoryLocation;
		vertexBuffer->Map(0, &CD3DX12_RANGE(0, 0),
			reinterpret_cast<void**>(&transferMemoryLocation));
		memcpy(transferMemoryLocation, verts, sizeof(verts));
		vertexBuffer->Unmap(0, nullptr);
		// TODO: Part 1c
		// Create a vertex View to send to a Draw() call.
		vertexView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
		vertexView.StrideInBytes = sizeof(float) * 4;
		vertexView.SizeInBytes = sizeof(verts);
		// Create Vertex Shader
		UINT compilerFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if _DEBUG
		compilerFlags |= D3DCOMPILE_DEBUG;
#endif
		Microsoft::WRL::ComPtr<ID3DBlob> vsBlob, errors;
		if (FAILED(D3DCompile(vertexShaderSource, strlen(vertexShaderSource),
			nullptr, nullptr, nullptr, "main", "vs_5_0", compilerFlags, 0, 
			vsBlob.GetAddressOf(), errors.GetAddressOf())))
		{
			std::cout << (char*)errors->GetBufferPointer() << std::endl;
			abort();
		}
		// Create Pixel Shader
		Microsoft::WRL::ComPtr<ID3DBlob> psBlob; errors.Reset();
		if (FAILED(D3DCompile(pixelShaderSource, strlen(pixelShaderSource),
			nullptr, nullptr, nullptr, "main", "ps_5_0", compilerFlags, 0, 
			psBlob.GetAddressOf(), errors.GetAddressOf())))
		{
			std::cout << (char*)errors->GetBufferPointer() << std::endl;
			abort();
		}
		// TODO: Part 1c
		// Create Input Layout
		D3D12_INPUT_ELEMENT_DESC format[] = {
			{ 
				"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 
				D3D12_APPEND_ALIGNED_ELEMENT, 
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
			}
		};
		// TODO: Part 2c
		CD3DX12_ROOT_PARAMETER rootParameter;
		rootParameter.InitAsConstants(64,0,0,D3D12_SHADER_VISIBILITY_VERTEX);
		
		// create root signature
		CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
		rootSignatureDesc.Init(1, &rootParameter, 0, nullptr,
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
		Microsoft::WRL::ComPtr<ID3DBlob> signature;
		D3D12SerializeRootSignature(&rootSignatureDesc, 
			D3D_ROOT_SIGNATURE_VERSION_1, &signature, &errors);
		creator->CreateRootSignature(0, signature->GetBufferPointer(), 
			signature->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
		// create pipeline state
		D3D12_GRAPHICS_PIPELINE_STATE_DESC psDesc;
		ZeroMemory(&psDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
		psDesc.InputLayout = { format, ARRAYSIZE(format) };
		psDesc.pRootSignature = rootSignature.Get();
		psDesc.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
		psDesc.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());
		psDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		psDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		psDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		psDesc.SampleMask = UINT_MAX;
		psDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE; // TODO: Part 1b
		psDesc.NumRenderTargets = 1;
		psDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		psDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
		psDesc.SampleDesc.Count = 1;
		creator->CreateGraphicsPipelineState(&psDesc, IID_PPV_ARGS(&pipeline));
		// free temporary handle
		creator->Release();
	}
	void Render()
	{
		// grab the context & render target
		ID3D12GraphicsCommandList* cmd;
		D3D12_CPU_DESCRIPTOR_HANDLE rtv;
		D3D12_CPU_DESCRIPTOR_HANDLE dsv;
		d3d.GetCommandList((void**)&cmd);
		d3d.GetCurrentRenderTargetView((void**)&rtv);
		d3d.GetDepthStencilView((void**)&dsv);
		// setup the pipeline
		cmd->SetGraphicsRootSignature(rootSignature.Get());
		cmd->OMSetRenderTargets(1, &rtv, FALSE, &dsv);
		cmd->SetPipelineState(pipeline.Get());
		// TODO: Part 3a
		// TODO: Part 2b
		SHADER_VARS Shader;
	    Shader.worldMatrix = FWorldMatrix;
		Shader.viewMatrix = FViewMatrix;
		Shader.projectionMatrix = FProjectionMatrix;
		cmd->SetGraphicsRoot32BitConstants(0, 64, &Shader, 0);
		// now we can draw
		cmd->IASetVertexBuffers(0, 1, &vertexView);
		// TODO: Part 1b
		cmd->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
		cmd->DrawInstanced(104, 1, 0, 0); // TODO: Part 1d
		

		// TODO: Part 3d
		Shader.worldMatrix = FWorldMatrix1;
		cmd->SetGraphicsRoot32BitConstants(0, 64, &Shader, 0);
		cmd->DrawInstanced(104, 1, 0, 0); // TODO: Part 1d
		
		Shader.worldMatrix = FWorldMatrix2;
		cmd->SetGraphicsRoot32BitConstants(0, 64, &Shader, 0);
		cmd->DrawInstanced(104, 1, 0, 0); // TODO: Part 1d
		
		Shader.worldMatrix = FWorldMatrix3;
		cmd->SetGraphicsRoot32BitConstants(0, 64, &Shader, 0);
		cmd->DrawInstanced(104, 1, 0, 0); // TODO: Part 1d

		Shader.worldMatrix = FWorldMatrix4;
		cmd->SetGraphicsRoot32BitConstants(0, 64, &Shader, 0);
		cmd->DrawInstanced(104, 1, 0, 0); // TODO: Part 1d

		Shader.worldMatrix = FWorldMatrix5;
		cmd->SetGraphicsRoot32BitConstants(0, 64, &Shader, 0);
		cmd->DrawInstanced(104, 1, 0, 0); // TODO: Part 1d

		
		// release temp handles
		cmd->Release();
	}
	// TODO: Part 4b 
	std::chrono::high_resolution_clock::time_point Last;
	

	void UpdateCamera()
	{
		
		GW::MATH::GMatrix GRMatrix4x4;
		GW::MATH::GVECTORF Movement;
		GRMatrix4x4.Create();
	
		//Controls Up AND Down
		float SPACE_KEY_STATE = 0;
		float LEFT_SHIFT_STATE = 0;
		float RIGHT_TRIGGER_STATE = 0;
		float LEFT_TRIGGER_STATE = 0;

		//Controls Foward AND Back
		float W_KEY_STATE = 0;
		float S_KEY_STATE = 0;
		float LEFT_STICK_Y_AXIS_STATE = 0;
		

		//Controls Left AND Right
		float D_KEY_STATE = 0;
		float A_KEY_STATE = 0;
		float LEFT_STICK_X_AXIS_STATE = 0;
	

		std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
		// TODO: Part 4c
		// TODO: Part 4d
		std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<float>>(now - Last);
		
		//Y axis
		PCcontrols.GetState(G_KEY_SPACE, SPACE_KEY_STATE);

		PCcontrols.GetState(G_KEY_LEFTSHIFT, LEFT_SHIFT_STATE);

		Consolecontrols.GetState(0 ,G_RIGHT_TRIGGER_AXIS, RIGHT_TRIGGER_STATE);

		Consolecontrols.GetState( 0,G_LEFT_TRIGGER_AXIS, LEFT_TRIGGER_STATE);
		
		float Total_Y_Change = 0;
		const float Camera_Speed = 0.3f;
		
	    Total_Y_Change = SPACE_KEY_STATE - LEFT_SHIFT_STATE ;
		FCameraMatrix.row4.y += Total_Y_Change * Camera_Speed * time_span.count();
		//-----------------------------------------------------------------------------------------------//
		//MOVEMENT
		PCcontrols.GetState(G_KEY_W, W_KEY_STATE);

		PCcontrols.GetState(G_KEY_A, A_KEY_STATE);

		PCcontrols.GetState(G_KEY_D, D_KEY_STATE);

		PCcontrols.GetState(G_KEY_S, S_KEY_STATE);

	

			//Consolecontrols.GetState(0, , LEFT_STICK_Y_AXIS_STATE);
			//Consolecontrols.GetState(0, G_LEFT_, LEFT_STICK_X_AXIS_STATE);
		float PerFrameSpeed = 0;
		float Total_Z_Change = 0;
		float Total_X_Change = 0;
		GW::MATH::GMATRIXF TranslationMF;

		PerFrameSpeed = Camera_Speed * time_span.count();
		Total_Z_Change = W_KEY_STATE - S_KEY_STATE ;
		Total_X_Change = D_KEY_STATE - A_KEY_STATE ;
		Movement.x = Total_X_Change * PerFrameSpeed;
		Movement.y = 0;
		Movement.z = Total_Z_Change * PerFrameSpeed;
		Movement.w = 0;
		GRMatrix4x4.IdentityF(TranslationMF);
		GRMatrix4x4.TranslateLocalF(TranslationMF, Movement, TranslationMF);
		GRMatrix4x4.MultiplyMatrixF(TranslationMF, FCameraMatrix, FCameraMatrix);
		//---------------------------------------------------------------------------------------------------//
			
		float Thumb_Speed = 0;
		float Total_Pitch = 0;
		float Total_Yaw = 0;
		float  RIGHT_STICK_Y_AXIS_STATE = 0;
		float  RIGHT_STICK_X_AXIS_STATE = 0;
		
		GW::MATH::GMATRIXF MousePitchMF;
		GW::MATH::GMATRIXF MouseYawMF;
		GW::MATH::GMATRIXF SaveMatrix;
		GW::MATH::GVECTORF SavePos;

		unsigned int SCREEN_HEIGHT = 0;
		unsigned int SCREEN_WIDTH = 0;
		float ASPECTRATIO = 0;
		win.GetClientHeight(SCREEN_HEIGHT);
		win.GetClientWidth(SCREEN_WIDTH);
		d3d.GetAspectRatio(ASPECTRATIO);
		
		float MOUSE_X_DELTA = 0;
		float MOUSE_Y_DELTA = 0;
		
		int SaveCounter = 0;
		PCcontrols.GetMouseDelta(MOUSE_X_DELTA, MOUSE_Y_DELTA);
	
			Thumb_Speed = G_PI_F * time_span.count();
			
			// Looking up and down 
			
				Total_Pitch = 65 * MOUSE_Y_DELTA / SCREEN_HEIGHT * Thumb_Speed;
				Total_Yaw = 65 * ASPECTRATIO * MOUSE_X_DELTA / SCREEN_WIDTH  * Thumb_Speed;

				SaveMatrix = FCameraMatrix;
				
				GRMatrix4x4.IdentityF(MousePitchMF);
				GRMatrix4x4.IdentityF(MouseYawMF);

				GRMatrix4x4.RotationYawPitchRollF(0, Total_Pitch, 0, MousePitchMF);
				GRMatrix4x4.MultiplyMatrixF(MousePitchMF, FCameraMatrix, FCameraMatrix);

				GRMatrix4x4.RotateYGlobalF(MouseYawMF, Total_Yaw, MouseYawMF);
				
				SavePos.x = FCameraMatrix.row4.x;
				SavePos.y = FCameraMatrix.row4.y;
				SavePos.z = FCameraMatrix.row4.z;
				SavePos.w = FCameraMatrix.row4.w;

				GRMatrix4x4.MultiplyMatrixF(FCameraMatrix, MouseYawMF, FCameraMatrix);


				FCameraMatrix.row4.x = SavePos.x;
				FCameraMatrix.row4.y = SavePos.y;
				FCameraMatrix.row4.z = SavePos.z;
				FCameraMatrix.row4.w = SavePos.w;
				
				if (MOUSE_X_DELTA == OLD_MOUSE_X_DELTA && MOUSE_Y_DELTA == OLD_MOUSE_Y_DELTA)
				{
					FCameraMatrix = SaveMatrix;
				}
			
			 OLD_MOUSE_X_DELTA = MOUSE_X_DELTA;
			 OLD_MOUSE_Y_DELTA = MOUSE_Y_DELTA;


		FViewMatrix = FCameraMatrix;
		
		Last = std::chrono::high_resolution_clock::now();
		
		
		GRMatrix4x4.InverseF(FViewMatrix, FViewMatrix);
		
	}
		
	~Renderer()
	{
		// ComPtr will auto release so nothing to do here 
	}
};
