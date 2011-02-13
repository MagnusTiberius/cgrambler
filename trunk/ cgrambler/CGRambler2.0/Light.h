/*
=关于官方网站：=
http://code.google.com/p/cgrambler/ 是CGRambler2.0的官方网站，你可以在那下载到本项目的最新源码，高清截图。请统一在官方网站发表评论，以便本人回复。
=About the official page:=
http://code.google.com/p/cgrambler/ is the official page of CGRambler2.0, you can download the newest source code and screenshots of this project from that site.Please comment this project at the official page, so that I can reply.

=项目简介：=
CGRambler2.0是继CGRambler1.0之后开发的一款基于DirectX 10的图形渲染引擎，关于CGRambler1.0，请浏览http://user.qzone.qq.com/499482794/blog/1285834895
相比CGRambler1.0，CGRambler2.0经过重新架构（几乎是重写），将更加注重引擎构架本身，即“看不见的渲染艺术”，而不是华丽的Shader。另外，本项目采用开源方式，可自由用于商业或非商业用途。
=Brief Introduction to CGRambler2.0:=
CGRambler2.0 is a DirectX 10 based rendering engine, for the previous version of CGRambler1.0,please see http://user.qzone.qq.com/499482794/blog/1285834895
Compare with CGrambler1.0, CGRambler2.0 have been designed from the very begining. It will focus on the architecture of the engine itself, not the gorgeous shaders. In addition, this project is completely open source, you can use it for commercial or non-commercial without permission.

=关于作者：=
华南师范大学 08级 李海全 cgrambler@gmail.com
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