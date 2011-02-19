/*
.____     ____    ____                         __       ___                          ___         __     
/\  _`\  /\  _`\ /\  _`\                      /\ \     /\_ \                       /'___`\     /'__`\   
\ \ \/\_\\ \ \L\_\ \ \L\ \     __      ___ ___\ \ \____\//\ \      __   _ __      /\_\ /\ \   /\ \/\ \  
.\ \ \/_/_\ \ \L_L\ \ ,  /   /'__`\  /' __` __`\ \ '__`\ \ \ \   /'__`\/\`'__\    \/_/// /__  \ \ \ \ \ 
..\ \ \L\ \\ \ \/, \ \ \\ \ /\ \L\.\_/\ \/\ \/\ \ \ \L\ \ \_\ \_/\  __/\ \ \/        // /_\ \__\ \ \_\ \
...\ \____/ \ \____/\ \_\ \_\ \__/.\_\ \_\ \_\ \_\ \_,__/ /\____\ \____\\ \_\       /\______/\_\\ \____/
....\/___/   \/___/  \/_/\/ /\/__/\/_/\/_/\/_/\/_/\/___/  \/____/\/____/ \/_/       \/_____/\/_/ \/___/ 

=���ڹٷ���վ��=
http://code.google.com/p/cgrambler/ ��CGRambler2.0�Ĺٷ���վ��������������ص�����Ŀ������Դ�룬�����ͼ����ͳһ�ڹٷ���վ�������ۣ��Ա㱾�˻ظ���

=��Ŀ��飺=
CGRambler2.0�Ǽ�CGRambler1.0֮����2011��1��18�ſ�ʼ������һ�����DirectX 10��ͼ����Ⱦ���棬����CGRambler1.0�������http://user.qzone.qq.com/499482794/blog/1285834895
���CGRambler1.0��CGRambler2.0�������¼ܹ�����������д����������ע�����湹�ܱ�����������������Ⱦ�������������ǻ�����Shader�����⣬����Ŀ���ÿ�Դ��ʽ��������������ҵ�����ҵ��;��

=�������ߣ�=
����ʦ����ѧ 08�� �ȫ cgrambler@gmail.com

******************************************************************************************************************************************************************************************************************************************************************************************************

=Brief Introduction to CGRambler2.0:=
CGRambler2.0 is a DirectX 10 based rendering engine under developing since 2011/1/18, for the previous version of CGRambler1.0,please see http://user.qzone.qq.com/499482794/blog/1285834895
Compare with CGrambler1.0, CGRambler2.0 have been designed from the very begining. It will focus on the architecture of the engine itself, not the gorgeous shaders. In addition, this project is completely open source, you can use it for commercial or non-commercial without permission.

=About the official page:=
http://code.google.com/p/cgrambler/ is the official page of CGRambler2.0, you can download the newest source code and screenshots of this project from that site.Please comment this project at the official page, so that I can reply.

=About the author:=
South China Normal University, Grade 2008, HaiQuan Li, cgrambler@gmail.com
*/
#pragma once
#include "DXUT.h"
#include "DXUTcamera.h"
#include "DXUTsettingsdlg.h"
#include "SDKmisc.h"
#include "SDKmesh.h"
#include "resource.h"
#include "SmartPtr.h"
#include "WString.h"
#include "Common.h"
#include <map>
using namespace std;

#define DEPTH_TEXTURE_FORMAT  DXGI_FORMAT_R24G8_TYPELESS


class Texture
{
	bool mSRVCreated;
	bool mRTVCreated;
	bool mDSVCreated;
	bool mIsManual;
	HRESULT hr;
	WString mType;
	D3D10_TEXTURE2D_DESC mDesc;
	WString mFileName;
	ID3D10DevicePtr mDevice;
	ID3D10Texture2DPtr mTexture;
	ID3D10ShaderResourceViewPtr mShaderResourceView;
	ID3D10RenderTargetViewPtr mRenderTargetView;
	::ID3D10DepthStencilViewPtr mDepthStencilView;
	::DXGI_FORMAT mFormat;
	::FLOAT mWidthRatio;
	::FLOAT mHeightRatio;

	D3D10_VIEWPORT  mViewport;
private:
	void createManualTexture();
	void createShaderResourceView();
	void createRenderTargetView();
	void createDepthStencilView();

public:
	Texture(ID3D10DevicePtr device,const WString &fileName,const WString &type);
	Texture(ID3D10DevicePtr device,const WString &fileName,FLOAT widthRatio,FLOAT heightRatio,DXGI_FORMAT format);


	virtual ~Texture();
	void onD3D10SwapChainResized(::IDXGISwapChainPtr pSwapChain,const ::DXGI_SURFACE_DESCPtr pBackSurfaceDesc);
	ID3D10ShaderResourceViewPtr getShaderResourceView();
	ID3D10RenderTargetViewPtr getRenderTargetView();
	::ID3D10DepthStencilViewPtr getDepthStencilView();
	void init();
	UINT getWidth();
	UINT getHeight();
	const ::D3D10_VIEWPORT &getViewport();

};


#ifdef USE_SMART_POINTER
typedef SmartPtr<Texture> TexturePtr;
#else
typedef Texture* TexturePtr;
#endif


typedef map<WString,TexturePtr>::iterator TextureItr;