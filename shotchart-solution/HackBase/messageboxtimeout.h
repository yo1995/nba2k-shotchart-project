#pragma once

#include "trainerbase.h"
#include <tchar.h>

//Functions & other definitions required-->
typedef int(__stdcall *MSGBOXAAPI)(IN HWND hWnd,
	IN LPCSTR lpText, IN LPCSTR lpCaption,
	IN UINT uType, IN WORD wLanguageId, IN DWORD dwMilliseconds);
typedef int(__stdcall *MSGBOXWAPI)(IN HWND hWnd,
	IN LPCWSTR lpText, IN LPCWSTR lpCaption,
	IN UINT uType, IN WORD wLanguageId, IN DWORD dwMilliseconds);

int MessageBoxTimeoutA(IN HWND hWnd, IN LPCSTR lpText,
	IN LPCSTR lpCaption, IN UINT uType,
	IN WORD wLanguageId, IN DWORD dwMilliseconds);
int MessageBoxTimeoutW(IN HWND hWnd, IN LPCWSTR lpText,
	IN LPCWSTR lpCaption, IN UINT uType,
	IN WORD wLanguageId, IN DWORD dwMilliseconds);

#ifdef UNICODE
#define MessageBoxTimeout MessageBoxTimeoutW
#else
#define MessageBoxTimeout MessageBoxTimeoutA
#endif 

#define MB_TIMEDOUT 32000

int MessageBoxTimeoutA(HWND hWnd, LPCSTR lpText,
	LPCSTR lpCaption, UINT uType, WORD wLanguageId,
	DWORD dwMilliseconds)
{
	static MSGBOXAAPI MsgBoxTOA = NULL;

	if (!MsgBoxTOA)
	{
		HMODULE hUser32 = GetModuleHandle(_T("user32.dll"));
		if (hUser32)
		{
			MsgBoxTOA = (MSGBOXAAPI)GetProcAddress(hUser32,
				"MessageBoxTimeoutA");
			//fall through to 'if (MsgBoxTOA)...'
		}
		else
		{
			//stuff happened, add code to handle it here 
			//(possibly just call MessageBox())
			return 0;
		}
	}

	if (MsgBoxTOA)
	{
		return MsgBoxTOA(hWnd, lpText, lpCaption,
			uType, wLanguageId, dwMilliseconds);
	}

	return 0;
}

int MessageBoxTimeoutW(HWND hWnd, LPCWSTR lpText,
	LPCWSTR lpCaption, UINT uType, WORD wLanguageId, DWORD dwMilliseconds)
{
	static MSGBOXWAPI MsgBoxTOW = NULL;

	if (!MsgBoxTOW)
	{
		HMODULE hUser32 = GetModuleHandle(_T("user32.dll"));
		if (hUser32)
		{
			MsgBoxTOW = (MSGBOXWAPI)GetProcAddress(hUser32,
				"MessageBoxTimeoutW");
			//fall through to 'if (MsgBoxTOW)...'
		}
		else
		{
			//stuff happened, add code to handle it here 
			//(possibly just call MessageBox())
			return 0;
		}
	}

	if (MsgBoxTOW)
	{
		return MsgBoxTOW(hWnd, lpText, lpCaption,
			uType, wLanguageId, dwMilliseconds);
	}

	return 0;
}
//End required definitions <--
