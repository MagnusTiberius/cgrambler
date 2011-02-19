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

#include "DXUT.h"
#include "DXUTgui.h"
#include "DXUTsettingsdlg.h"
#include "DXUTcamera.h"
#include "SDKmesh.h"
#include "SDKmisc.h"
#include "WString.h"
#include <sstream>
#include "SmartPtr.h"
#include <iostream>
#include <fstream>
#include <ctime>
using namespace std;






class Common
{
	static	WCHAR aWStr[MAX_PATH];
	static	WCHAR bWStr[MAX_PATH];
	static char aStr[MAX_PATH];
	static char bStr[MAX_PATH];
	static bool mLogFirst;
	static DWORD dTime;
public:
	template <typename T>
	static void log(WString name,T data);
	static WString getPath(const WString &fileName);

	static void getRotationTo(D3DXMATRIX& res, D3DXVECTOR3 &src, D3DXVECTOR3 &dest);
	template <typename T>
	static void strToData(WString &str,T *t,int num);
	template <typename T>
	static void dataToStr(WString &str,T *t,int num);
	static void varToStr(WString &str,ID3D10EffectVariablePtr var);
	static void v3tov4(::D3DXVECTOR4 &res,const ::D3DXVECTOR3 &src);
	static void v4tov3(::D3DXVECTOR3 &res,const ::D3DXVECTOR4 &src);

	static void markRenderEventBegin(const WString &name,const WString &eventDesc);
	static void markRenderEventEnd();
	static float getDistance(const D3DXVECTOR3 &a,const D3DXVECTOR3 &b);
	static float getLength(const D3DXVECTOR3 &v);
	static float getAngle(const D3DXVECTOR3 &a,const D3DXVECTOR3 &m,const D3DXVECTOR3 &b);
	static void timeBegin();
	static UINT timeEnd(); 
};



ostream &operator<<(ostream &out,const D3DXVECTOR3 &v);
ostream &operator<<(ostream &out,const D3DXVECTOR4 &v);



template <typename T>
void Common::log(WString name,T data)
{

	ofstream fout;
	if(mLogFirst)
	{
		fout.open("log.txt",ios::out );
		mLogFirst=false;
	}
	else
		fout.open("log.txt",ios::out | ios::app );


	fout<<name.getLPSTR()<<":"<<data<<endl;
	fout.close(); //关闭，以防程序中途崩溃,log丢失。

}



template <typename T>
void  Common::dataToStr(WString &str,T *t,int num)
{
	int i;
	stringstream sout;
	for(i=0;i<num;i++)
		sout<<t[i]<<" ";
	sout.getline(aStr,1000);
	str=aStr;
}


template <typename T>
void  Common::strToData(WString &str,T *t,int num)
{ 
	int i;
	string cstr=str.getLPSTR();
	stringstream scin(cstr);
	for(i=0;i<num;i++)
		scin>>t[i]; 
}


