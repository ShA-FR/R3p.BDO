#pragma once

#pragma comment(lib, "Libs/lua5.1.lib")

#include "Lua/LuaIntf.h"

#ifdef DEBUG
#pragma comment(lib, "Libs/MinHook_MTd.lib")
#else
#pragma comment(lib, "Libs/MinHook_MT.lib")
#endif

#include <vector>

#include "MinHook/MinHook.h"
#include "Addresses.h"
#include <fstream>
#include <experimental/filesystem>


#include "Rendering/rImGui.h"
#include "D3D/D3D11.h"

#pragma comment(lib, "Libs/d3d11.lib")
#pragma comment(lib, "Libs/d3dcompiler.lib")
#pragma comment(lib, "Libs/dinput8.lib")

bool _imguiInit = false;
ID3D11Device *pDevice = NULL;
ID3D11DeviceContext *pContext = NULL;
__int64 _dxgiVTable = NULL;
__int64 _d3d11VTable = NULL;

typedef void(__fastcall* __lua_dostring)(lua_State* L, const char* str, __int64 strLength);
__lua_dostring olua_dostring = (__lua_dostring)_lua_dostring;

typedef const char *(__fastcall* __lua_tostring)(lua_State *L, int index);
__lua_tostring olua_tostring = (__lua_tostring)_lua_tostring;

typedef __int64(__fastcall* __lua_gettop)(lua_State* L);
__lua_gettop olua_gettop = reinterpret_cast<__lua_gettop>(_lua_gettop);
__lua_gettop fplua_gettop = NULL;

__int64 lastTick = 0;
std::vector<std::string> _queuedLuaCode;

void appendLineToFile(std::string line)
{
	std::string filepath;

	TCHAR buffer[MAX_PATH]; 

	auto readChars = GetModuleFileNameA(NULL, buffer, MAX_PATH);

	filepath = std::string(buffer, buffer + readChars) + "/lualog.txt";

    std::ofstream file;
    
    file.open(filepath, std::ios::out | std::ios::app);
    /*if (file.fail())
        throw std::ios_base::failure(std::strerror(errno));*/
	   
    file.exceptions(file.exceptions() | std::ios::failbit | std::ifstream::badbit);

    file << line << std::endl;
}

__int64 mlua_gettop(lua_State* L)
{
	if (_queuedLuaCode.size() > 0 && lastTick + 200 <= GetTickCount64())
	{
		lastTick = GetTickCount64();

		//LuaIntf::LuaState _L = LuaIntf::LuaState(L);

		std::string code = _queuedLuaCode.front();

		//_L.doString(code.c_str());

		olua_dostring(L, code.c_str(), code.length());
						
		try
		{
			if (_queuedLuaCode.size() > 0)
				_queuedLuaCode.erase(_queuedLuaCode.begin());
		}
		catch (std::exception &e)
		{

		}
	}

	return fplua_gettop(L);
}

typedef HRESULT(__stdcall *D3D11PresentHook) (IDXGISwapChain* This, UINT SyncInterval, UINT Flags);
static D3D11PresentHook oPresent = NULL;
static D3D11PresentHook fpPresent = NULL;

typedef BOOL(WINAPI *tGetMessageA)(LPMSG lpMsg, HWND  hWnd, UINT  wMsgFilterMin, UINT  wMsgFilterMax);
static tGetMessageA fpGetMessageA = NULL;

typedef BOOL(WINAPI *tGetMessageW)(LPMSG lpMsg, HWND  hWnd, UINT  wMsgFilterMin, UINT  wMsgFilterMax);
static tGetMessageW fpGetMessageW = NULL;

typedef BOOL(WINAPI *tPeekMessageA)(LPMSG lpMsg, HWND  hWnd, UINT  wMsgFilterMin, UINT  wMsgFilterMax, UINT  wRemoveMsg);
static tPeekMessageA fpPeekMessageA = NULL;

typedef BOOL(WINAPI *tPeekMessageW)(LPMSG lpMsg, HWND  hWnd, UINT  wMsgFilterMin, UINT  wMsgFilterMax, UINT  wRemoveMsg);
static tPeekMessageW fpPeekMessageW = NULL;

typedef BOOL(WINAPI *tTranslateMessage)(const MSG *lpMsg);
static tTranslateMessage fpTranslateMessage = NULL;

__declspec(selectany) int g_translateMessageIgnoreCounter = 0;

static BOOL WINAPI mTranslateMessage(const MSG *lpMsg)
{
	if (g_translateMessageIgnoreCounter > 0)
	{
		g_translateMessageIgnoreCounter--;
		return FALSE;
	}
	auto result = fpTranslateMessage(lpMsg);
	return result;
}

static BOOL WINAPI mGetMessageA(LPMSG lpMsg, HWND  hWnd, UINT  wMsgFilterMin, UINT  wMsgFilterMax)
{
	auto result = fpGetMessageA(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax);
	if (TranslateMessage(lpMsg))
		g_translateMessageIgnoreCounter++;
	if (result == TRUE && Rendering::OnWindowMessage(lpMsg))
		RtlZeroMemory(lpMsg, sizeof(MSG));
	return result;
}

static BOOL WINAPI mGetMessageW(LPMSG lpMsg, HWND  hWnd, UINT  wMsgFilterMin, UINT  wMsgFilterMax)
{
	auto result = fpGetMessageW(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax);
	if (TranslateMessage(lpMsg))
		g_translateMessageIgnoreCounter++;
	if (result == TRUE && Rendering::OnWindowMessage(lpMsg))
		RtlZeroMemory(lpMsg, sizeof(MSG));
	return result;
}

static BOOL WINAPI mPeekMessageA(LPMSG lpMsg, HWND  hWnd, UINT  wMsgFilterMin, UINT  wMsgFilterMax, UINT  wRemoveMsg)
{
	auto result = fpPeekMessageA(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax, wRemoveMsg);
	if (TranslateMessage(lpMsg))
		g_translateMessageIgnoreCounter++;
	if (result == TRUE && Rendering::OnWindowMessage(lpMsg))
		RtlZeroMemory(lpMsg, sizeof(MSG));
	return result;
}

static BOOL WINAPI mPeekMessageW(LPMSG lpMsg, HWND  hWnd, UINT  wMsgFilterMin, UINT  wMsgFilterMax, UINT  wRemoveMsg)
{
	auto result = fpPeekMessageW(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax, wRemoveMsg);
	if (TranslateMessage(lpMsg))
		g_translateMessageIgnoreCounter++;
	if (result == TRUE && Rendering::OnWindowMessage(lpMsg))
		RtlZeroMemory(lpMsg, sizeof(MSG));
	return result;
}

static HRESULT mPresent(IDXGISwapChain* This, UINT SyncInterval, UINT Flags)
{
	if (!_imguiInit)
	{
		This->GetDevice(__uuidof(pDevice), (void**)&pDevice);
		pDevice->GetImmediateContext(&pContext);

		_imguiInit = Rendering::InitIMGui(pDevice, pContext);
	}

	if (_imguiInit)
	{
		ID3D11Texture2D *pBackBuffer;
		This->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);//This = Swapchain

		ID3D11RenderTargetView *pRTV;


		pDevice->CreateRenderTargetView(pBackBuffer, NULL, &pRTV);


		pContext->OMSetRenderTargets(1, &pRTV, NULL);
		Rendering::Draw();//Rendering here

		D3D11_TEXTURE2D_DESC backBufferDesc;
		pBackBuffer->GetDesc(&backBufferDesc);

		pBackBuffer->Release();
	}

	return fpPresent(This, SyncInterval, Flags);
}

static __int64 GetVTableFunction(__int64 vTable, DWORD index)
{
	return *(__int64*)(vTable + (0x8 * index));
}

static void GenerateD3DVTables()
{
	HWND hwnd = GetForegroundWindow();

	if (hwnd != NULL)
	{
		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		memset(&swapChainDesc, 0, sizeof(swapChainDesc));
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 1;
		swapChainDesc.OutputWindow = hwnd;
		swapChainDesc.Windowed = TRUE;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		UINT flags = D3D11_CREATE_DEVICE_SINGLETHREADED;
		ID3D11Device *device = NULL;
		ID3D11DeviceContext *context = NULL;
		IDXGISwapChain *chain = NULL;
		D3D_FEATURE_LEVEL featureLvl = D3D_FEATURE_LEVEL_11_0;
		HRESULT res = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, &featureLvl, 1, D3D11_SDK_VERSION, &swapChainDesc, &chain, &device, NULL, &context);

		if (res == S_OK)
		{
			_dxgiVTable = *(__int64*)chain;
			_d3d11VTable = *(__int64*)device;
		}
	}
}

bool Load()
{
	/*GenerateD3DVTables();
	oPresent = reinterpret_cast<D3D11PresentHook>(GetVTableFunction(_dxgiVTable, 8));*/

	if (MH_Initialize() != MH_OK)
	{
		return false;
	}

	//if (MH_CreateHook(&GetMessageA, mGetMessageA, reinterpret_cast<LPVOID*>(&fpGetMessageA)) != MH_OK)
	//{
	//	return false;
	//}

	//if (MH_CreateHook(&GetMessageW, mGetMessageW, reinterpret_cast<LPVOID*>(&fpGetMessageW)) != MH_OK)
	//{
	//	return false;
	//}

	//if (MH_CreateHook(&PeekMessageA, mPeekMessageA, reinterpret_cast<LPVOID*>(&fpPeekMessageA)) != MH_OK)
	//{
	//	return false;
	//}

	//if (MH_CreateHook(&PeekMessageW, mPeekMessageW, reinterpret_cast<LPVOID*>(&fpPeekMessageW)) != MH_OK)
	//{
	//	return false;
	//}

	//if (MH_CreateHook(&TranslateMessage, mTranslateMessage, reinterpret_cast<LPVOID*>(&fpTranslateMessage)) != MH_OK)
	//{
	//	return false;
	//}

	//if (MH_CreateHook(oPresent, mPresent, reinterpret_cast<LPVOID*>(&fpPresent)) != MH_OK)
	//{
	//	return false;
	//}

	if (MH_CreateHook(olua_gettop, mlua_gettop, reinterpret_cast<LPVOID*>(&fplua_gettop)) != MH_OK)
	{
		return false;
	}

	if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
	{
		return false;
	}

	return true;
}

bool Unload()
{
	if (MH_DisableHook(MH_ALL_HOOKS) != MH_OK)
	{
		return false;
	}

	if (MH_Uninitialize() != MH_OK)
	{
		return false;
	}

	return true;
}