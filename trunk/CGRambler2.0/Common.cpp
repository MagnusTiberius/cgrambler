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
#include "Common.h"



WCHAR Common::aWStr[MAX_PATH]={};
WCHAR Common::bWStr[MAX_PATH]={};
char Common::aStr[MAX_PATH]={};
char Common::bStr[MAX_PATH]={};
bool Common::mLogFirst=true;
DWORD Common::dTime=0;


ostream &operator<<(ostream &out,const D3DXVECTOR3 &v)
{
	out<<"("<<v.x<<","<<v.y<<","<<v.z<<")";
	return out;
}


ostream &operator<<(ostream &out,const D3DXVECTOR4 &v)
{
	out<<"("<<v.x<<","<<v.y<<","<<v.z<<","<<v.w<<")";
	return out;
}



void Common::timeBegin()
{
	dTime=GetTickCount();
}


UINT Common::timeEnd()
{
	dTime=GetTickCount()-dTime;
	return dTime;
}


void Common::markRenderEventBegin(const WString &name,const WString &eventDesc)
{
#if defined(DEBUG) || defined(_DEBUG)
	swprintf(aWStr,L"***%s:%s",name.getLPCWSTR(),eventDesc.getLPCWSTR());
	::D3DPERF_BeginEvent(0xff,aWStr);
#endif
}


float Common::getAngle(const D3DXVECTOR3 &a,const D3DXVECTOR3 &m,const D3DXVECTOR3 &b)
{
	D3DXVECTOR3 ma=D3DXVECTOR3(a.x-m.x,a.y-m.y,a.z-m.z);
	D3DXVECTOR3 mb=D3DXVECTOR3(b.x-m.x,b.y-m.y,b.z-m.z);
	::D3DXVec3Normalize(&ma,&ma);
	::D3DXVec3Normalize(&mb,&mb);
	return D3DXVec3Dot(&ma,&mb);
}


float Common::getDistance(const D3DXVECTOR3 &a,const D3DXVECTOR3 &b)
{
	D3DXVECTOR3 ab=D3DXVECTOR3(b.x-a.x,b.y-a.y,b.z-a.z);
	return   getLength(ab);
}


float Common::getLength(const D3DXVECTOR3 &v)
{
	return sqrt(D3DXVec3Dot(&v,&v));
}



void Common::markRenderEventEnd()
{
#if defined(DEBUG) || defined(_DEBUG)
	D3DPERF_EndEvent();
#endif
}


WString  Common::getPath(const WString &fileName)
{
	DXUTFindDXSDKMediaFileCch(aWStr,MAX_PATH,fileName.getLPCWSTR());
	return aWStr;
}





void Common::getRotationTo(D3DXMATRIX& res, D3DXVECTOR3 &src, D3DXVECTOR3 &dest)
{
	D3DXVECTOR3 c;
	float d,s;
	D3DXVec3Normalize(&src,&src);
	D3DXVec3Normalize(&dest,&dest);
	D3DXVec3Cross(&c,&src,&dest);
	d=D3DXVec3Dot(&src,&dest);
	s=sqrt((1.0f+d)*2.0f);
	if(fabs(s)<1e-3)
	{
		D3DXVECTOR3 axis;
		::D3DXVec3Cross(&axis,&D3DXVECTOR3(1,0,0),&src);
		if(::D3DXVec3Length(&axis)<1e-3)
			::D3DXVec3Cross(&axis,&D3DXVECTOR3(0,1,0),&src);
		D3DXMatrixRotationAxis(&res,&axis,D3DX_PI);
	}
	else
	{
		D3DXQUATERNION qRotate(c.x/s,c.y/s,c.z/s,s/2.0f);
		D3DXMatrixRotationQuaternion(&res,&qRotate);
	}
}




void Common::varToStr(WString &str,ID3D10EffectVariablePtr var)
{
	ID3D10EffectTypePtr type;
	D3D10_EFFECT_TYPE_DESC typeDesc;
	LPCSTR val;
	type=var->GetType();
	type->GetDesc(&typeDesc);
	UINT num=typeDesc.Columns*typeDesc.Rows;
	UINT psize=typeDesc.PackedSize;
	UINT stride=typeDesc.Stride;
	if(typeDesc.Type==D3D10_SVT_BOOL)
	{
		BOOL v[100];
		var->GetRawValue(v,0,sizeof(BOOL)*num);
		dataToStr(str,v,num);
	}
	else if(typeDesc.Type== D3D10_SVT_INT)
	{
		INT v[100];
		var->GetRawValue(v,0,sizeof(INT)*num);
		dataToStr(str,v,num);
	}
	else if(typeDesc.Type== D3D10_SVT_FLOAT)
	{
		FLOAT v[100];
		var->GetRawValue(v,0,sizeof(FLOAT)*num);
		dataToStr(str,v,num);
	}
	else if(typeDesc.Type== D3D10_SVT_STRING )
	{
		LPCSTR v=NULL;
		var->AsString()->GetString(&v);
		str=v;
	}
	else if(typeDesc.Type== D3D10_SVT_UINT )
	{
		UINT v[100];
		var->GetRawValue(v,0,sizeof(UINT)*num);
		dataToStr(str,v,num);
	}
}




void Common::v3tov4(::D3DXVECTOR4 &res,const ::D3DXVECTOR3 &src)
{
	res.x=src.x;
	res.y=src.y;
	res.z=src.z;
	res.w=1.0f;
}


void Common::v4tov3(::D3DXVECTOR3 &res,const ::D3DXVECTOR4 &src)
{
	res.x=src.x;
	res.y=src.y;
	res.z=src.z;
}