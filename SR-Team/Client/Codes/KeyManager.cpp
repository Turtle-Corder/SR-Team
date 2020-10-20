#include "stdafx.h"
#include "..\Headers\KeyManager.h"

USING(Client)

CKeyManager* CKeyManager::m_pInstance = nullptr;
CKeyManager::CKeyManager()
{
	ZeroMemory(bKeyState, sizeof(bKeyState));
}

bool CKeyManager::Key_Pressing(_int _key)
{
	if (GetAsyncKeyState(_key) & 0x8000)
		return true;
	return false;
}

bool CKeyManager::Key_Down(_int _key)
{
	if (!bKeyState[_key] && (GetAsyncKeyState(_key) & 0x8000))
	{
		bKeyState[_key] = !bKeyState[_key];
		return true;
	}
	return false;
}

bool CKeyManager::Key_Up(_int _key)
{
	if (bKeyState[_key] && !(GetAsyncKeyState(_key) & 0x8000))
	{
		bKeyState[_key] = !bKeyState[_key];
		return true;
	}
	return false;
}

void CKeyManager::Key_Update()
{
	for (int i = 0; i < VK_MAX; ++i)
	{
		if (bKeyState[i] && !(GetAsyncKeyState(i) & 0x8000))
			bKeyState[i] = !bKeyState[i];
		if (!bKeyState[i] && (GetAsyncKeyState(i) & 0x8000))
			bKeyState[i] = !bKeyState[i];
	}
}
