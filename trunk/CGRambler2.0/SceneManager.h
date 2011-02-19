/*
.____     ____    ____                         __       ___                          ___         __     
/\  _`\  /\  _`\ /\  _`\                      /\ \     /\_ \                       /'___`\     /'__`\   
\ \ \/\_\\ \ \L\_\ \ \L\ \     __      ___ ___\ \ \____\//\ \      __   _ __      /\_\ /\ \   /\ \/\ \  
.\ \ \/_/_\ \ \L_L\ \ ,  /   /'__`\  /' __` __`\ \ '__`\ \ \ \   /'__`\/\`'__\    \/_/// /__  \ \ \ \ \ 
..\ \ \L\ \\ \ \/, \ \ \\ \ /\ \L\.\_/\ \/\ \/\ \ \ \L\ \ \_\ \_/\  __/\ \ \/        // /_\ \__\ \ \_\ \
...\ \____/ \ \____/\ \_\ \_\ \__/.\_\ \_\ \_\ \_\ \_,__/ /\____\ \____\\ \_\       /\______/\_\\ \____/
....\/___/   \/___/  \/_/\/ /\/__/\/_/\/_/\/_/\/_/\/___/  \/____/\/____/ \/_/       \/_____/\/_/ \/___/ 


=关于官方网站：=
http://code.google.com/p/cgrambler/ 是CGRambler2.0的官方网站，你可以在那下载到本项目的最新源码，高清截图。请统一在官方网站发表评论，以便本人回复。

=项目简介：=
CGRambler2.0是继CGRambler1.0之后，于2011年1月18号开始开发的一款基于DirectX 10的图形渲染引擎，关于CGRambler1.0，请浏览http://user.qzone.qq.com/499482794/blog/1285834895
相比CGRambler1.0，CGRambler2.0经过重新架构（几乎是重写），将更加注重引擎构架本身，即“看不见的渲染艺术”，而不是华丽的Shader。另外，本项目采用开源方式，可自由用于商业或非商业用途。

=关于作者：=
华南师范大学 08级 李海全 cgrambler@gmail.com

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



#include "CGRambler.h" 
#include <map>
#include "WString.h"
#include "Effect.h"
#include "SceneManager.h"
#include "Texture.h"
#include "Light.h"
#include "Camera.h"
#include "DXUT.h"
#include "SDKmisc.h"



class SceneManager;


#ifdef USE_SMART_POINTER
typedef SmartPtr<SceneManager> SceneManagerPtr;
#else
typedef SceneManager* SceneManagerPtr;
#endif

using namespace std;

struct QuadVertex
{
	D3DXVECTOR3 mPos;
	D3DXVECTOR2 mUV;
};


typedef map<WString,::LODMeshPtr>::iterator LODMeshItr;
typedef map<WString,MeshInstancePtr>::iterator MeshInstanceItr;

class SceneManager 
{
	map<WString, LODMeshPtr> mLODMeshs;
	std::map<WString,MeshInstancePtr> mMeshInstances;
	HRESULT hr;
	static SceneManagerPtr mSingleton;
	std::map<WString,EffectPtr> mEffects;

	ID3D10DevicePtr mDevice;
	CameraPtr mCamera;
	float mElapsedTime;
	double mTime;
	map<WString,TexturePtr> mTextures;
	D3DXVECTOR3 mMousePos;
	D3DXVECTOR3 mMouseBut;
	map<WString,LightPtr>  mLights;
	WString mMediaPath;
	D3DXVECTOR4 mAmbient;

	WString mMidRTV[2];
	WString mMidDSV[2];
	UINT mCurMid;

	WString mCurRTV;
	WString mCurDSV;


	D3DXVECTOR4 mClearColor;
	FLOAT mClearDepth;

	ID3D10BufferPtr mQuadBuf;
	ID3D10EffectPassPtr mCurPass;
	WString mCurPassName;

private:
	SceneManager();
	void handleRenderScripts(bool techScripts,EffectPtr effect,MeshInstancePtr mesh,const WString &scriptStr,const WString &scriptClass,const WString &scriptOrder);
	void renderMesh(EffectPtr effect,MeshInstancePtr mesh);
	void updataMatrixs(::ID3D10EffectMatrixVariablePtr var,const WString& semantic,  D3DXMATRIX  world,   D3DXMATRIX  view,  D3DXMATRIX  proj);
	void createQuadBuf();

	TexturePtr getTexture(const WString &name);
	void clearShaderResources();
	void clearRenderTargets();
	void paramsUpdateEveryMesh(map<WString,ParameterPtr> &params,MeshInstancePtr mesh);
	void paramsUpdateEveryFrame(map<WString,ParameterPtr> &params);

	void setRenderTargets(const WString &rtv,const WString &dsv);
	void presentToScreen(const WString &target);
	void drawQuad(ID3D10EffectPassPtr pass);
public:
	void init(ID3D10DevicePtr device);
	virtual void Release();
	void onFrameMove(double fTime,float fElapsedTime);
	void onFrameRender(double fTime,float fElapsedTime);
	void onD3D10SwapChainResized(IDXGISwapChainPtr pSwapChain,const DXGI_SURFACE_DESCPtr pBackBufferSurfaceDesc);
	//在这里完成创建时的参数读取与绑定
	EffectPtr createEffect(const WString &name,const WString &fileName);
	EffectPtr getEffect(const WString &name);
	//fileName是否在mMeshs的mFileName成员中已出现
	MeshInstancePtr createMesh(const WString &name,const WString &meshFileName,const WString &skeFileName,  D3DXVECTOR3 &pos=D3DXVECTOR3(0.0f,0.0f,0.0f),  D3DXVECTOR3 &dir=D3DXVECTOR3(0.0f,0.0f,1.0f), D3DXVECTOR3 &size=D3DXVECTOR3(1.0f,1.0f,1.0f));
	MeshInstancePtr getMesh(const WString &name);
	CameraPtr getCamera();
	void setAmbientColor(const D3DXVECTOR4 &color);
	//检查texBuf的mFileName是否在mTexture的成员中已出现
	TexturePtr createFileTexture(const WString &fileName,const WString &type);
	TexturePtr createManualTexture(const WString &fileName,FLOAT widthRatio,FLOAT heightRatio,DXGI_FORMAT format);
	void msgProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
	LightPtr createLight(const WString &name,   D3DXVECTOR3  &pos,   D3DXVECTOR3 &lookAt,const D3DXVECTOR4 &specular,FLOAT falloffAngle,FLOAT falloffExponent);
	void deleteLight(const WString &name);
	LightPtr getLight(const WString &name);
	void setMediaPath(const WString &path);
	static SceneManagerPtr getSingleton();
	FLOAT getTime();
	FLOAT getElapsedTime();
	const D3DXVECTOR3 &getMousePos();
	const D3DXVECTOR3 &getMouseBut();




	const D3DXVECTOR4 &getAmbient();



	void OnD3D10SwapChainReleasing();




	static bool mInited;
};


