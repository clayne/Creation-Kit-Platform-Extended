﻿// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "Core/Shaders.h"
#include "Core/D3D11Proxy.h"
#include "D3D11Patch.h"

#include <imgui.h>
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_dx11.h>

#include <shared_mutex>

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		decltype(&CreateDXGIFactory) ptrCreateDXGIFactory = nullptr;
		decltype(&D3D11CreateDeviceAndSwapChain) ptrD3D11CreateDeviceAndSwapChain = nullptr;
		ID3D11Device2* pointer_d3d11Device2Intf = nullptr;
		ID3D11Device* pointer_d3d11DeviceIntf = nullptr;
		IDXGISwapChain* pointer_dxgiSwapChain = nullptr;
		ID3D11DeviceContext* pointer_d3d11DeviceContext = nullptr;
		uintptr_t gGlobAddrDeviceContext = 0;
		//uintptr_t gOldCreateRenderTargetView = 0;
		//std::vector<ID3D11RenderTargetView*> pointer_mainRenderTargetView;
		ImFont* imguiFonts[3];

		static const ImWchar GlobalFontRanges[] =
		{
			0x0020, 0xFFFF, // ALL
			0,
		};

		D3D11Patch::D3D11Patch() : Module(GlobalEnginePtr), moduleDXGI(NULL), moduleD3D11(NULL)
		{}

		bool D3D11Patch::HasOption() const
		{
			return false;
		}

		bool D3D11Patch::HasCanRuntimeDisabled() const
		{
			return false;
		}

		const char* D3D11Patch::GetOptionName() const
		{
			return nullptr;
		}

		const char* D3D11Patch::GetName() const
		{
			return "D3D11 Patch";
		}

		bool D3D11Patch::HasDependencies() const
		{
			return false;
		}

		Array<String> D3D11Patch::GetDependencies() const
		{
			return {};
		}

		bool D3D11Patch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
			const char* lpcstrPlatformRuntimeVersion) const
		{
			if (eEditorCurrentVersion <= EDITOR_FALLOUT_C4_LAST)
			{
				auto SystemVersion = _engine->GetSystemVersion();
				// Win 8.1 and newer
				return ((SystemVersion.MajorVersion > 6) || 
					((SystemVersion.MajorVersion == 6) && (SystemVersion.MinorVersion == 3)));
			}

			return false;
		}

		bool D3D11Patch::Activate(const Relocator* lpRelocator,
			const RelocationDatabaseItem* lpRelocationDatabaseItem)
		{
			if (lpRelocationDatabaseItem->Version() == 1)
			{
				auto Section = GlobalEnginePtr->GetSection(SECTION_TEXT);
				auto Patterns = voltek::find_patterns(Section.base, Section.end - Section.base, "81 ? ? ? ? ? 00 B0 00 00");

				if (Patterns.size() != 1)
				{
					_ERROR("Can't find the D3D11 level check.");
					return false;
				}

				auto addr = Patterns[0];
				if (((uint8_t*)addr)[10] != 0x74)
				{
					_ERROR("Can't find the D3D11 level check.");
					return false;
				}

				{
					ScopeRelocator text;
					((uint8_t*)addr)[10] = 0xEB;
				}

				// Grab the original function pointers
				moduleDXGI = LoadLibraryExA("dxgi.dll", NULL, LOAD_LIBRARY_SEARCH_SYSTEM32);
				Assert(moduleDXGI);
				moduleD3D11 = LoadLibraryExA("d3d11.dll", NULL, LOAD_LIBRARY_SEARCH_SYSTEM32);
				Assert(moduleD3D11);

				*(FARPROC*)&ptrCreateDXGIFactory = GetProcAddress(moduleDXGI, "CreateDXGIFactory1");
				if (!ptrCreateDXGIFactory)
					*(FARPROC*)&ptrCreateDXGIFactory = GetProcAddress(moduleDXGI, "CreateDXGIFactory");
				*(FARPROC*)&ptrD3D11CreateDeviceAndSwapChain = GetProcAddress(moduleD3D11, "D3D11CreateDeviceAndSwapChain");

				AssertMsg(ptrCreateDXGIFactory, "CreateDXGIFactory import not found");
				AssertMsg(ptrD3D11CreateDeviceAndSwapChain, "D3D11CreateDeviceAndSwapChain import not found");

				PatchIAT(HKCreateDXGIFactory, "dxgi.dll", "CreateDXGIFactory");
				PatchIAT(HKD3D11CreateDeviceAndSwapChain, "d3d11.dll", "D3D11CreateDeviceAndSwapChain");

				return true;
			}

			return false;
		}

		bool D3D11Patch::Shutdown(const Relocator* lpRelocator,
			const RelocationDatabaseItem* lpRelocationDatabaseItem)
		{
			ImGui_ImplDX11_Shutdown();
			ImGui_ImplWin32_Shutdown();
			ImGui::DestroyContext();

			return false;
		}

		HRESULT WINAPI D3D11Patch::HKCreateDXGIFactory(REFIID riid, void** ppFactory)
		{
			if (SUCCEEDED(ptrCreateDXGIFactory(__uuidof(IDXGIFactory3), ppFactory)))
				return S_OK;

			if (SUCCEEDED(ptrCreateDXGIFactory(__uuidof(IDXGIFactory2), ppFactory)))
				return S_OK;

			return ptrCreateDXGIFactory(__uuidof(IDXGIFactory), ppFactory);
		}

		//HRESULT WINAPI D3D11Patch::HKD3D11CreateRenderTargetView(ID3D11Device* pDevice, 
		//	ID3D11Resource* pResource,
		//	const D3D11_RENDER_TARGET_VIEW_DESC* pDesc,
		//	ID3D11RenderTargetView** ppRTView)
		//{
		//	//if (pResource)
		//	//{
		//	//	Microsoft::WRL::ComPtr<ID3D11Texture2D> Tex;
		//	//	pResource->QueryInterface<ID3D11Texture2D>(Tex.GetAddressOf());

		//	//	D3D11_TEXTURE2D_DESC Desc;
		//	//	Tex->GetDesc(&Desc);


		//	//	//_MESSAGE("HKD3D11CreateRenderTargetView: %u %u", Desc.Width, Desc.Height);
		//	//}

		//	//if (!pointer_mainRenderTargetView)
		//	//{
		//		auto res = fastCall<HRESULT>(gOldCreateRenderTargetView, pDevice, pResource, pDesc, ppRTView);
		//		if (SUCCEEDED(res) && ppRTView)
		//		{
		//			pointer_mainRenderTargetView.push_back(*ppRTView);
		//			_MESSAGE("HKD3D11CreateRenderTargetView: %p %p", ppRTView, *ppRTView);
		//		}

		//		return res;
		//	//}

		//	//auto res = fastCall<HRESULT>(gOldCreateRenderTargetView, pDevice, pResource, pDesc, ppRTView);

		//	//return res;
		//}

		HRESULT WINAPI D3D11Patch::HKD3D11CreateDeviceAndSwapChain(
			IDXGIAdapter* pAdapter,
			D3D_DRIVER_TYPE DriverType,
			HMODULE Software,
			UINT Flags,
			const D3D_FEATURE_LEVEL* pFeatureLevels,
			UINT FeatureLevels,
			UINT SDKVersion,
			const DXGI_SWAP_CHAIN_DESC* pSwapChainDesc,
			IDXGISwapChain** ppSwapChain,
			ID3D11Device** ppDevice,
			D3D_FEATURE_LEVEL* pFeatureLevel,
			ID3D11DeviceContext** ppImmediateContext)
		{
			auto SwapChainDesc = const_cast<DXGI_SWAP_CHAIN_DESC*>(pSwapChainDesc);
			SwapChainDesc->BufferDesc.RefreshRate.Numerator = 60;
			SwapChainDesc->BufferDesc.RefreshRate.Denominator = 
				(UINT)_READ_OPTION_BOOL("CreationKit", "bRenderWindowVSync", true);

			//
			// From MSDN:
			//
			// If the Direct3D 11.1 runtime is present on the computer and pFeatureLevels is set to NULL,
			// this function won't create a D3D_FEATURE_LEVEL_11_1 device. To create a D3D_FEATURE_LEVEL_11_1
			// device, you must explicitly provide a D3D_FEATURE_LEVEL array that includes
			// D3D_FEATURE_LEVEL_11_1. If you provide a D3D_FEATURE_LEVEL array that contains
			// D3D_FEATURE_LEVEL_11_1 on a computer that doesn't have the Direct3D 11.1 runtime installed,
			// this function immediately fails with E_INVALIDARG.
			//
			const D3D_FEATURE_LEVEL testFeatureLevels[] =
			{
				D3D_FEATURE_LEVEL_11_1,
				D3D_FEATURE_LEVEL_11_0,
			};

			D3D_FEATURE_LEVEL level;
			HRESULT hr;

			for (INT i = 0; i < ARRAYSIZE(testFeatureLevels); i++) 
			{
				hr = NsightHack_D3D11CreateDeviceAndSwapChain(
					pAdapter,
					DriverType,
					Software,
					Flags,
					&testFeatureLevels[i],
					1,
					SDKVersion,
					pSwapChainDesc,
					ppSwapChain,
					ppDevice,
					&level,
					ppImmediateContext);

				if (SUCCEEDED(hr)) 
				{
					if (pFeatureLevel)
						*pFeatureLevel = level;

					break;
				}
			}

			if (FAILED(hr)) 
				AssertMsg(FALSE, "DirectX11 device creation failed. Creation Kit will now exit.");
			
			_MESSAGE("Created D3D11 device with feature level %X...", level);
			if (SUCCEEDED((*ppDevice)->QueryInterface<ID3D11Device2>(&pointer_d3d11Device2Intf))) 
			{
				// Force DirectX11.2 in case we use features later (11.3+ requires Win10 or higher)
				ID3D11Device2* proxyDevice = new D3D11DeviceProxy(*ppDevice);
				ID3D11DeviceContext2* proxyContext = new D3D11DeviceContextProxy(*ppImmediateContext);

				pointer_d3d11DeviceIntf = proxyDevice;
				*ppDevice = proxyDevice;
				pointer_d3d11DeviceContext = proxyContext;
				*ppImmediateContext = proxyContext;

				_MESSAGE("Force DirectX11.2");
			}
			else 
			{
				pointer_d3d11DeviceIntf = *ppDevice;
				pointer_d3d11DeviceContext = *ppImmediateContext;
			}

			(*ppDevice)->SetExceptionMode(D3D11_RAISE_FLAG_DRIVER_INTERNAL_ERROR);
			pointer_dxgiSwapChain = *ppSwapChain;

			/*auto vtableCreateRenderTargetView = reinterpret_cast<uintptr_t*>((uintptr_t)(*((uintptr_t*)(*ppDevice))) + (9 * sizeof(void*)));
			gOldCreateRenderTargetView = voltek::detours_jump(*vtableCreateRenderTargetView, (uintptr_t)&HKD3D11CreateRenderTargetView);*/

			IMGUI_CHECKVERSION();
			ImGui::CreateContext();

			ImGuiIO& io = ImGui::GetIO();
			io.ConfigFlags |= ImGuiConfigFlags_NoKeyboard;		
			io.ConfigFlags |= ImGuiConfigFlags_NoMouse;
			io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;		// Hide cursor

			io.Fonts->AddFontDefault();

			char path[MAX_PATH];
			if (FAILED(SHGetFolderPath(NULL, CSIDL_FONTS, NULL, SHGFP_TYPE_CURRENT, path)))
				return E_FAIL;
			String ps(path);
			
			imguiFonts[0] = io.Fonts->AddFontFromFileTTF((ps + "\\consola.ttf").c_str(), 12.0f, nullptr, GlobalFontRanges);
			imguiFonts[1] = io.Fonts->AddFontFromFileTTF((ps + "\\consolab.ttf").c_str(), 12.0f, nullptr, GlobalFontRanges);
			imguiFonts[2] = io.Fonts->AddFontFromFileTTF((ps + "\\consola.ttf").c_str(), 10.0f, nullptr, GlobalFontRanges);
			if (!imguiFonts[0] || !imguiFonts[1] || !imguiFonts[2])
				return E_FAIL;

			ImGui_ImplWin32_Init(pSwapChainDesc->OutputWindow);
			ImGui_ImplDX11_Init(*ppDevice, *ppImmediateContext);

			GlobalD3D11ShaderEngine = new D3D11ShaderEngine(*ppDevice, *ppImmediateContext);
			gGlobAddrDeviceContext = (uintptr_t)ppImmediateContext;

			return hr;
		}

		HRESULT WINAPI D3D11Patch::NsightHack_D3D11CreateDeviceAndSwapChain(
			IDXGIAdapter* pAdapter,
			D3D_DRIVER_TYPE DriverType,
			HMODULE Software,
			UINT Flags,
			const D3D_FEATURE_LEVEL* pFeatureLevels,
			UINT FeatureLevels,
			UINT SDKVersion,
			const DXGI_SWAP_CHAIN_DESC* pSwapChainDesc,
			IDXGISwapChain** ppSwapChain,
			ID3D11Device** ppDevice,
			D3D_FEATURE_LEVEL* pFeatureLevel,
			ID3D11DeviceContext** ppImmediateContext)
		{
			//
			// Nvidia NSight checks the return address of D3D11CreateDeviceAndSwapChain to see if it's
			// a blacklisted directx dll. "d3dx9_42.dll" happens to be in that list. So, now I need to
			// generate code which spoofs the return address to something random.
			//
			// NOTE: Do NOT touch rcx, rdx, r8, r9
			//
			class d3djmp : public Xbyak::CodeGenerator {
			public:
				d3djmp() : Xbyak::CodeGenerator() {
					push(rbx);
					push(rsi);
					push(rdi);
					sub(rsp, 0x60);
					mov(rax, qword[rsp + 0xD8]);
					mov(r10, qword[rsp + 0xD0]);
					mov(r11, qword[rsp + 0xC8]);
					mov(rbx, qword[rsp + 0xC0]);
					mov(rdi, qword[rsp + 0xB8]);
					mov(rsi, qword[rsp + 0xA0]);
					mov(qword[rsp + 0x58], rax);
					mov(eax, dword[rsp + 0xB0]);
					mov(qword[rsp + 0x50], r10);
					mov(qword[rsp + 0x48], r11);
					mov(qword[rsp + 0x40], rbx);
					mov(qword[rsp + 0x38], rdi);
					mov(dword[rsp + 0x30], eax);
					mov(eax, dword[rsp + 0xA8]);
					mov(dword[rsp + 0x28], eax);
					mov(qword[rsp + 0x20], rsi);

					mov(rax, (uintptr_t)ptrD3D11CreateDeviceAndSwapChain);
					call(rax);

					add(rsp, 0x60);
					pop(rdi);
					pop(rsi);
					pop(rbx);
					ret();
				}
			} hack;

			auto newPtr = hack.getCode<decltype(&D3D11CreateDeviceAndSwapChain)>();
			return newPtr(pAdapter, DriverType, Software, Flags, pFeatureLevels, FeatureLevels, SDKVersion, pSwapChainDesc, ppSwapChain, ppDevice, pFeatureLevel, ppImmediateContext);
		}
	}
}