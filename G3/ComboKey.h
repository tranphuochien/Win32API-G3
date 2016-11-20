#ifndef _COMBOKEY_
#define _COMBOKEY_

#include "resource.h"

#define MAX_LEN_KEY 6
#define MAX_LEN_DIR 200
int convertVirtualSystemKey(TCHAR*);
int convertVirtualNormalKey(TCHAR);
struct ComboKey
{
	int oldSystemKey1;
	TCHAR sOldSystemKey1[MAX_LEN_KEY];
	int oldSystemKey2;
	TCHAR sOldSystemKey2[MAX_LEN_KEY];
	int oldKey;
	TCHAR sOldKey[2];

	int newSystemKey1;
	TCHAR sNewSystemKey1[MAX_LEN_KEY];
	int newSystemKey2;
	TCHAR sNewSystemKey2[MAX_LEN_KEY];
	int newKey;
	TCHAR sNewKey[2];

	TCHAR dirName[MAX_LEN_DIR];

	void Update(HWND, HWND, HWND, HWND, HWND, HWND, HWND);

};
#endif