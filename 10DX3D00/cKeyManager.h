#pragma once
#include <bitset> //bit단위로 저장하고 조작 할 수 있는 class

#define g_pKeyManager cKeyManager::GetInstance()
#define KEYMAX 256

class cKeyManager
{
private:
	SINGLETONE(cKeyManager);
	bitset<KEYMAX> _keyUp;
	bitset<KEYMAX> _keyDown;

public:
	bool isOnceKeyDown(int key);
	bool isOnceKeyUp(int key);
	bool isStayKeyDown(int key);
	bool isToggleKey(int key);

	bitset<KEYMAX> getKeyUp() { return _keyUp; }
	bitset<KEYMAX> getKeyDown() { return _keyDown; }
	void setKeyDown(int key, bool state) { _keyDown.set(key, state); }
	void setKeyUp(int key, bool state) { _keyUp.set(key, state); }
};

