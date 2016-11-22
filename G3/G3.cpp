// G3.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "G3.h"
#include "HookKeyboard.h"
#include "resource.h"
#include <string>
#include <commdlg.h>

//Use this later, not now
/*
#include <stdio.h> //load list process ID
#include <tchar.h> //load list process ID
#include <psapi.h> //load list process ID
*/


enum SYSKEY { CTRL, ALT, SHIFT };


//Refactor: not use global varialbe but later
// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name 

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
INT_PTR CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
void getListCbKey(std::vector<ComboKey> *&);
void addNewCbKey(ComboKey);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_G3, szWindowClass, MAX_LOADSTRING);
	//MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_G3));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   
HWND hWnd;
   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG1), 0, WndProc);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
// To ensure correct resolution of symbols, add Psapi.lib to TARGETLIBS
// and compile with -DPSAPI_VERSION=1

void InitCombobox(HWND hCmbSysKey1)
{
	SendMessage(hCmbSysKey1, CB_ADDSTRING, 0, (LPARAM)L"");
	SendMessage(hCmbSysKey1, CB_ADDSTRING, 0, (LPARAM)SYSKEY_CTRL);
	//SendMessage(hCmbSysKey1, CB_ADDSTRING, 0, (LPARAM)SYSKEY_ALT);
	SendMessage(hCmbSysKey1, CB_ADDSTRING, 0, (LPARAM)SYSKEY_SHIFT);
}

void onInitDialog(HWND &hDlg, HWND &hCmbSysKey1, HWND &hCmbSysKey2, HWND &hCmbSysKey3, HWND &hCmbSysKey4, HWND &hEdTOldKey, HWND &hEdTNewKey, HWND &hListOld, HWND &hListNew, HWND &hEdtDir)
{
	hEdTOldKey = GetDlgItem(hDlg, IDC_OLDKEY);
	hEdTNewKey = GetDlgItem(hDlg, IDC_NEWKEY);
	hCmbSysKey1 = GetDlgItem(hDlg, IDC_CMB_SYSKEY1);
	hCmbSysKey2 = GetDlgItem(hDlg, IDC_CMB_SYSKEY2);
	hCmbSysKey3 = GetDlgItem(hDlg, IDC_CMB_SYSKEY3);
	hCmbSysKey4 = GetDlgItem(hDlg, IDC_CMB_SYSKEY4);
	hListOld = GetDlgItem(hDlg, IDC_LIST_OLD);
	hListNew = GetDlgItem(hDlg, IDC_LIST_NEW);
	hEdtDir = GetDlgItem(hDlg, IDC_DIR);

	InitCombobox(hCmbSysKey1);
	InitCombobox(hCmbSysKey2);
	InitCombobox(hCmbSysKey3);
	InitCombobox(hCmbSysKey4);
}

BOOL TriggerCombobox(HWND hCmbSysKey1, HWND hCmbSysKey2)
{
	TCHAR buffer1[MAX_LEN_SYSKEY];
	TCHAR buffer2[MAX_LEN_SYSKEY];

	GetWindowText(hCmbSysKey1, buffer1, MAX_LEN_SYSKEY);
	GetWindowText(hCmbSysKey2, buffer2, MAX_LEN_SYSKEY);

	//If user no choose system key
	if (wcslen(buffer1) == 0 && wcslen(buffer2) == 0)
		return TRUE;

	if (wcscmp(buffer1, buffer2) == 0)
		return FALSE;
	return TRUE;
}

// check if user input more than 2 character
BOOL TriggerEditText(HWND hEditText)
{
	TCHAR buffer[MAX_LOADSTRING];

	GetWindowText(hEditText, buffer, MAX_LOADSTRING);
	if (wcslen(buffer) > 1)
		return FALSE;
	
	return TRUE;
}

void mergeStringComboKey(int sysKey1, int sysKey2, int key, TCHAR *sSysKey1, TCHAR *sSysKey2, TCHAR *sKey, HWND hList)
{
	TCHAR buffer[MAX_LOADSTRING];
	TCHAR tmp[MAX_LOADSTRING];
	buffer[0] = '\0';

	if (sysKey1 > 0)
	{
		tmp[0] = '\0';
		wsprintf(tmp, TEXT("%s + "), sSysKey1);
		wcscat_s(buffer, tmp);
	}
	if (sysKey2 > 0)
	{
		tmp[0] = '\0';
		wsprintf(tmp, TEXT("%s + "), sSysKey2);
		wcscat_s(buffer, tmp);
	}
	if (key > 0)
	{
		tmp[0] = '\0';
		wsprintf(tmp, TEXT("%s"), sKey);
		wcscat_s(buffer, tmp);
	}
	SendMessage(hList, LB_ADDSTRING, NULL, (LPARAM)buffer);
}

void UpdateListbox(HWND hListOld, HWND hListNew, ComboKey cbKey)
{
	mergeStringComboKey(cbKey.oldSystemKey1, cbKey.oldSystemKey2, cbKey.oldKey, cbKey.sOldSystemKey1, cbKey.sOldSystemKey2, cbKey.sOldKey, hListOld);
	if (wcslen(cbKey.dirName) < 1)
		mergeStringComboKey(cbKey.newSystemKey1, cbKey.newSystemKey2, cbKey.newKey, cbKey.sNewSystemKey1, cbKey.sNewSystemKey2, cbKey.sNewKey, hListNew);
	else
		SendMessage(hListNew, LB_ADDSTRING, NULL, (LPARAM)cbKey.dirName);
}

void ResetInput(HWND &hCmbSysKey1, HWND &hCmbSysKey2, HWND &hCmbSysKey3, HWND &hCmbSysKey4, HWND &hEdTOldKey, HWND &hEdTNewKey, HWND &hEdTDir)
{
	SetWindowText(hEdTOldKey, TEXT(""));
	SetWindowText(hEdTNewKey, TEXT(""));
	SetWindowText(hEdTDir, TEXT(""));

	SendMessage(hCmbSysKey1, CB_SETCURSEL, 0, 0);
	SendMessage(hCmbSysKey2, CB_SETCURSEL, 0, 0);
	SendMessage(hCmbSysKey3, CB_SETCURSEL, 0, 0);
	SendMessage(hCmbSysKey4, CB_SETCURSEL, 0, 0);
}

void onRemoveOneHook(HWND hOldList, HWND hNewList, std::vector<ComboKey> *&listCbKey)
{
	int curInd;

	//get current hook chose
	curInd = SendMessage(hOldList, LB_GETCURSEL, 0, 0);

	//delete in list (in dialog)
	SendMessage(hOldList, LB_DELETESTRING, curInd, 0);
	SendMessage(hNewList, LB_DELETESTRING, curInd, 0);

	//delete in listHook
	listCbKey->erase(listCbKey->begin() + curInd);
}

void changeUI(HWND hDlg, int type)
{
	HWND hTmp;
	HMENU hMenu;
	int status1, status2;
	RECT rect;
	int width, height;

	GetWindowRect(hDlg, &rect);
	width = rect.right - rect.left;
	height = rect.bottom - rect.top;
	hMenu = GetMenu(hDlg);
	if (type == 1)
	{
		status2 = SW_HIDE;
		status1 = SW_SHOW;
		//SetWindowPos(hDlg, HWND_NOTOPMOST, rect.left, rect.top, width, height + 50, SWP_NOMOVE);
		CheckMenuItem(hMenu, ID_OPTIONS_REPLACEKEYS, MF_CHECKED | MF_BYCOMMAND);
		EnableMenuItem(hMenu, ID_OPTIONS_REPLACEKEYS, MF_DISABLED);
		EnableMenuItem(hMenu, ID_OPTIONS_SHORTCUT, MF_ENABLED);
		CheckMenuItem(hMenu, ID_OPTIONS_SHORTCUT, MF_UNCHECKED | MF_BYCOMMAND);
	}
	else if (type == 2)
	{
		status1 = SW_HIDE;
		status2 = SW_SHOW;
		//SetWindowPos(hDlg, HWND_NOTOPMOST, rect.left, rect.top, width, height - 50, SWP_NOMOVE);
		CheckMenuItem(hMenu, ID_OPTIONS_SHORTCUT, MF_CHECKED | MF_BYCOMMAND);
		EnableMenuItem(hMenu, ID_OPTIONS_SHORTCUT, MF_DISABLED);
		EnableMenuItem(hMenu, ID_OPTIONS_REPLACEKEYS, MF_ENABLED);
		CheckMenuItem(hMenu, ID_OPTIONS_REPLACEKEYS, MF_UNCHECKED | MF_BYCOMMAND);
	}
	//Show hide all commmon belong to replace key
	hTmp = GetDlgItem(hDlg, IDC_CMB_SYSKEY3);
	ShowWindow(hTmp, status1);
	hTmp = GetDlgItem(hDlg, IDC_CMB_SYSKEY4);
	ShowWindow(hTmp, status1);
	hTmp = GetDlgItem(hDlg, IDC_NEWKEY);
	ShowWindow(hTmp, status1);


	
	hTmp = GetDlgItem(hDlg, IDC_STATIC1);
	ShowWindow(hTmp, status1);
	hTmp = GetDlgItem(hDlg, IDC_STATIC2);
	ShowWindow(hTmp, status1);
	hTmp = GetDlgItem(hDlg, IDC_STATIC3);
	ShowWindow(hTmp, status1);
	hTmp = GetDlgItem(hDlg, IDC_STATIC4);
	ShowWindow(hTmp, status1);
	hTmp = GetDlgItem(hDlg, IDC_BTN_INSTALL);
	ShowWindow(hTmp, status1);

	//Show common item belong to shortcut
	hTmp = GetDlgItem(hDlg, IDC_INSTALLEXEC);
	ShowWindow(hTmp, status2);
	hTmp = GetDlgItem(hDlg, IDC_OPEN);
	ShowWindow(hTmp, status2);
	hTmp = GetDlgItem(hDlg, IDC_DIR);
	ShowWindow(hTmp, status2);

	DeleteObject(hMenu);
	DeleteObject(hTmp);
}

void saveHook(std::vector<ComboKey> *listCbKey)
{
	HANDLE hFile = 0;
	DWORD  dwBytesWritten;
	if (INVALID_HANDLE_VALUE ==
		(hFile = CreateFile(TEXT("save.hook"), GENERIC_WRITE, 0,
		NULL, CREATE_ALWAYS, 0, NULL)))
	{
		wchar_t buf[256];
		FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buf, 256, NULL);
		MessageBox(NULL, buf, NULL, 0);
		return;
	}
		
	int n = listCbKey->size();
	WriteFile(hFile, &n, sizeof(int), &dwBytesWritten, NULL);
	for (int i = 0; i < n; i++)
	{
		ComboKey tmp = listCbKey->operator[](i);
		WriteFile(hFile, &tmp, sizeof(ComboKey), &dwBytesWritten, NULL);
	}
	CloseHandle(hFile);
}

void loadHook(std::vector<ComboKey> *&listCbKey, HWND hListOld, HWND hListNew)
{
	HANDLE hFile;
	DWORD  dwBytesRead;
	if (INVALID_HANDLE_VALUE ==
		(hFile = CreateFile(TEXT("save.hook"), GENERIC_READ, FILE_SHARE_READ,
		NULL, OPEN_EXISTING, 0, NULL)))
		return;
	int n;
	ReadFile(hFile, &n, sizeof(int), &dwBytesRead, NULL);
	for (int i = 0; i < n; i++)
	{
		ComboKey tmp;
		ReadFile(hFile, &tmp, sizeof(ComboKey), &dwBytesRead, NULL);
		listCbKey->push_back(tmp);
		UpdateListbox(hListOld, hListNew, tmp);
	}
	CloseHandle(hFile);
}

void onOpenDialog(HWND hWnd, TCHAR pstrFileName[], TCHAR pstrTitleName[])
{
	OPENFILENAME	ofn;
	ZeroMemory(&ofn, sizeof(OPENFILENAME));

	ofn.nFilterIndex = 4;
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.hInstance = NULL;
	ofn.lpstrFilter = TEXT(".exe\0*.exe\0");
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 0;
	ofn.lpstrFile = pstrFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFileTitle = pstrTitleName;
	ofn.nMaxFileTitle = MAX_PATH;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt = TEXT("exe");
	ofn.lCustData = 0L;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;

	GetOpenFileName(&ofn);
};


INT_PTR CALLBACK WndProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	static bool isOk = false;
	static HWND hCmbSysKey1 = 0;
	static HWND hCmbSysKey2 = 0;
	static HWND hCmbSysKey3 = 0;
	static HWND hCmbSysKey4 = 0;
	static HWND hEdTOldKey = 0;
	static HWND hEdTNewKey = 0;
	static HWND hListOld = 0;
	static HWND hListNew = 0;
	static HWND hEdtDir = 0;
	static HKeyboard_HL hk(hDlg);
	static std::vector<ComboKey> *listCbKey;
	static TCHAR	pstrFileName[MAX_LOADSTRING], pstrTitleName[MAX_LOADSTRING];
	HMENU hMenu = 0;


	switch (message)
	{
	case WM_INITDIALOG:
		onInitDialog(hDlg, hCmbSysKey1, hCmbSysKey2, hCmbSysKey3, hCmbSysKey4, hEdTOldKey, hEdTNewKey, hListOld, hListNew, hEdtDir);
		getListCbKey(listCbKey);
		hMenu = LoadMenu(hInst, MAKEINTRESOURCE(IDC_G3));
		SetMenu(hDlg, hMenu);
		CheckMenuItem(hMenu, ID_OPTIONS_REPLACEKEYS, MF_CHECKED | MF_BYCOMMAND);
		EnableMenuItem(hMenu, ID_OPTIONS_REPLACEKEYS, MF_DISABLED);
		loadHook(listCbKey, hListOld, hListNew);
		hk.Install(listCbKey);
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		
		//Catch message
		switch (wmId)
		{
		case ID_OPTIONS_SHORTCUT:
			changeUI(hDlg, 2);
			
			break;
		case ID_OPTIONS_REPLACEKEYS:
			changeUI(hDlg, 1);
			break;
		case IDC_BTN_INSTALL:
		{
			//If input is not correct
			if (!isOk)
				return 0;

			HWND hLabel = GetDlgItem(hDlg, IDC_WARNING);
			ComboKey cbKey;

			//Check: is input data completely?
			cbKey.Update(hCmbSysKey1, hCmbSysKey2, hCmbSysKey3, hCmbSysKey4, hEdTOldKey, hEdTNewKey, hEdtDir);

			if ((cbKey.oldKey < 0 && cbKey.oldSystemKey1 < 0 && cbKey.oldSystemKey2 < 0) ||
				(cbKey.newKey < 0 && cbKey.newSystemKey1 < 0 && cbKey.newSystemKey2 < 0))
			{
				SetWindowText(hLabel, L"Some inputs must not be empty");
				DeleteObject(hLabel);
				return 0;
			}
			
			//Add cbKey to share memory region
			addNewCbKey(cbKey);
			
			UpdateListbox(hListOld, hListNew, cbKey);
			ResetInput(hCmbSysKey1, hCmbSysKey2, hCmbSysKey3, hCmbSysKey4, hEdTOldKey, hEdTNewKey, hEdtDir);

			DeleteObject(hLabel);
			break;
		}
		case IDC_BTN_INSTALL_EXEC:
		{
			//If input is not correct
			if (!isOk)
				return 0;

			HWND hLabel = GetDlgItem(hDlg, IDC_WARNING);
			ComboKey cbKey;
			cbKey.Update(hCmbSysKey1, hCmbSysKey2, hCmbSysKey3, hCmbSysKey4, hEdTOldKey, hEdTNewKey, hEdtDir);

			if ((cbKey.oldKey < 0 && cbKey.oldSystemKey1 < 0 && cbKey.oldSystemKey2 < 0) ||
				wcslen(cbKey.dirName) < 1)
			{
				SetWindowText(hLabel, L"Some inputs must not be empty");
				DeleteObject(hLabel);
				return 0;
			}

			//Add cbKey to share memory region
			addNewCbKey(cbKey);

			UpdateListbox(hListOld, hListNew, cbKey);
			ResetInput(hCmbSysKey1, hCmbSysKey2, hCmbSysKey3, hCmbSysKey4, hEdTOldKey, hEdTNewKey, hEdtDir);

			DeleteObject(hLabel);
			break;
		}
		case IDC_BTN_REMOVE:
			onRemoveOneHook(hListOld, hListNew, listCbKey);
			break;
		case IDC_REMOVEALL:
			SendMessage(hListOld, LB_RESETCONTENT, 0, 0);
			SendMessage(hListNew, LB_RESETCONTENT, 0, 0);
			listCbKey->resize(0);
			hk.Remove();
			break;
		case IDC_OPEN:
		{		
			onOpenDialog(hDlg,  pstrFileName, pstrTitleName);

			HWND hEditText = GetDlgItem(hDlg, IDC_DIR);
			SetWindowText(hEditText, pstrFileName);

			DeleteObject(hEditText);
			return 1;
		}
		}

		//Catch event
		switch (wmEvent)
		{
			//Event select change combobox and list
		case CBN_SELCHANGE:
		{
			//Combobox
			HWND hLabel = GetDlgItem(hDlg, IDC_WARNING);
			if (TriggerCombobox(hCmbSysKey1, hCmbSysKey2) == FALSE ||
				TriggerCombobox(hCmbSysKey3, hCmbSysKey4) == FALSE)
			{
				SetWindowText(hLabel, L"Two system key must be different");
				isOk = false;
			}
			else
			{
				SetWindowText(hLabel, L"");
				isOk = true;
			}
			DeleteObject(hLabel);

			//Listbox
			int ind = SendMessage(hListOld, LB_GETCURSEL, 0, 0);
			SendMessage(hListNew, LB_SETCURSEL, ind, 0);
			break;
		}
		//Event select change edittext control
		case EN_CHANGE:
		{
			HWND hLabel = GetDlgItem(hDlg, IDC_WARNING);
			if (TriggerEditText(hEdTOldKey) == FALSE ||
				TriggerEditText(hEdTNewKey) == FALSE)
			{
				SetWindowText(hLabel, L"A normal key must be only one character");
				isOk = false;
			}
			else
			{
				SetWindowText(hLabel, L"");
				isOk = true;
			}
			DeleteObject(hLabel);
			break;
		}
		}

		return DefWindowProc(hDlg, message, wParam, lParam);

	case WM_PAINT:
		hdc = BeginPaint(hDlg, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hDlg, &ps);
		break;
	
	case WM_CLOSE:
	case WM_DESTROY:
		saveHook(listCbKey);
		DeleteObject(hMenu);
		listCbKey = NULL;
		//Remove hook is important
		hk.Remove();
		PostQuitMessage(0);
		break;

	}
	return 0;
}

void getListCbKey(std::vector<ComboKey> *&list)
{

	typedef std::vector<ComboKey>*(*MYPROC)();

	HINSTANCE hinstLib;
	MYPROC ProcAddr;

	hinstLib = LoadLibrary(L"ListComboKeyDLL.dll");
	if (hinstLib != NULL) {
		ProcAddr = (MYPROC)GetProcAddress(hinstLib, "getListComboKey");
		// Nếu lấy được địa chỉ hàm, gọi thực hiện hàm
		if (ProcAddr != NULL)
			list = ProcAddr();
	}
}

void addNewCbKey(ComboKey cbKey)
{

	typedef VOID(*MYPROC)(ComboKey);

	HINSTANCE hinstLib;
	MYPROC ProcAddr;

	hinstLib = LoadLibrary(L"ListComboKeyDLL.dll");
	if (hinstLib != NULL) {
		ProcAddr = (MYPROC)GetProcAddress(hinstLib, "addNewComboKey");
		// Nếu lấy được địa chỉ hàm, gọi thực hiện hàm
		if (ProcAddr != NULL)
			ProcAddr(cbKey);
	}
}