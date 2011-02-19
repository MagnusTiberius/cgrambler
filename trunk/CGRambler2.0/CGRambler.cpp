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
#if defined(DEBUG) || defined(_DEBUG)
#include "vld.h" //visual leak detector,not part of cgrambler
#endif


#include "CGRambler.h"




#define KEY_INTERNAL_TIME 0.5f

HRESULT hr;

SceneManagerPtr g_pSceneManager;
CameraPtr g_pCamera;
bool g_bFPS=true;
bool g_bFullScreen=false;
UINT Width=600;
UINT Height=480;
UINT ColorBits=32;
char Str[MAX_PATH];
WCHAR wStr[MAX_PATH];
FLOAT keyPressTime=KEY_INTERNAL_TIME;


/*
注意：//shader的vs input的数据为
float3 Position	: POSITION; //即不是float4
float3 Normal	: NORMAL;   //即不是float4
float2 UV		: TEXCOORD0;//即不是float4
具体情况要看你用的sdkmesh
*/
/*
NB:the input data of vertex shader should be
float3 Position	: POSITION; //not float4
float3 Normal	: NORMAL;   //not float4
float2 UV		: TEXCOORD0;//not float4
It all depends on the sdkmesh that you are using. 
*/
void InitCGRambler()
{
	g_pSceneManager=SceneManager::getSingleton();
	g_pSceneManager->init( DXUTGetD3D10Device());

	g_pCamera=g_pSceneManager->getCamera();
	g_pCamera->setPosition(D3DXVECTOR3(0.0f,0.0f,-3));

	LightPtr light=g_pSceneManager->createLight("SpotLight",D3DXVECTOR3(20.0f,20.0f,-14.0f),D3DXVECTOR3(0.0f,0.0f,0.0f), D3DXVECTOR4(1.0f,1.0f,1.0f,1.0f),30.0f,2.0f);

	//	EffectPtr pEffect=g_pSceneManager->createEffect("effectName","Phong.fx");
	EffectPtr pEffect=g_pSceneManager->createEffect("effectName","Phong.fx");

	int i;
	int num=1;
	float offset=4;
	for(i=0;i<num;i++)
	{
		MeshInstancePtr pMesh=g_pSceneManager->createMesh("meshName","dragon.sdkmesh","");
		pMesh->setLOD(false,2);
		pMesh->setPosition( D3DXVECTOR3( 0 , 0.0f , (i-num/2)*offset ));
		// pMesh->setDirection( D3DXVECTOR3(-1,0,0) );
		// pMesh->setSize( D3DXVECTOR3(0.01f,0.01f,0.01f) );
		pEffect->addMesh(pMesh);
	}


}




WCHAR info[MAX_PATH];
void RenderText()
{
	time_t t=time(0);
	strftime(Str,sizeof(Str),"Rendered by CGRambler2.0 at %Y/%m/%d",localtime(&t));
	TextPrinter::getSingleton()->printLine(Str);
	TextPrinter::getSingleton()->printLine(L"Author:华南师范大学 08级 李海全 cgrambler@gmail.com"); //中文输出时请带L
	TextPrinter::getSingleton()->printLine(L"Press 'F' to toggle fullscreen mode");

	MeshInstancePtr pMesh=g_pSceneManager->getMesh("meshName");
	int tn=pMesh->getLODMesh()->GetNumIndices(0)/3;
	int vn=pMesh->getLODMesh()->GetNumVertices(0,0);
	swprintf(info,L"*****fileName:%s | trianglesNum:%d | verticesNum:%d*****",pMesh->getFileName().getLPCWSTR(),tn,vn);
//	TextPrinter::getSingleton()->printLine(info);
}




void CALLBACK OnGUIEvent(UINT nEvent,int uContorlID,CDXUTControl *pControl,void *pUserContext)
{

}



void initApp()
{

}



bool CALLBACK IsD3D10DeviceAcceptable(UINT Adapter,UINT Output,D3D10_DRIVER_TYPE DeviceType,DXGI_FORMAT BackBufferFormat,bool bWindowed,void *pUserContext)
{
	return true;
}


HRESULT CALLBACK OnD3D10CreateDevice(ID3D10Device *pd3dDevice,const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc,void *pUserContext)
{


	return S_OK;
}


void CALLBACK OnD3D10DestroyDevice(void *pUserContext)
{
	SAFE_RELEASE(g_pSceneManager);
}

void CALLBACK OnD3D10FrameRender(ID3D10Device *pd3dDevice,double fTime,float fElapsedTime,void *pUserContext)
{
	if(SceneManager::mInited)
	{
		g_pSceneManager->onFrameRender(fTime,fElapsedTime);
		RenderText();
	}
}

void CALLBACK OnD3D10SwapChainReleasing(void *pUserContext)
{

	if(SceneManager::mInited)
		g_pSceneManager->OnD3D10SwapChainReleasing();
}


HRESULT CALLBACK OnD3D10SwapChainResized(ID3D10Device *pd3dDevice,IDXGISwapChain *pSwapChain,const DXGI_SURFACE_DESC *pBackBufferSurfaceDesc,void *pUserContext)
{

	if(SceneManager::mInited)
		g_pSceneManager->onD3D10SwapChainResized(pSwapChain,pBackBufferSurfaceDesc); 

	return S_OK;
}


bool CALLBACK ModifyDeviceSettings(DXUTDeviceSettings *pDeviceSettings,void *pUserContext)
{ 
	return true;
}

void CALLBACK OnFrameMove(double fTime,float fElapsedTime,void *pUserContext)
{
	static bool bInited=false;
	if(!bInited)
	{
		InitCGRambler();
		bInited=true;
	}
	if(SceneManager::mInited)
		g_pSceneManager->onFrameMove(fTime,fElapsedTime);

	if( keyPressTime >0.0f)
		keyPressTime-=fElapsedTime;
}

LRESULT CALLBACK MsgProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam,bool *pbNoFurtherProcessing,void *pUserContext)
{


	if(SceneManager::mInited)
	{
		g_pSceneManager->msgProc(hWnd,uMsg,wParam,lParam);

	}

	return 0;
}

void CALLBACK KeyboardProc(UINT nChar,bool bKeyDown,bool bAltDown,void *pUserContext)
{
	if(keyPressTime<=0.0f)
	{
		if(nChar=='F')
			DXUTToggleFullScreen();
		//else if(nChar-'0'<MAX_MESH_LODS)
		// g_pSceneManager->getMesh("meshName")->setLOD(false,nChar-'0');
		keyPressTime=KEY_INTERNAL_TIME;
	}
}


INT WINAPI wWinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPWSTR lpCmdLine,int nShowCmd)
{

	DXUTSetCallbackD3D10DeviceAcceptable(IsD3D10DeviceAcceptable);
	DXUTSetCallbackD3D10DeviceCreated(OnD3D10CreateDevice);
	DXUTSetCallbackD3D10DeviceDestroyed(OnD3D10DestroyDevice);
	DXUTSetCallbackD3D10FrameRender(OnD3D10FrameRender);
	DXUTSetCallbackD3D10SwapChainReleasing(OnD3D10SwapChainReleasing);
	DXUTSetCallbackD3D10SwapChainResized(OnD3D10SwapChainResized);
	DXUTSetCallbackDeviceChanging(ModifyDeviceSettings);
	DXUTSetCallbackFrameMove(OnFrameMove);
	DXUTSetCallbackMsgProc(MsgProc);
	DXUTSetCallbackKeyboard(KeyboardProc);

	initApp();
	V(DXUTInit(true,true));
	V(DXUTCreateWindow(L"CGRambler2.0 华南师范大学 李海全 cgrambler@gmail.com"));
	V(DXUTCreateDevice(true,Width,Height));
	DXUTSetCursorSettings(true,true);
	V(DXUTMainLoop());
	return DXUTGetExitCode();

}