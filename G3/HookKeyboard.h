#ifndef _HOOKKEYBOARD_
#define _HOOKKEYBOARD_

#include "ComboKey.h"

//Abstract class HookKeyboard
class IHKeyboard
{
protected:
	HWND _hWnd;
	int _typeHook;
public:
	virtual BOOL Install(std::vector<ComboKey> *) = 0;
	virtual void Remove() = 0;

};


//Class HookKeyBoard High Level
class HKeyboard_HL : public IHKeyboard
{
private:
	bool isFirst;
public:
	HKeyboard_HL(HWND);
	BOOL Install(std::vector<ComboKey> *);
	void Remove();

	bool isFirstInstall();
	~HKeyboard_HL();
};
#endif