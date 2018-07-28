#include "hackbase.h"

D3D9Hook *mSingleton = 0;
HackBase *mD3D9Base = 0;

bool bCompare(LPBYTE lpData, LPBYTE lpMask, LPSTR szMask) {
	if (lpData == nullptr || lpMask == nullptr || szMask == nullptr) return FALSE;
	for (; *szMask; ++szMask, ++lpData, ++lpMask)
		if (*szMask == 'x' && *lpData != *lpMask) return FALSE;
	return ((*szMask) == 0) ? true : false;
}

DWORD FindPattern(DWORD dwAddress, DWORD dwLen, BYTE *bMask, char * szMask) {
	DWORD dwBack;
	VirtualProtect((LPVOID)dwAddress, dwLen, PAGE_EXECUTE_READWRITE, &dwBack);
	for (DWORD i = 0; i < dwLen; i++) {
		if (bCompare((BYTE*)(dwAddress + i), bMask, szMask)) {
			VirtualProtect((LPVOID)dwAddress, dwLen, dwBack, &dwBack);
			return (DWORD)(dwAddress + i);
		}
	}
	VirtualProtect((LPVOID)dwAddress, dwLen, dwBack, &dwBack);
	return 0;
}

D3D9Hook *D3D9Hook::Singleton() {
	mD3D9Base = HackBase::Singleton();
	if(!mSingleton) {
		if(mD3D9Base)
			mSingleton = new D3D9Hook();
		else
			dbglogln("Error creating D3D9Hook!");
	}
	return mSingleton;
}

typedef HRESULT(__stdcall* D3D9EndScene_t)(LPDIRECT3DDEVICE9);
D3D9EndScene_t pD3D9EndScene;

DWORD EndScene = NULL;
DWORD ResetAddr = NULL;
DWORD Device = NULL;

HRESULT __stdcall hkD3D9EndScene(IDirect3DDevice9 *pDevice) {
	__asm pushad

	static bool FirstRun = true;
	if(FirstRun) {
		FirstRun = false;
		if(mD3D9Base) {
			mD3D9Base->setD3DVersion(D3DVersion_D3D9);
			mD3D9Base->setRenderer(new D3D9Renderer(pDevice));
		}
	} else {
		if(mD3D9Base->getRenderer()) {
			mD3D9Base->getRenderer()->BeginScene();

			((D3D9Renderer*)mD3D9Base->getRenderer())->RefreshData(pDevice);

			if(mD3D9Base->mOnRender)
				mD3D9Base->mOnRender(mD3D9Base->getRenderer());
			
			mD3D9Base->getRenderer()->EndScene();
			
		} else {
			dbglogln("Unable to render: Renderer == NULL");
			MessageBox(0, "Error! Unable to render.", "", 0);
			ExitProcess(0);
		}
	}

	__asm popad
	return pD3D9EndScene(pDevice);
}


typedef HRESULT(WINAPI* oReset)(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
oReset pReset;

HRESULT __stdcall myReset(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters) {
	_asm pushad;

	mD3D9Base->getRenderer()->PreLost();
	HRESULT hRet = pReset(pDevice, pPresentationParameters);
	if (SUCCEEDED(hRet)) {
		mD3D9Base->getRenderer()->PostLost();
	}

	_asm popad;
	return hRet;
}

DWORD *D3D9VTable = NULL;

LRESULT CALLBACK D3D9MsgProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam){return DefWindowProc(hwnd, uMsg, wParam, lParam);}

D3D9Hook::D3D9Hook() {
	if(Imports::Singleton()->Direct3DCreate9) {
		DWORD D3D = 0;
		while (!D3D) {
			D3D = (DWORD)LoadLibraryA("C:\\WINDOWS\\system32\\d3d9.dll");  // well will it work on other platform?? I assume yes but we'll see...maybe include the original file in released version.
			Sleep(10);
		}
		DWORD Table = FindPattern(D3D, 0x127850, (BYTE*)"\xC7\x06\x00\x00\x00\x00\x89\x86\x00\x00\x00\x00\x89\x86", "xx????xx????xx"); // mov register, [ADDY] -> c7 0something 44 33 22 11
		DWORD* D3D9VTable = *(DWORD**)(Table + 2);

		// thought used by many apps like CE, OBS, the following hook is not good. especially with fullscrenn games. changed to findpattern method.
		// highly depends on CreateDevice.
		/*
		WNDCLASSEXA wc = {sizeof(WNDCLASSEX),CS_CLASSDC,D3D9MsgProc,0L,0L,GetModuleHandleA(NULL),NULL,NULL,NULL,NULL,"DX",NULL};
		if (RegisterClassExA(&wc) == 0) {
			dbglogln("Failed to register window!");
			MessageBox(0, "Failed to register window!", "Error!", 0);
			ExitProcess(0);
		}
		HWND hWnd = CreateWindowA("DX",NULL,WS_OVERLAPPEDWINDOW,100,100,300,300,NULL,NULL,wc.hInstance,NULL);  // parent -> GetDesktopWindow()
		if (hWnd == NULL) {
			dbglogln("Failed to create window!");
			MessageBox(0, "Failed to create window!", "Error!", 0);
			ExitProcess(0);
		}
		LPDIRECT3D9 pD3D = Imports::Singleton()->Direct3DCreate9( D3D_SDK_VERSION );
		D3DPRESENT_PARAMETERS d3dpp;
		ZeroMemory(&d3dpp, sizeof(d3dpp));
		d3dpp.Windowed = TRUE;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
		IDirect3DDevice9 *pd3dDevice;
		if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pd3dDevice))) {
			dbglogln("Failed to create device!");
			MessageBox(0, "Failed to create device!", "Error!", 0);
			pD3D->Release();
			DestroyWindow(hWnd);
			ExitProcess(0);
		}
		DWORD* pVTable = (DWORD*)pd3dDevice;
		pVTable = (DWORD*)pVTable[0];
		D3D9VTable = pVTable;
		pD3D->Release();
		pd3dDevice->Release();
		DestroyWindow(hWnd);
		*/

		EndScene = D3D9VTable[42];  // EndScene is #43 func of IDirect3DDevice9
		ResetAddr = D3D9VTable[16];  // Reset is #17 func of IDirect3DDevice9
	} else {
		dbglogln("Failed to create D3D9 hook!");
		MessageBox(0, "Failed to create D3D9 hook!", "Error!", 0);
		ExitProcess(0);
	}

	DWORD hookAddress = CheckPointer(EndScene);
#ifdef DEBUGLOG
	if(hookAddress != EndScene) {
		dbglogln("Hooking IDirect3DDevice9::EndScene\nat [0x%08X -> 0x%08X]", EndScene, hookAddress);
	} else {
		dbglogln("Hooking IDirect3DDevice9::EndScene\nat [0x%08X]", EndScene);
	}
#endif
	pD3D9EndScene = (D3D9EndScene_t)DetourCreate((LPVOID)hookAddress, hkD3D9EndScene, DETOUR_TYPE_PUSH_RET);

	pReset = (oReset)DetourCreate((BYTE*)ResetAddr, (BYTE*)&myReset, 5);
}

D3D9Hook::~D3D9Hook() {
	if(pD3D9EndScene) {
#ifdef DEBUGLOG
		dbglogln("Removing IDirect3DDevice9::EndScene public hook");
#endif
		DetourRemove(pD3D9EndScene);
		pD3D9EndScene = NULL;
	}
}
