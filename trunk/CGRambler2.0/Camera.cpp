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
#include "Camera.h"

Camera::Camera(  D3DXVECTOR3 &vecEye, D3DXVECTOR3 &vecAt,FLOAT fNear,FLOAT fFar)
{

	SetViewParams(&vecEye,&vecAt);
	mNear=fNear;
	mFar=fFar;

	onD3D10SwapChainResized();
	SetRotateButtons(true,false,false);

}

Camera::~Camera(void)
{
}






void Camera::setPosition(   D3DXVECTOR3  &pos)
{
	::D3DXVECTOR3 tmp=*GetLookAtPt();
	SetViewParams(&pos,&tmp);
	FrameMove(0.0f);
}
void Camera::setLookAt(   D3DXVECTOR3 &lookAt)
{
	::D3DXVECTOR3 tmp=*GetEyePt();
	SetViewParams(&tmp,&lookAt);
	FrameMove(0.0f);
}



D3DXVECTOR3 Camera::getPosition()
{
	return *GetEyePt();
}

D3DXVECTOR3 Camera::getLookAt()
{
	return *GetLookAtPt();
}



void Camera::onD3D10SwapChainResized()
{
	DXGI_SURFACE_DESCPtr desc;
	desc=DXUTGetDXGIBackBufferSurfaceDesc();
	FLOAT width=desc->Width;
	FLOAT height=desc->Height;
	FLOAT ratio=width/height;
	SetProjParams(D3DX_PI/4.0f,ratio,mNear,mFar);

}
