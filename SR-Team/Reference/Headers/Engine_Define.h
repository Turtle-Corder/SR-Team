#pragma once

#ifndef __ENGINE_DEFINE_H__
#define __ENGINE_DEFINE_H__

#pragma  warning(disable : 4251)





//----------------------------------------------------------------------------------------------------
// DLL
//----------------------------------------------------------------------------------------------------

#ifdef __ENGINE_EXPORTS__
#define ENGINE_DLL _declspec(dllexport)
#else
#define ENGINE_DLL _declspec(dllimport)
#endif // __ENGINE_EXPORTS__


#define MIN_STR 64
#define MID_STR 128
#define MAX_STR 256





//----------------------------------------------------------------------------------------------------
// LOG
//----------------------------------------------------------------------------------------------------

//--------------------------------------------------
// �α� ����
//--------------------------------------------------
namespace LOG
{	
	enum LEVEL 
	{ 
		SYSTEM, 
		ENGINE, 
		CLIENT, 
		DEBUG, 
	};	
}


//--------------------------------------------------
// �α� �޽���
//--------------------------------------------------
#ifdef _DEBUG
#define PRINT_LOG(logMsg, level)								\
{																\
	switch(level)												\
	{															\
		case LOG::SYSTEM:										\
			::MessageBox(0, logMsg, L"System Error", MB_OK);	\
			break;												\
																\
		case LOG::ENGINE:										\
			::MessageBox(0, logMsg, L"Engine Error", MB_OK);	\
			break;												\
																\
		case LOG::CLIENT:										\
			::MessageBox(0, logMsg, L"Client Error", MB_OK);	\
			break;												\
																\
		case LOG::DEBUG:										\
			::MessageBox(0, logMsg, L"Debug Log", MB_OK);		\
			break;												\
	}															\
}
#else
#define PRINT_LOG(log, level)									\
{																\
	switch(level)												\
	{															\
		case LOG::SYSTEM:										\
			::MessageBox(0, logMsg, L"System Error", MB_OK);	\
			break;												\
																\
		case LOG::ENGINE:										\
			::MessageBox(0, logMsg, L"Engine Error", MB_OK);	\
			break;												\
																\
		case LOG::CLIENT:										\
			::MessageBox(0, logMsg, L"Client Error", MB_OK);	\
			break;												\
	}															\
}
#endif





//----------------------------------------------------------------------------------------------------
// Namespace
//----------------------------------------------------------------------------------------------------
#define BEGIN(Name) namespace Name	{
#define END							}
#define USING(Name)	using namespace Name;





//----------------------------------------------------------------------------------------------------
// Instance
//----------------------------------------------------------------------------------------------------

//--------------------------------------------------
// ���� ������, ���Կ����� ����
//--------------------------------------------------
#define NO_COPY(ClassName)								\
private:												\
	ClassName(const ClassName&) = delete;				\
	ClassName& operator=(const ClassName&) = delete;


//--------------------------------------------------
// �̱��� ���� ��ũ��
//--------------------------------------------------
#define DECLARE_SINGLETON(ClassName)					\
		NO_COPY(ClassName)								\
public:													\
	static	ClassName* Get_Instance();					\
	_uint	Destroy_Instance();							\
														\
private:												\
	static	ClassName*	m_pInstance;


//--------------------------------------------------
// �̱��� ���� ��ũ��
//--------------------------------------------------
#define IMPLEMENT_SINGLETON(ClassName)					\
ClassName* ClassName::m_pInstance = nullptr;			\
ClassName* ClassName::Get_Instance()					\
{														\
	if(nullptr == m_pInstance)							\
		m_pInstance = new ClassName;					\
														\
	return m_pInstance;									\
}														\
														\
_uint ClassName::Destroy_Instance()						\
{														\
	_uint iRefCnt = 0;									\
	if(m_pInstance)										\
		iRefCnt = m_pInstance->Release();				\
														\
	if(0 == iRefCnt)									\
		m_pInstance = nullptr;							\
														\
	return iRefCnt;										\
}





//----------------------------------------------------------------------------------------------------
// Object State
//----------------------------------------------------------------------------------------------------
namespace GAMEOBJECT
{
	enum eState
	{
		NOEVENT,
		DEAD,
		WARN,
		ERR
	};
}





#endif