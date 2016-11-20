// DLLHook.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <vector>
#define EXPORT  __declspec(dllexport)
#define MAX_LEN_SYSKEY 6
#define MAX_LEN_DIR 200
struct ComboKey
{
	int oldSystemKey1;
	TCHAR sOldSystemKey1[MAX_LEN_SYSKEY];
	int oldSystemKey2;
	TCHAR sOldSystemKey2[MAX_LEN_SYSKEY];
	int oldKey;
	TCHAR sOldKey[2];

	int newSystemKey1;
	TCHAR sNewSystemKey1[MAX_LEN_SYSKEY];
	int newSystemKey2;
	TCHAR sNewSystemKey2[MAX_LEN_SYSKEY];
	int newKey;
	TCHAR sNewKey[2];

	TCHAR dirName[MAX_LEN_DIR];
};

HINSTANCE hInstLib;
HHOOK hHook;
std::vector<ComboKey> * listCbKey;
bool isRunningHook = false;


void SendKey(BYTE vk, BOOL Release = FALSE)
{
	if (Release)
	{
		if (vk == VK_CONTROL)
			keybd_event(vk, 0x45, KEYEVENTF_KEYUP, 0);
		else
			keybd_event(vk, 0x45, KEYEVENTF_KEYUP, 0);
	}
	else
		keybd_event(vk, 0x45, KEYEVENTF_EXTENDEDKEY, 0);

}
bool isKeyNormaldown(int lParam)
{
	return !((lParam >> 31) & 1);
}
bool isSysKeydown(int key)
{
	return (GetAsyncKeyState(key) & 0x8000) != 0;

}
bool isAltDown(int lParam)
{
	return ((lParam >> 29) & 1);
}

void processNewCombo(int sys1, int sys2, int key, TCHAR *nameDir)
{
	
	//complete test
	if (sys1 < 0 && sys2 < 0 && key > 0)
	{
		SendKey(key);
		SendKey(key,true);
		return;
	}
	//complete test \ ALT
	if (sys1 > 0 && sys2 < 0 && key > 0)
	{
		SendKey(sys1);
		SendKey(key);

		keybd_event(sys1, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
		keybd_event(key, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
		return;
	}
	//complete test \ ALT
	if (sys1 < 0 && sys2 > 0 && key > 0)
	{
		SendKey(sys2);
		SendKey(key);

		//SendKey(sys2, true);
		keybd_event(sys2, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
		SendKey(key, true);
		return;
	}
	//complete test \ ALT
	if (sys1 > 0 && sys2 > 0 && key > 0)
	{
		SendKey(sys1);
		SendKey(sys2);
		SendKey(key);

		//SendKey(sys1, true);
		//SendKey(sys2, true);
		keybd_event(sys1, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
		keybd_event(sys2, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
		SendKey(key, true);
		return;
	}
	if (nameDir)
		if (wcslen(nameDir) > 0)
		{
			STARTUPINFO si;
			PROCESS_INFORMATION pi;
			ZeroMemory(&si, sizeof(si));
			si.cb = sizeof(si);
			ZeroMemory(&pi, sizeof(pi));

			CreateProcess(NULL,   // No module name (use command line)
				nameDir,        // Command line
				NULL,           // Process handle not inheritable
				NULL,           // Thread handle not inheritable
				FALSE,          // Set handle inheritance to FALSE
				0,              // No creation flags
				NULL,           // Use parent's environment block
				NULL,           // Use parent's starting directory 
				&si,            // Pointer to STARTUPINFO structure
				&pi);       // Pointer to PROCESS_INFORMATION structure
		}

}
bool processHook(KBDLLHOOKSTRUCT* pkbhs, WPARAM wParam, int oldSys1, int oldSys2, int oldKey, int newSys1, int newSys2, int newKey, TCHAR * nameDir)
{
	if (oldSys1 > 0 && oldSys2 > 0 && oldKey > 0)
	{
		if (isSysKeydown(oldSys1) && isSysKeydown(oldSys2) && pkbhs->vkCode == oldKey && wParam == WM_KEYDOWN)
		{
			//Key up...
			SendKey(oldSys1, true);
			SendKey(oldSys2, true);
			SendKey(oldKey, true);

			//process new combooldKey
			processNewCombo(newSys1, newSys2, newKey, nameDir);
			return 1;
		}
	}

	if (oldSys1 < 0 && oldSys2 > 0 && oldKey > 0)
	{
		if (isSysKeydown(oldSys2) && pkbhs->vkCode == oldKey && wParam == WM_KEYDOWN)
		{
			//Key up...
			SendKey(oldSys2, true);
			SendKey(oldKey, true);

			//process new combooldKey
			processNewCombo(newSys1, newSys2, newKey, nameDir);
			return 1;
		}
	}

	if (oldSys1 > 0 && oldSys2 < 0 && oldKey > 0)
	{
		if (isSysKeydown(oldSys1) && pkbhs->vkCode == oldKey && wParam == WM_KEYDOWN)
		{
			//Key up...
			SendKey(oldSys1, true);
			SendKey(oldKey, true);

			//process new combooldKey
			processNewCombo(newSys1, newSys2, newKey, nameDir);
			return 1;
		}
	}


	if (oldSys1 > 0 && oldSys2 > 0 && oldKey < 0)
	{
		if (isSysKeydown(oldSys1) && isSysKeydown(oldSys2))
		{
			//Key up...
			SendKey(oldSys1, true);
			SendKey(oldSys2, true);

			//process new combooldKey
			processNewCombo(newSys1, newSys2, newKey, nameDir);
			return 1;
		}
	}

	if (oldSys1 > 0 && oldSys2 < 0 && oldKey < 0)
	{
		if (isSysKeydown(oldSys1))
		{
			//Key up...
			SendKey(oldSys1, true);

			//process new combooldKey
			processNewCombo(newSys1, newSys2, newKey, nameDir);
			return 1;
		}
	}

	if (oldSys1 < 0 && oldSys2 > 0 && oldKey < 0)
	{
		if (isSysKeydown(oldSys2))
		{
			//Key up...
			SendKey(oldSys2, true);

			//process new combooldKey
			processNewCombo(newSys1, newSys2, newKey, nameDir);
			return 1;
		}
	}

	if (oldSys1 < 0 && oldSys2 < 0 && oldKey > 0)
	{
		if (pkbhs->vkCode == oldKey && wParam == WM_KEYDOWN)
		{
			//Key up...
			SendKey(oldKey, true);
			
			//process new combooldKey
			processNewCombo(newSys1, newSys2, newKey, nameDir);
			return 1;
		}
	}
	
	return false;
}
LRESULT CALLBACK KeyboardProcess(int nCode, WPARAM wParam, LPARAM lParam)
 {
	 if (nCode < 0 || isRunningHook)
		return CallNextHookEx(hHook, nCode, wParam, lParam);

	
	/*
	if (isSysKeydown(VK_SHIFT) && isSysKeydown(VK_MENU) && (wParam == 'T') && isKeyNormaldown(lParam)) {
		SendKey(VK_SHIFT, true);
		//keybd_event(VK_SHIFT, 0x45, KEYEVENTF_KEYUP, 0);

		SendKey(VK_MENU, true);
		//keybd_event(VK_MENU, 0x45, KEYEVENTF_KEYUP, 0);
		SendKey('T', true);
		
		SendKey(VK_CONTROL);
		SendKey(int('A'));
		SendKey(VK_CONTROL, true);
		return 1;
	}
	*/
	/*

	int sys1, sys2, key;
	sys1 = comboKey[0];
	sys2 = comboKey[1];
	key = comboKey[2];
	//sys1 = sys2 = key = 1;
 	if (sys1 > 0 && sys2 > 0 && key > 0)
	{
		
		if (isSysKeydown(VK_SHIFT) && isSysKeydown(VK_MENU) && (wParam == 'T') && isKeyNormaldown(lParam)) {
			SendKey(VK_SHIFT, true);
			//keybd_event(VK_SHIFT, 0x45, KEYEVENTF_KEYUP, 0);

			SendKey(VK_MENU, true);
			//keybd_event(VK_MENU, 0x45, KEYEVENTF_KEYUP, 0);
			SendKey('T', true);

			SendKey(VK_CONTROL);
			SendKey(int('A'));
			SendKey(VK_CONTROL, true);
			return 1;
		}
	}
	*/

	KBDLLHOOKSTRUCT* pkbhs = (KBDLLHOOKSTRUCT*)lParam;
	
	int nHook = listCbKey->size();
	int oldSys1, oldSys2, oldKey, newSys1, newSys2, newKey;
	TCHAR *nameDir = NULL;

	for (int i = 0; i < nHook; i++)
	{	
		oldSys1 = listCbKey->operator[](i).oldSystemKey1;
		oldSys2 = listCbKey->operator[](i).oldSystemKey2;
		oldKey = listCbKey->operator[](i).oldKey;
		newSys1 = listCbKey->operator[](i).newSystemKey1;
		newSys2 = listCbKey->operator[](i).newSystemKey2;
		newKey = listCbKey->operator[](i).newKey;
		nameDir = listCbKey->operator[](i).dirName;
		isRunningHook = true;
		if (processHook(pkbhs, wParam, oldSys1, oldSys2, oldKey, newSys1, newSys2, newKey, nameDir))
		{
			isRunningHook = false;
			return 1;
		}
			
		isRunningHook = false;
	}

	
	return CallNextHookEx(hHook, nCode, wParam, lParam);
}

EXPORT void doInstallHookKeyboard(std::vector<ComboKey> * list)
{
	if (hHook != NULL)
	{
		return;
	}
	listCbKey = list;
	
	hHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)KeyboardProcess, hInstLib, 0);
	//MessageBox(hWnd, L"Install hook successfully", L"Result", MB_OK);
}

EXPORT void doRemoveHookKeyboard()
{
	if (hHook == NULL)
	{
		return;
	}
	
	UnhookWindowsHookEx(hHook);
	listCbKey = NULL;
	hHook = NULL;
	//MessageBox(hWnd, L"Remove hook successfully", L"Result", MB_OK);
}