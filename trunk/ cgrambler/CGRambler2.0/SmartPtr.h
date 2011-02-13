/*
=���ڹٷ���վ��=
http://code.google.com/p/cgrambler/ ��CGRambler2.0�Ĺٷ���վ��������������ص�����Ŀ������Դ�룬�����ͼ����ͳһ�ڹٷ���վ�������ۣ��Ա㱾�˻ظ���
=About the official page:=
http://code.google.com/p/cgrambler/ is the official page of CGRambler2.0, you can download the newest source code and screenshots of this project from that site.Please comment this project at the official page, so that I can reply.

=��Ŀ��飺=
CGRambler2.0�Ǽ�CGRambler1.0֮�󿪷���һ�����DirectX 10��ͼ����Ⱦ���棬����CGRambler1.0�������http://user.qzone.qq.com/499482794/blog/1285834895
���CGRambler1.0��CGRambler2.0�������¼ܹ�����������д����������ע�����湹�ܱ�����������������Ⱦ�������������ǻ�����Shader�����⣬����Ŀ���ÿ�Դ��ʽ��������������ҵ�����ҵ��;��
=Brief Introduction to CGRambler2.0:=
CGRambler2.0 is a DirectX 10 based rendering engine, for the previous version of CGRambler1.0,please see http://user.qzone.qq.com/499482794/blog/1285834895
Compare with CGrambler1.0, CGRambler2.0 have been designed from the very begining. It will focus on the architecture of the engine itself, not the gorgeous shaders. In addition, this project is completely open source, you can use it for commercial or non-commercial without permission.

=�������ߣ�=
����ʦ����ѧ 08�� �ȫ cgrambler@gmail.com
=About the author:=
South China Normal University, Grade 2008, HaiQuan Li, cgrambler@gmail.com
*/
#pragma once
#include <iostream>

#include "DXUT.h"
#include "DXUTgui.h"
#include "DXUTsettingsdlg.h"
#include "DXUTcamera.h"
#include "SDKmesh.h"
#include "SDKmisc.h"

//NB: this smart pointer has lots of problems, I disabled it currently;
template <typename T>
class SmartPtr
{
public:
	SmartPtr(T *p = 0);
	SmartPtr(const SmartPtr& src);

	inline SmartPtr& operator= (const SmartPtr& rhs);
	inline T *operator->();
	inline T &operator*();
	inline T **operator&();
	inline operator T*();
	inline bool operator==(SmartPtr<T> tar);


	virtual ~SmartPtr();
private:
	inline void decrUse();
	T *ptr;
	size_t *pUse;
};




template <typename T>
T **SmartPtr<T>::operator&()
{
	return &ptr;
}

template <typename T>
SmartPtr<T>::SmartPtr(const SmartPtr& src): ptr(src.ptr), pUse(src.pUse) 
{
	++*pUse;
}

template <typename T>
SmartPtr<T>::SmartPtr(T *p=0):ptr(p), pUse(new size_t(1)) { }





template <typename T>
SmartPtr<T>& SmartPtr<T>::operator= (const SmartPtr<T>& rhs)
{
	++*rhs.pUse;
	decrUse();
	ptr = rhs.ptr;
	pUse = rhs.pUse;
	return *this;
}

template <typename T>
T *SmartPtr<T>::operator->()
{
	return ptr;
}

template <typename T>
T &SmartPtr<T>::operator*() 
{
	return *ptr;
}


template <typename T>
SmartPtr<T>::operator T*() 
{
	return ptr;
}



template <typename T>
SmartPtr<T>::~SmartPtr()
{
	decrUse();
}


template <typename T>
void SmartPtr<T>::decrUse()
{
	if (--*pUse == 0) 
	{
		SAFE_RELEASE(ptr);
		delete pUse;
	}
}



template <typename T>
bool SmartPtr<T>::operator==(SmartPtr<T> tar)
{
	return ptr==tar;
}


#ifdef  USE_SMART_POINTER 

typedef SmartPtr<ID3D10Texture2D> ID3D10Texture2DPtr;
typedef SmartPtr<ID3D10Buffer> ID3D10BufferPtr;
typedef SmartPtr<ID3D10RenderTargetView> ID3D10RenderTargetViewPtr;
typedef SmartPtr<ID3D10DepthStencilView> ID3D10DepthStencilViewPtr;
typedef SmartPtr<ID3D10Effect> ID3D10EffectPtr;
typedef SmartPtr<ID3D10InputLayout> ID3D10InputLayoutPtr;
typedef SmartPtr<ID3D10Device> ID3D10DevicePtr;
typedef SmartPtr<ID3D10ShaderResourceView> ID3D10ShaderResourceViewPtr;
typedef SmartPTr<IDXGISwapChain> IDXGISwapChainPtr;


//������Щ��û��Release���� 

typedef ID3D10EffectVariable*  ID3D10EffectVariablePtr;
typedef ID3D10EffectShaderResourceVariable* ID3D10EffectShaderResourceVariablePtr;
typedef ID3D10EffectTechnique* ID3D10EffectTechniquePtr;
typedef CDXUTSDKMesh* CDXUTSDKMeshPtr;
typedef char* CharPtr;
typedef ID3D10EffectPass* ID3D10EffectPassPtr;
typedef ID3D10EffectType* ID3D10EffectTypePtr;
typedef float* FLOATPtr;
typedef UINT* UINTPtr;
typedef INT* INTPtr;

typedef const DXGI_SURFACE_DESC* DXGI_SURFACE_DESCPtr;
typedef SDKMESH_SUBSET* SDKMESH_SUBSETPtr;
typedef ID3D10EffectMatrixVariable* ID3D10EffectMatrixVariablePtr;

typedef ID3DX10Font* ID3DX10FontPtr;
typedef ID3DX10Sprite* ID3DX10SpritePtr;
typedef CDXUTTextHelper*  CDXUTTextHelperPtr;


#else


typedef  ID3D10Texture2D* ID3D10Texture2DPtr;
typedef  ID3D10Buffer* ID3D10BufferPtr;
typedef  ID3D10RenderTargetView* ID3D10RenderTargetViewPtr;
typedef  ID3D10DepthStencilView* ID3D10DepthStencilViewPtr;
typedef  ID3D10Effect* ID3D10EffectPtr;
typedef  ID3D10InputLayout* ID3D10InputLayoutPtr;
typedef ID3D10Device* ID3D10DevicePtr;
typedef  ID3D10ShaderResourceView* ID3D10ShaderResourceViewPtr;
typedef  IDXGISwapChain* IDXGISwapChainPtr;


typedef ID3D10EffectVariable*  ID3D10EffectVariablePtr;
typedef ID3D10EffectShaderResourceVariable* ID3D10EffectShaderResourceVariablePtr;
typedef ID3D10EffectTechnique* ID3D10EffectTechniquePtr;
typedef CDXUTSDKMesh* CDXUTSDKMeshPtr;
typedef char* CharPtr;
typedef ID3D10EffectPass* ID3D10EffectPassPtr;
typedef ID3D10EffectType* ID3D10EffectTypePtr;
typedef float* FLOATPtr;
typedef UINT* UINTPtr;
typedef INT* INTPtr;

typedef const DXGI_SURFACE_DESC* DXGI_SURFACE_DESCPtr;
typedef SDKMESH_SUBSET* SDKMESH_SUBSETPtr;
typedef ID3D10EffectMatrixVariable* ID3D10EffectMatrixVariablePtr;

typedef ID3DX10Font* ID3DX10FontPtr;
typedef ID3DX10Sprite* ID3DX10SpritePtr;
typedef CDXUTTextHelper*  CDXUTTextHelperPtr;

#endif


