#pragma once
#ifndef __KEYMGR_H__
#define __KEYMGR_H__

BEGIN(Client)

#define VK_MAX 0xff

class CKeyManager
{
private:
	CKeyManager();
	virtual ~CKeyManager() = default;

public:
	bool Key_Pressing(_int _key);
	bool Key_Down(_int _key);
	bool Key_Up(_int _key);
	void Key_Update();

public:
	static CKeyManager* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CKeyManager;
		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}



private:
	static CKeyManager*	m_pInstance;
	bool				bKeyState[VK_MAX];
};

END

#endif // !__KEYMGR_H__
