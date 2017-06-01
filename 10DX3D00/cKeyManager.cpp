#include "stdafx.h"
#include "cKeyManager.h"


cKeyManager::cKeyManager()
{
	for (int i = 0; i < KEYMAX; ++i)
	{
		this->setKeyDown(i, false);
		this->setKeyUp(i, false);
	}
}


cKeyManager::~cKeyManager()
{
}

//0x0000 : ������ ���� ���� ���� ȣ�� ������ �� ���� ����
//0x8000 : ������ ���� ���� ���� ȣ�� ������ ���� ����
//0x8001 : ������ ���� ���� �ְ� ȣ�� ������ ���� ����
//0x0001 : ������ ���� ���� �ְ� ȣ�� ������ �� ���� ����

//Ű�� �ѹ��� ���ȴ��� �˻�
bool cKeyManager::isOnceKeyDown(int key)
{
	if (GetAsyncKeyState(key) & 0x8000)
	{
		//key��° �迭�� false���¸� == ������ ���� ������
		if (!this->getKeyDown()[key])
		{
			//�����ش�!~!!
			this->setKeyDown(key, true);
			return true;
		}
	}
	else this->setKeyDown(key, false);
	return false;
}

//Ű�� �����ٰ� ������ �� �˻�
bool cKeyManager::isOnceKeyUp(int key)
{
	if (GetAsyncKeyState(key) & 0x8000)
	{
		this->setKeyUp(key, true);
	}
	else
	{
		if (this->getKeyUp()[key])
		{
			this->setKeyUp(key, false);
			return true;
		}
	}

	return false;
}

//Ű�� ������ ���� �� ��� �˻�
bool cKeyManager::isStayKeyDown(int key)
{
	if (GetAsyncKeyState(key) & 0x8000) return true;
	return false;
}

//�����ٰ� ���� true, �ٽ� �����ٰ� ���� false... ���ѹݺ�
bool cKeyManager::isToggleKey(int key)
{
	if (GetKeyState(key) & 0x0001) return true;
	return false;
}
