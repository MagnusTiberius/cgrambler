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
#include "SmartPtr.h"
#include "Common.h"

class Camera:
	public CFirstPersonCamera
{
	FLOAT mNear;
	FLOAT mFar;
public:
	Camera(  D3DXVECTOR3 &vecEye=D3DXVECTOR3(0.0f,0.0f,-10.0f), D3DXVECTOR3 &vecAt=D3DXVECTOR3(0.0f,0.0f,0.0f),FLOAT fNear=0.1f,FLOAT fFar=1000.0f);

	virtual ~Camera();
	void setPosition( D3DXVECTOR3 & pos);
	void setLookAt(  D3DXVECTOR3  &lookAt);

	D3DXVECTOR3  getPosition();
	D3DXVECTOR3  getLookAt();
	void onD3D10SwapChainResized();
};

#ifdef USE_SMART_POINTER
typedef SmartPtr<Camera> CameraPtr;
#else
typedef  Camera* CameraPtr;
#endif