// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <Windows.h>
#include <C:\Detours\src\detours.h>


using namespace std;



BOOL(WINAPI* TrueSetWindowTextW)(HWND hWnd, LPCWSTR lpString) = SetWindowTextW;

__declspec(dllexport) BOOL WINAPI MySetWindowTextW(HWND hWnd, LPCWSTR lpString)
{
		return TrueSetWindowTextW(hWnd, L"Установлен новый заголовок");
}


BOOL WINAPI DllMain(HINSTANCE hinst, DWORD dwReason, LPVOID reserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		DetourRestoreAfterWith();
		DetourTransactionBegin();

		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&(PVOID&)TrueSetWindowTextW, MySetWindowTextW);
		DetourTransactionCommit();
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourDetach(&(PVOID&)TrueSetWindowTextW, MySetWindowTextW);
		DetourTransactionCommit();
	}
	return TRUE;
}

