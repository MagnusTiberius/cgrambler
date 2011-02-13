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
#include "DXUT.h"
#include "DXUTcamera.h"
#include "DXUTsettingsdlg.h"
#include "SDKmisc.h"
#include "SDKmesh.h"
#include "WString.h"
#include <map>
#include "Camera.h"
#include "Common.h"
using namespace std;

class Light:public Camera
{
private:
	WString mName;

	D3DXVECTOR4 mSpecular;
	FLOAT mFalloffAngle;  
	FLOAT mFalloffExponent;
public:
	Light(const WString &name,const D3DXVECTOR4 & pos, const D3DXVECTOR4  &lookAt,const D3DXVECTOR4 &specular,FLOAT falloffAngle,FLOAT falloffExponent,FLOAT fNear=1.0f,FLOAT fFar=100.0f);

	void setSpecular(const D3DXVECTOR4 &specular);
	void setFallofAngle(FLOAT falloffAngle);
	void setFalloffExponent(FLOAT falloffExponent);
	virtual ~Light();
	const WString& getName();

	const D3DXVECTOR4& getSpecular();
	FLOAT getFalloffAngle();
	FLOAT getFalloffExponent();
};


#ifdef USE_SMART_POINTER
typedef SmartPtr<Light> LightPtr;
typedef SmartPtr<map<WString,LightPtr>> LightMapPtr;
#else
typedef  Light* LightPtr;
#endif

typedef  map<WString,LightPtr>::iterator LightItr;