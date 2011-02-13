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
#include "Camera.h"

Camera::Camera( const D3DXVECTOR4 &vecEye,const D3DXVECTOR4 &vecAt,FLOAT fNear,FLOAT fFar)
{
	::D3DXVECTOR3 VecEye,VecAt;
	Common::v4tov3(VecEye,vecEye);
	Common::v4tov3(VecAt,vecAt);

	SetViewParams(&VecEye,&VecAt);
	mNear=fNear;
	mFar=fFar;

	onD3D10SwapChainResized();
	SetRotateButtons(true,false,false);

}

Camera::~Camera(void)
{
}






void Camera::setPosition( const  D3DXVECTOR4  &Pos)
{
	::D3DXVECTOR3 tmp=*GetLookAtPt();
	D3DXVECTOR3 pos;
	Common::v4tov3(pos,Pos);
	SetViewParams(&pos,&tmp);

	FrameMove(0.0f);
}
void Camera::setLookAt(  const D3DXVECTOR4 &LookAt)
{
	::D3DXVECTOR3 tmp=*GetEyePt();
	::D3DXVECTOR3 lookAt;
	Common::v4tov3(lookAt,LookAt);
	SetViewParams(&tmp,&lookAt);
	FrameMove(0.0f);
}



D3DXVECTOR4 Camera::getPosition()
{
	D3DXVECTOR4 pos;
	Common::v3tov4(pos,*GetEyePt());
	return pos;
}

D3DXVECTOR4 Camera::getLookAt()
{
	::D3DXVECTOR4 lookAt;
	Common::v3tov4(lookAt,*GetLookAtPt());
	return lookAt;
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
