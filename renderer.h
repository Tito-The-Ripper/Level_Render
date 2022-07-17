// minimalistic code to draw a single triangle, this is not part of the API.
// required for compiling shaders on the fly, consider pre-compiling instead
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

#include "d3dx12.h" // official helper file provided by microsoft
#include <chrono>
#include "Filemanage.h"

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

	 GW::MATH::GMATRIXF FViewMatrix;
	 GW::MATH::GMATRIXF FCameraMatrix;
	 GW::MATH::GMATRIXF FProjectionMatrix;

	 GW::MATH::GVECTORF FVec;
	 GW::MATH::GVECTORF FCameraPos;
	 GW::MATH::GVECTORF LightDirection;
	 GW::MATH::GVECTORF LightColor;
	 GW::MATH::GVECTORF LightAmbient;
	// what we need at a minimum to draw a triangle
	D3D12_VERTEX_BUFFER_VIEW					vertexView;
	Microsoft::WRL::ComPtr<ID3D12Resource>		vertexBuffer;

	D3D12_INDEX_BUFFER_VIEW indexView;
	Microsoft::WRL::ComPtr<ID3D12Resource>indexBuffer;
	
	D3D12_CONSTANT_BUFFER_VIEW_DESC  ConstaView;
	Microsoft::WRL::ComPtr<ID3D12Resource> ConstaBuffer;

	D3D12_SHADER_RESOURCE_VIEW_DESC  structViewWorld;
	D3D12_SHADER_RESOURCE_VIEW_DESC  structViewMats;
	D3D12_SHADER_RESOURCE_VIEW_DESC  structViewLight;

	Microsoft::WRL::ComPtr<ID3D12Resource> structBufferWorld;
	Microsoft::WRL::ComPtr<ID3D12Resource> structBufferMats;
	Microsoft::WRL::ComPtr<ID3D12Resource> structBufferLight;
	
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> HeapConstaBF;
	D3D12_DESCRIPTOR_HEAP_DESC HeapDescConstaBF;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> HeapStructureBF;
	D3D12_DESCRIPTOR_HEAP_DESC HeapDescStructureBF;

	Microsoft::WRL::ComPtr<ID3D12RootSignature>	rootSignature;
	Microsoft::WRL::ComPtr<ID3D12PipelineState>	pipeline;
	
	// Private Temp Vars
	IDXGISwapChain4* Swapping;
	DXGI_SWAP_CHAIN_DESC Swap_Data;
	float OLD_MOUSE_X_DELTA = 0;
	float  OLD_MOUSE_Y_DELTA = 0;

	unsigned int FrameRate = Swap_Data.BufferCount;

public:
	



	struct SCENE_DATA
	{
	
		GW::MATH::GMATRIXF viewMatrix, projectionMatrix; // cameraMatrix;
		GW::MATH::GVECTORF cameraPos;
		GW::MATH::GVECTORF  passing[7];
	
	};

	struct MESH_DATA
	{

		std::vector<H2B::ATTRIBUTES> material;
	
	};

	struct World
	{
		std::vector<GW::MATH::GMATRIXF> WorldMatrixs;
	};

	struct Mats
	{
		std::vector<H2B::ATTRIBUTES> material;
	};

	#pragma region Lightings

	struct DirectionalLight
	{
		GW::MATH::GVECTORF sunDirection, sunColor, sunAmbient;
	};
	
	#pragma endregion

	struct Lights
	{
		DirectionalLight DLight;
	
	};
	SCENE_DATA S_Data;
	MESH_DATA M_Data[2];
	World MatrixWorrld;
	Mats Materials;
	Lights Lighting;

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
		GW::MATH::GMatrix CreationMatrixes;
		GW::MATH::GVECTORF FVec;
		CreationMatrixes.Create();

		std::string vertexShaderSource;
		vertexShaderSource = ShaderAsString("../VertexShader.hlsl");
		
		
		std::string pixelShaderSource;
		pixelShaderSource = ShaderAsString("../PixelShader.hlsl");

		//World id with animation rotation 
		//CreationMatrixes.IdentityF(FWolrldMatrix);
		//CreationMatrixes.IdentityF(FWolrldMatrix2);

		

		//View Matrix

		#pragma region Camera/View Matrix

		CreationMatrixes.IdentityF(FViewMatrix);
		

		GW::MATH::GVECTORF FVecView;
		GW::MATH::GVECTORF FVecUP;
		GW::MATH::GVECTORF FVecLookAt;

		FVecUP.x = 0;  FVecUP.y = 1; FVecUP.z = 0; FVecUP.w = 0;
		FVecView.x = 0.75; FVecView.y = 0.25; FVecView.z = -1.5f; FVecView.w = 0;
		FVecLookAt.x = 0.15; FVecLookAt.y = 0.75; FVecLookAt.z = 0.0f; FVecLookAt.w = 0;

		

		CreationMatrixes.LookAtLHF(FVecView, FVecLookAt, FVecUP, FViewMatrix);

		FCameraPos = FVecView;

		FCameraMatrix = FViewMatrix;
		CreationMatrixes.InverseF(FCameraMatrix, FCameraMatrix);
#pragma endregion

		//Projection

		#pragma region Projection Matrix

		CreationMatrixes.IdentityF(FProjectionMatrix);

		float ASPECRATIO = 0;

		_d3d.GetAspectRatio(ASPECRATIO);

		CreationMatrixes.ProjectionDirectXLHF(1.13446f, ASPECRATIO, 0.1f, 100.0, FProjectionMatrix);
		#pragma endregion

		//Lights
		#pragma region Directional Light

		//LightDirection 
		LightDirection.x = -1; LightDirection.y = -1; LightDirection.z = 2; LightDirection.w = 0;

		//Light Color
		LightColor.x = 0.9; LightColor.y = 0.9; LightColor.z = 1.0; LightColor.w = 1.0;

		//Light Ambient
		LightAmbient.x = 0.25; LightAmbient.y = 0.25; LightAmbient.z = 0.35; LightAmbient.w = 0;

		#pragma endregion

		// TODO: part 2b
		S_Data.viewMatrix = FViewMatrix;
		S_Data.cameraPos = FCameraPos;
		S_Data.projectionMatrix = FProjectionMatrix;

		Lighting.DLight.sunDirection = LightDirection;
		Lighting.DLight.sunColor = LightColor;
		Lighting.DLight.sunAmbient = LightAmbient;

		for (int i = 0; i < ModelContainer.size(); i++)
		{
			for (int j = 0; j < ModelContainer[i].World.size(); j++)
			{
				MatrixWorrld.WorldMatrixs.push_back(ModelContainer[i].World[j]);
			}
			
		}


		

		//M_Data[0].worldMatrix = FWolrldMatrix;
		//M_Data[0].material = FSLogo_materials[0].attrib;

		//M_Data[1].worldMatrix = FWolrldMatrix2;
		//M_Data[1].material = FSLogo_materials[1].attrib;

		// TODO: Part 1b
		// TODO: Part 1c
		// Create Vertex Buffer
		
		// TODO: Part 1d

		#pragma region VertexBuffer
		
		unsigned int Vetrex = Level.Vertices.size() * sizeof(H2B::VERTEX);

			creator->CreateCommittedResource( // using UPLOAD heap for simplicity
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), // DEFAULT recommend  
				D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(Vetrex),
				D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertexBuffer));
			// Transfer triangle data to the vertex buffer.
			UINT8* transferMemoryLocationVertices;
			vertexBuffer->Map(0, &CD3DX12_RANGE(0, 0),
				reinterpret_cast<void**>(&transferMemoryLocationVertices));
			memcpy(transferMemoryLocationVertices, Level.Vertices.data(), Vetrex);
			vertexBuffer->Unmap(0, nullptr);
			// TODO: Part 1c
			// Create a vertex View to send to a Draw() call.
			vertexView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
			vertexView.StrideInBytes = sizeof(H2B::VERTEX) * 1;
			vertexView.SizeInBytes = Vetrex;
		
		#pragma endregion

		#pragma region IndicesBuffer
		

			unsigned int Indices = Level.Indices.size() * sizeof(unsigned int);
			creator->CreateCommittedResource( // using UPLOAD heap for simplicity
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), // DEFAULT recommend  
				D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(Indices),
				D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&indexBuffer));
			// Transfer triangle data to the vertex buffer.
			UINT8* transferMemoryLocationIndices;
			indexBuffer->Map(0, &CD3DX12_RANGE(0, 0),
				reinterpret_cast<void**>(&transferMemoryLocationIndices));
			memcpy(transferMemoryLocationIndices, Level.Indices.data(), Indices);
			indexBuffer->Unmap(0, nullptr);
			// TODO: Part 1c
			// Create a vertex View to send to a Draw() call.
			indexView.BufferLocation = indexBuffer->GetGPUVirtualAddress();
			indexView.Format = DXGI_FORMAT_R32_UINT;
			indexView.SizeInBytes = Indices;
		
		#pragma endregion

		#pragma region ConstantBuffer
			_d3d.GetSwapchain4((void**)&Swapping);
			Swapping->GetDesc(&Swap_Data);
				
			Swapping->GetCurrentBackBufferIndex();

			creator->CreateCommittedResource( // using UPLOAD heap for simplicity
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), // DEFAULT recommend  
				D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(sizeof(SCENE_DATA)),
				D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&ConstaBuffer));
			
			// Transfer triangle data to the vertex buffer.
			
			UINT8* transferMemoryLocationConstantBF;
			ConstaBuffer->Map(0, &CD3DX12_RANGE(0, 0),
				reinterpret_cast<void**>(&transferMemoryLocationConstantBF));

				memcpy(transferMemoryLocationConstantBF, &S_Data, sizeof(SCENE_DATA));

			ConstaBuffer->Unmap(0, nullptr);
			// TODO: Part 1c
			// Create a vertex View to send to a Draw() call.
			ConstaView.BufferLocation = ConstaBuffer->GetGPUVirtualAddress();
			ConstaView.SizeInBytes = sizeof(SCENE_DATA);
	

		#pragma region HeapCB
			HeapDescConstaBF.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
			HeapDescConstaBF.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			HeapDescConstaBF.NumDescriptors = 1;
			HeapDescConstaBF.NodeMask = 0;

			creator->CreateDescriptorHeap(&HeapDescConstaBF, IID_PPV_ARGS(HeapConstaBF.ReleaseAndGetAddressOf()));

			// TODO: Part 2f
			creator->CreateConstantBufferView(&ConstaView, HeapConstaBF->GetCPUDescriptorHandleForHeapStart());

		#pragma endregion	

		#pragma endregion

		#pragma region StructuredBuffer

			

			

			// World Matrices
			
			#pragma region StructuredBufferWordMatrices
			structViewWorld.Buffer.FirstElement = 0;
			structViewWorld.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
			structViewWorld.Buffer.NumElements = ModelContainer.data()->World.size();
			structViewWorld.Buffer.StructureByteStride = sizeof(World);
			structViewWorld.Format = DXGI_FORMAT_UNKNOWN;
			structViewWorld.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
			structViewWorld.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

			creator->CreateCommittedResource( // using UPLOAD heap for simplicity
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), // DEFAULT recommend  
				D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(structViewWorld.Buffer.NumElements* structViewWorld.Buffer.StructureByteStride),
				D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&structBufferWorld));


			// World Matrices
			UINT8* transferMemoryLocationStructBFWorld;
			structBufferWorld->Map(0, &CD3DX12_RANGE(0, 0),
				reinterpret_cast<void**>(&transferMemoryLocationStructBFWorld));

			memcpy(transferMemoryLocationStructBFWorld, &MatrixWorrld, sizeof(World));

			structBufferWorld->Unmap(0, nullptr);
			#pragma endregion


			// Mats data
			#pragma region StructuredBufferMaterials
			structViewMats.Buffer.FirstElement = 0;
			structViewMats.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
			structViewMats.Buffer.NumElements = ModelContainer.data()->Mats.size();
			structViewMats.Buffer.StructureByteStride = sizeof(Mats);
			structViewMats.Format = DXGI_FORMAT_UNKNOWN;
			structViewMats.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
			structViewMats.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

			creator->CreateCommittedResource( // using UPLOAD heap for simplicity
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), // DEFAULT recommend  
				D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(structViewMats.Buffer.NumElements* structViewMats.Buffer.StructureByteStride),
				D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&structBufferMats));


			UINT8* transferMemoryLocationStructBFMats;
			structBufferMats->Map(0, &CD3DX12_RANGE(0, 0),
				reinterpret_cast<void**>(&transferMemoryLocationStructBFMats));

			memcpy(transferMemoryLocationStructBFMats, &Materials, sizeof(Mats));

			structBufferMats->Unmap(0, nullptr);
			#pragma endregion

			
			

			#pragma region HeapSB
			HeapDescStructureBF.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
			HeapDescStructureBF.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			HeapDescStructureBF.NumDescriptors = 1;
			HeapDescStructureBF.NodeMask = 0;

			creator->CreateDescriptorHeap(&HeapDescStructureBF, IID_PPV_ARGS(HeapStructureBF.ReleaseAndGetAddressOf()));

			// TODO: Part 2f
			creator->CreateShaderResourceView(structBufferWorld.Get(), &structViewWorld , HeapStructureBF->GetCPUDescriptorHandleForHeapStart());
			
			creator->CreateShaderResourceView(structBufferMats.Get(), &structViewMats, HeapStructureBF->GetCPUDescriptorHandleForHeapStart());

			#pragma endregion	

		#pragma endregion

		// Create Vertex Shader
		UINT compilerFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if _DEBUG
		compilerFlags |= D3DCOMPILE_DEBUG;
#endif
		Microsoft::WRL::ComPtr<ID3DBlob> vsBlob, errors;
		if (FAILED(D3DCompile(vertexShaderSource.c_str(), strlen(vertexShaderSource.c_str()),
			nullptr, nullptr, nullptr, "main", "vs_5_1", compilerFlags, 0, 
			vsBlob.GetAddressOf(), errors.GetAddressOf())))
		{
			std::cout << (char*)errors->GetBufferPointer() << std::endl;
			abort();
		}
		// Create Pixel Shader
		Microsoft::WRL::ComPtr<ID3DBlob> psBlob; errors.Reset();
		if (FAILED(D3DCompile(pixelShaderSource.c_str(), strlen(pixelShaderSource.c_str()),
			nullptr, nullptr, nullptr, "main", "ps_5_1", compilerFlags, 0, 
			psBlob.GetAddressOf(), errors.GetAddressOf())))
		{
			std::cout << (char*)errors->GetBufferPointer() << std::endl;
			abort();
		}
		// TODO: Part 1c
		// Create Input Layout
		#pragma region InputLayout
		D3D12_INPUT_ELEMENT_DESC format[] = {

			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{"TEXTURE", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{"NORM", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
		};
		#pragma endregion

		// TODO: Part 2c
		#pragma region RootParameter N RootSignature



		std::array<CD3DX12_ROOT_PARAMETER, 4> Rootpara;
		Rootpara[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
		Rootpara[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);
		Rootpara[2].InitAsDescriptorTable();
		//Rootpara[2].InitAsShaderResourceView(2, 0, D3D12_SHADER_VISIBILITY_ALL);
		//Rootpara[3].InitAsShaderResourceView(3, 0, D3D12_SHADER_VISIBILITY_ALL);
		//Rootpara[4].InitAsShaderResourceView(4, 0, D3D12_SHADER_VISIBILITY_ALL);
		
		// create root signature
		CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
		rootSignatureDesc.Init(Rootpara.size(), Rootpara.data(), 0, nullptr,
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
		Microsoft::WRL::ComPtr<ID3DBlob> signature;
		D3D12SerializeRootSignature(&rootSignatureDesc, 
			D3D_ROOT_SIGNATURE_VERSION_1, &signature, &errors);
		creator->CreateRootSignature(0, signature->GetBufferPointer(), 
			signature->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
		#pragma endregion


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
		
		// TODO: Part 3a
		// TODO: Part 2b
	

	

		cmd->SetDescriptorHeaps(0, HeapConstaBF.ReleaseAndGetAddressOf());

		cmd->SetGraphicsRootConstantBufferView(0, ConstaBuffer->GetGPUVirtualAddress());

		cmd->SetGraphicsRootConstantBufferView(1, ConstaBuffer->GetGPUVirtualAddress());

		//cmd->SetDescriptorHeaps(1, HeapStructureBF.ReleaseAndGetAddressOf());

		cmd->SetGraphicsRootShaderResourceView(2, structBufferWorld->GetGPUVirtualAddress());

		cmd->SetGraphicsRootShaderResourceView(3, structBufferMats->GetGPUVirtualAddress());

		//cmd->SetGraphicsRootShaderResourceView(4, structBufferLight->GetGPUVirtualAddress());
		
		cmd->OMSetRenderTargets(1, &rtv, FALSE, &dsv);
		cmd->SetPipelineState(pipeline.Get());

		cmd->IASetVertexBuffers(0, 1, &vertexView);
		cmd->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		cmd->IASetIndexBuffer(&indexView);
		//cmd->DrawIndexedInstanced(8532, 1, 0, 0, 0);


		// TODO: Part 3b
		//for (int i = 0; i < FSLogo_materialcount; i++)
		//{
		//	// TODO: Part 3c
		//	// now we can draw
		//	
		//	// TODO: Part 1h
		



		//	if (i == 0)
		//	{
		//		
		//		

				
		//	}
		//	else
		//	{
		//		

		//		//cmd->SetGraphicsRootConstantBufferView(1, ConstaBuffer->GetGPUVirtualAddress() + sizeof(SCENE_DATA) + sizeof(MESH_DATA));
		//	}

			cmd->DrawIndexedInstanced(80000, 1, 0, 0, 0);
		//}
		
				//cmd->SetGraphicsRoot32BitConstants(0, 64, &, 0);
		

		
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
