// Simple basecode showing how to create a window and attatch a d3d12surface
#define GATEWARE_ENABLE_CORE // All libraries need this
#define GATEWARE_ENABLE_SYSTEM // Graphics libs require system level libraries
#define GATEWARE_ENABLE_GRAPHICS // Enables all Graphics Libraries
// Ignore some GRAPHICS libraries we aren't going to use
#define GATEWARE_DISABLE_GDIRECTX11SURFACE // we have another template for this
#define GATEWARE_DISABLE_GOPENGLSURFACE // we have another template for this
#define GATEWARE_DISABLE_GVULKANSURFACE // we have another template for this
#define GATEWARE_DISABLE_GRASTERSURFACE // we have another template for this
// TODO: Part 2a
#define GATEWARE_ENABLE_MATH 
#define GATEWARE_ENABLE_AUDIO
// TODO: Part 4a
#define GATEWARE_ENABLE_INPUT 
// With what we want & what we don't defined we can include the API
#include "Gateware.h"
#include "renderer.h"
#include "FileSelection.h"

// open some namespaces to compact the code a bit
using namespace GW;
using namespace CORE;
using namespace SYSTEM;
using namespace GRAPHICS;
// lets pop a window and use D3D12 to clear to a jade colored screen
int main()
{
	GWindow win;
	GEventResponder msgs;
	GDirectX12Surface d3d12;
	GWindow winMinimap;
	GDirectX12Surface d3d12Minimap;

	float audioVolume = 0.0;
	GW::AUDIO::GAudio AudioM;
	GW::AUDIO::GAudio AudioS;
	AudioM.Create();
	AudioS.Create();

	GW::AUDIO::GMusic music;
	GW::AUDIO::GSound soundeffect;

	music.Create("../Sounds/Music.wav", AudioM, 0.1f);
	soundeffect.Create("../Sounds/coolsaber.wav", AudioS, 0.2);

	AudioM.PlayMusic();
	bool stoper = false;

	if (+win.Create(0, 0, 800, 600, GWindowStyle::WINDOWEDBORDERED) && +winMinimap.Create(0, 0, 250, 180, GWindowStyle::WINDOWEDBORDERED))
	{

		// TODO: Part 1a
		win.SetWindowName("Anthony Tejeda - LevelRender");
		float clr[] = { 0, 168/255.0f, 107/255.0f, 1 }; // start with a jade color
		msgs.Create([&](const GW::GEvent& e) {
			GW::SYSTEM::GWindow::Events q;
			if (+e.Read(q) && q == GWindow::Events::RESIZE)
				clr[0] += 0.01f; // move towards a orange as they resize
			
		});

		win.Register(msgs);
		winMinimap.Register(msgs);
		if (+d3d12.Create(win, GW::GRAPHICS::DEPTH_BUFFER_SUPPORT) && +d3d12Minimap.Create(winMinimap, GW::GRAPHICS::DEPTH_BUFFER_SUPPORT))
		{unsigned int Tempx = 0;
				
			
			FileParse();
			Renderer* renderer = new Renderer(win, d3d12); // init
			Renderer* renderer2 = new Renderer(winMinimap, d3d12Minimap); // init
		
			while (+win.ProcessWindowEvents() && +winMinimap.ProcessWindowEvents())
			{	
				unsigned int TempY = 0;
				unsigned int TempHeight = 0;
				unsigned int TempWidth = 0;

				win.GetX(Tempx);
				win.GetY(TempY);
				winMinimap.GetHeight(TempHeight);
				winMinimap.GetWidth(TempWidth);
				winMinimap.ReconfigureWindow(Tempx + 18, TempY + 34, TempWidth, TempHeight, GWindowStyle::WINDOWEDBORDERLESS);

				
				
				if (+d3d12.StartFrame() && +d3d12Minimap.StartFrame())
				{
					ID3D12GraphicsCommandList* cmd;
					D3D12_CPU_DESCRIPTOR_HANDLE rtv;
					D3D12_CPU_DESCRIPTOR_HANDLE dsv;

					ID3D12GraphicsCommandList* cmd1;
					D3D12_CPU_DESCRIPTOR_HANDLE rtv1;
					D3D12_CPU_DESCRIPTOR_HANDLE dsv1;
					if (+d3d12.GetCommandList((void**)&cmd) && 
						+d3d12.GetCurrentRenderTargetView((void**)&rtv) &&
						+d3d12.GetDepthStencilView((void**)&dsv) && +d3d12Minimap.GetCommandList((void**)&cmd1) &&
						+d3d12Minimap.GetCurrentRenderTargetView((void**)&rtv1) &&
						+d3d12Minimap.GetDepthStencilView((void**)&dsv1))
					{
						cmd->ClearRenderTargetView(rtv, clr, 0, nullptr);
						cmd->ClearDepthStencilView(dsv, D3D12_CLEAR_FLAG_DEPTH, 1, 0, 0, nullptr);

						cmd1->ClearRenderTargetView(rtv1, clr, 0, nullptr);
						cmd1->ClearDepthStencilView(dsv1, D3D12_CLEAR_FLAG_DEPTH, 1, 0, 0, nullptr);
						// TODO: Part 4b
					
					
						
						if (GetAsyncKeyState('E') & 0x0001)
						{
							
							AudioS.PlaySounds();
						}	
						if (GetAsyncKeyState(VK_F1))
						{
							FileTxTSwitchHolder = openfilename();
							std::string PlaceHolder;

						

							if (FileTxTSwitchHolder.find("GameLevel") != std::string::npos)
							{
								SwitchTxtFile = true;
								PlaceHolder = FileTxTSwitchHolder;
								//delete renderer;
								renderer->~Renderer();

								FileParse();
								renderer = new Renderer(win, d3d12);

							}
						
							if (!strcmp(FileTxTSwitchHolder.c_str(),""))
							{
								FileTxTSwitchHolder = PlaceHolder;
							}
					
						}

							
							renderer->UpdateCamera();
							renderer->Render(true); // 32
							
							//renderer2->UpdateCamera();
							renderer2->Render(false); // 32
						
						d3d12.EndFrame(false);
						cmd->Release();

						d3d12Minimap.EndFrame(false);
						cmd1->Release();
					}
				}
			}// clean-up when renderer falls off stack
		}
	}
	return 0; // that's all folks
}