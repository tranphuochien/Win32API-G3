// ListComboKeyDLL.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <vector>

HINSTANCE hInstLib;
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

//Global variable
#pragma data_seg ("SHARED")
std::vector<ComboKey> listComboKey;
#pragma data_seg()
#pragma comment(linker, "/section:SHARED,RWS") 

EXPORT void addNewComboKey(ComboKey cbKey)
{
	listComboKey.push_back(cbKey);
}

EXPORT std::vector<ComboKey>* getListComboKey()
{	
	return &listComboKey;
}

