#include "stdafx.h"
#include"HookKeyboard.h"


HKeyboard_HL::HKeyboard_HL(HWND hWnd)
{
	this->_hWnd = hWnd;
	this->_typeHook = WH_KEYBOARD;
	this->isFirst = true;
}
bool HKeyboard_HL::isFirstInstall()
{
	return this->isFirst;
}
BOOL HKeyboard_HL::Install(std::vector<ComboKey> * listCbKey)
{
	// gọi hàm DLL theo kiểu Run-time
	// Định nghĩa prototype của hàm
	typedef VOID(*MYPROC)(std::vector<ComboKey> *);

	HINSTANCE hinstLib;
	MYPROC ProcAddr;  

	// load DLL và lấy handle của DLL module
	hinstLib = LoadLibrary(L"DLLHook.dll");
	// Nếu load thành công, lấy địa chỉ của hàm DrawCircle trong DLL
	if (hinstLib != NULL) {
		ProcAddr = (MYPROC)GetProcAddress(hinstLib, "doInstallHookKeyboard");
		// Nếu lấy được địa chỉ hàm, gọi thực hiện hàm
		if (ProcAddr != NULL)
		{
			ProcAddr(listCbKey);
			isFirst = false;
			return TRUE;
		}
	}

	return FALSE;
}
void HKeyboard_HL::Remove()
{
	// gọi hàm DLL theo kiểu Run-time
	// Định nghĩa prototype của hàm
	typedef VOID(*MYPROC)();

	HINSTANCE hinstLib;
	MYPROC ProcAddr;

	// load DLL và lấy handle của DLL module
	hinstLib = LoadLibrary(L"DLLHook.dll");
	// Nếu load thành công, lấy địa chỉ của hàm DrawCircle trong DLL
	if (hinstLib != NULL) {
		ProcAddr = (MYPROC)GetProcAddress(hinstLib, "doRemoveHookKeyboard");
		// Nếu lấy được địa chỉ hàm, gọi thực hiện hàm
		if (ProcAddr != NULL)
			ProcAddr();
	}
}

HKeyboard_HL::~HKeyboard_HL()
{
	DeleteObject(this->_hWnd);
}