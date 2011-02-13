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
	D3DXVECTOR3 pos;
	D3DXVECTOR2 uv;
};

typedef map<WString,MeshPtr>::iterator MeshItr;

class SceneManager
{

	HRESULT hr;
	static SceneManagerPtr mSingleton;
	std::map<WString,EffectPtr> mEffects;
	std::map<WString,MeshPtr> mMeshs;
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

	ID3DX10FontPtr mFont;
	ID3DX10SpritePtr mSprite;
	CDXUTTextHelperPtr mTextHelper;

	WString mTextBuf[MAX_PATH];
	UINT mTextNum;




private:
	SceneManager();

	void markRenderEventBegin(const WString &effectName,const WString &passName,const WString &events);
	void markRenderEventEnd();
	void handleRenderScripts(bool techScripts,EffectPtr effect,MeshPtr mesh,const WString &scriptStr,const WString &scriptClass,const WString &scriptOrder);
	void renderMesh(EffectPtr effect,MeshPtr mesh);
	void  updataMatrixs(::ID3D10EffectMatrixVariablePtr var,const WString& semantic,  D3DXMATRIX  world,   D3DXMATRIX  view,  D3DXMATRIX  proj);
	void createQuadBuf();

	TexturePtr getTexture(const WString &name);
	void clearShaderResources();
	void clearRenderTargets();
	void paramsUpdateEveryMesh(map<WString,ParameterPtr> &params,MeshPtr mesh);
	void paramsUpdateEveryFrame(map<WString,ParameterPtr> &params);
	void drawText(const D3DXVECTOR4 &color=D3DXVECTOR4(1.0f,1.0f,0.0f,1.0f));
	void setRenderTargets(const WString &rtv,const WString &dsv);
	void presentToScreen(const WString &target);
public:
	void init(ID3D10DevicePtr device);
	virtual void Release();
	void onFrameMove(double fTime,float fElapsedTime);
	void onFrameRender(double fTime,float fElapsedTime);
	void onD3D10SwapChainResized(IDXGISwapChainPtr pSwapChain,const DXGI_SURFACE_DESCPtr pBackBufferSurfaceDesc);
	//��������ɴ���ʱ�Ĳ�����ȡ���
	EffectPtr createEffect(const WString &name,const WString &fileName);
	EffectPtr getEffect(const WString &name);
	//fileName�Ƿ���mMeshs��mFileName��Ա���ѳ���
	MeshPtr createMesh(const WString &name,const WString &meshFileName,const WString &skeFileName,const D3DXVECTOR4 &pos=D3DXVECTOR4(0.0f,0.0f,0.0f,0.0f),const D3DXVECTOR4 &dir=D3DXVECTOR4(0.0f,0.0f,1.0f,0.0f),const D3DXVECTOR4 &size=D3DXVECTOR4(1.0f,1.0f,1.0f,0.0f));
	MeshPtr getMesh(const WString &name);
	CameraPtr getCamera();
	void setAmbientColor(const D3DXVECTOR4 &color);
	//���texBuf��mFileName�Ƿ���mTexture�ĳ�Ա���ѳ���
	TexturePtr createFileTexture(const WString &fileName,const WString &type);
	TexturePtr createManualTexture(const WString &fileName,FLOAT widthRatio,FLOAT heightRatio,DXGI_FORMAT format);
	void msgProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
	LightPtr createLight(const WString &name, const D3DXVECTOR4  &pos, const D3DXVECTOR4 &lookAt,const D3DXVECTOR4 &specular,FLOAT falloffAngle,FLOAT falloffExponent);
	void deleteLight(const WString &name);
	LightPtr getLight(const WString &name);
	void setMediaPath(const WString &path);
	static SceneManagerPtr getSingleton();
	FLOAT getTime();
	FLOAT getElapsedTime();
	const D3DXVECTOR3 &getMousePos();
	const D3DXVECTOR3 &getMouseBut();

	void drawLine(const WString &str);
	void drawData(const WString &name,const D3DXVECTOR4 &data);


	const D3DXVECTOR4 &getAmbient();
	void drawQuad(ID3D10EffectPassPtr pass);


	void OnD3D10SwapChainReleasing();




	static bool mInited;
};


