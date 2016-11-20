#include "stdafx.h"
#include "ComboKey.h"

#define NUM_0 0x30
#define NUM_9 0x39
#define MAX_LEN_DIR 200

int convertVirtualSystemKey(TCHAR *t)
{
	if (wcscmp((wchar_t*)t, SYSKEY_CTRL) == 0)
		return VK_CONTROL;
	if (wcscmp((wchar_t*)t, SYSKEY_ALT) == 0)
		return VK_MENU;
	if (wcscmp((wchar_t*)t, SYSKEY_SHIFT) == 0)
		return VK_SHIFT;
	return -1;
}

int convertVirtualNormalKey(TCHAR t)
{
	
	if (VkKeyScan(t) == 818)
		return -1;
	return VkKeyScan(t);
}
void ComboKey::Update(HWND hCmbSysKey1, HWND hCmbSysKey2, HWND hCmbSysKey3, HWND hCmbSysKey4, HWND hEdTOldKey, HWND hEdTNewKey, HWND hEdtDir)
{
	GetWindowText(hCmbSysKey1, sOldSystemKey1, MAX_LEN_SYSKEY);
	GetWindowText(hCmbSysKey2, sOldSystemKey2, MAX_LEN_SYSKEY);
	GetWindowText(hCmbSysKey3, sNewSystemKey1, MAX_LEN_SYSKEY);
	GetWindowText(hCmbSysKey4, sNewSystemKey2, MAX_LEN_SYSKEY);
	GetWindowText(hEdTOldKey, sOldKey, MAX_LEN_SYSKEY);
	GetWindowText(hEdTNewKey, sNewKey, MAX_LEN_SYSKEY);
	GetWindowText(hEdtDir, this->dirName, MAX_LEN_DIR);

	this->oldSystemKey1 = convertVirtualSystemKey(sOldSystemKey1);
	this->oldSystemKey2 = convertVirtualSystemKey(sOldSystemKey2);
	this->newSystemKey1 = convertVirtualSystemKey(sNewSystemKey1);
	this->newSystemKey2 = convertVirtualSystemKey(sNewSystemKey2);
	this->oldKey = convertVirtualNormalKey(sOldKey[0]);
	this->newKey = convertVirtualNormalKey(sNewKey[0]);

	
}