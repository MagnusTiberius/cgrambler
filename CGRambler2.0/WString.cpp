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
#include "WString.h"

WString::WString(LPWSTR str)
{
	mLBuf[0]='\0';
	if(str==NULL)
		mBuf[0]='\0';
	else
		wcscpy(mBuf,str);
}


WString::WString(LPCWSTR str)
{
	mLBuf[0]='\0';
	if(str==NULL)
		mBuf[0]='\0';
	else
		wcscpy(mBuf,str);
}

WString::WString(LPSTR str)
{
	mLBuf[0]='\0';
	if(str==NULL)
		mBuf[0]='\0';
	else
		mbstowcs(mBuf,str,WSTRING_MAX_LEN);
}

WString::WString(LPCSTR str)
{
	mLBuf[0]='\0';
	if(str==NULL)
		mBuf[0]='\0';
	else
		mbstowcs(mBuf,str,WSTRING_MAX_LEN);
}


WString::WString(const WString &str)
{
	mLBuf[0]='\0';
	if(this==&str) return;
	wcscpy(mBuf,str.getLPCWSTR());
}



WString::~WString(void)
{

}





WString WString::getUpper() const
{
	WString res=*this;
	wcsupr(res.getLPWSTR());
	return  res;

}

WString WString::getLower() const
{	 
	WString res=*this;
	wcslwr(res.getLPWSTR());
	return  res;
}



bool WString::operator==(const WString &str)const
{
	return wcscmp(mBuf,str.mBuf)==0;
}



bool WString::operator!=(const WString &str)const
{
	return !(*this==str);
}



bool WString::operator==(LPWSTR str)const
{
	return wcscmp(mBuf,str)==0;
}


bool WString::operator!=( LPWSTR str)const
{
	return  !(*this==str);
}

bool WString::operator==(LPSTR str)const
{
	return (*this==WString(str));
}

bool WString::operator!=(LPSTR str)const
{
	return !(*this==str);
}






WString WString::operator+(const WString &str)const
{
	WString res=*this;
	wcscpy(res.mBuf+wcslen(res.mBuf),str.mBuf);
	return res;
}


WString WString::operator+(LPWSTR str)const
{
	WString res=*this;
	return res+WString(str);
}


WString WString::operator+(LPSTR str)const
{
	WString res=*this;
	return res+WString(str);
}





bool WString::operator<(const WString &str)const
{
	return wcscmp(mBuf,str.mBuf)<0;
}


UINT WString::size()const
{
	return wcslen(mBuf);
}



WCHAR WString::operator[](int i) const
{
	return mBuf[i];
}









LPWSTR  WString:: getLPWSTR() 
{ 
	return mBuf;
}


LPCWSTR WString::getLPCWSTR()const
{
	return mBuf;
}




LPSTR WString::  getLPSTR() 
{
	wcstombs(mLBuf,mBuf,WSTRING_MAX_LEN);
	return mLBuf;
}


LPCSTR WString::  getLPCSTR() 
{
	wcstombs(mLBuf,mBuf,WSTRING_MAX_LEN);
	return mLBuf;
}