#pragma once

extern		HWND		g_hWnd;

#define  INFINITY 		FLT_MAX;
#define  EPSILON  		0.001f;


template<typename T>
void	Safe_Delete(T& Temp)
{
	if (Temp)
	{
		delete Temp;
		Temp = nullptr;
	}
}

typedef struct Vertex
{
	Vertex(float x, float y, float z, float nx, float ny, float nz, float u, float v)
	{
		vPos.x = x; vPos.y = y; vPos.z = z;
		vNormal.x = nx; vNormal.y = ny; vNormal.z = nz;
		_u = u; _v = v;
	}
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vNormal;
	float	_u, _v;

}Vertex;

#define FVF_NORMAL (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)

#define ERR_MSG(message)								\
::MessageBox(nullptr, message, L"error", MB_OK)

#define NO_COPY(ClassName)								\
ClassName(const ClassName&) = delete;					\
ClassName& operator=(const ClassName&) = delete;

#define DECLARE_SINGLETON(ClassName)					\
		NO_COPY(ClassName)								\
public :												\
	static ClassName* Get_Instance();					\
	static void		  Destroy_Instance();				\
private:												\
	static ClassName* m_pInstance;

#define IMPLEMENT_SINGLETON(ClassName)					\
ClassName* ClassName::m_pInstance = nullptr;			\
ClassName* ClassName::Get_Instance()					\
{														\
	if (nullptr == m_pInstance)							\
		m_pInstance = new ClassName;					\
	return m_pInstance;									\
}														\
void ClassName::Destroy_Instance()						\
{														\
	if(nullptr != m_pInstance)							\
	{													\
		delete m_pInstance;								\
		m_pInstance = nullptr;							\
	}													\
}