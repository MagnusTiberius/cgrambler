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
#include "TextPrinter.h"



TextPrinterPtr TextPrinter::mSingleton=NULL;
ID3DX10FontPtr TextPrinter::mFont=NULL;
ID3DX10SpritePtr TextPrinter::mSprite=NULL;
CDXUTTextHelperPtr TextPrinter::mTextHelper=NULL;
WString TextPrinter::mTextBuf[MAX_PATH]={""};
UINT TextPrinter::mTextNum=0;
::ID3D10DevicePtr TextPrinter::mDevice=NULL;


TextPrinterPtr  TextPrinter::getSingleton()
{
	return mSingleton;
}


void TextPrinter::init(::ID3D10DevicePtr device)
{
	mSingleton=new TextPrinter();
	mDevice=device;
	mTextNum=0;

	D3DX10CreateFont(mDevice,15,0, FW_BOLD,1,FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH |  FF_DONTCARE, L"Arial",  &mFont);

	D3DX10CreateSprite(mDevice,512,&mSprite);
	mTextHelper=new CDXUTTextHelper(NULL,NULL,mFont,mSprite);
}



void TextPrinter::Release(void)
{
	SAFE_DELETE(mTextHelper);
	SAFE_RELEASE(mFont);
	SAFE_RELEASE(mSprite);
	delete mSingleton;
}





void TextPrinter::printLine(const WString& str)
{
	mTextBuf[mTextNum++]=str;
	if(mTextNum==MAX_PATH)
		mTextNum=0;
}


void TextPrinter::drawText(const D3DXVECTOR4 &color)
{
	mTextHelper->Begin();
	mTextHelper->SetInsertionPos(2,0);
	mTextHelper->SetForegroundColor(D3DXCOLOR(color.x,color.y,color.z,color.w));
	UINT i;
	for(i=0;i<mTextNum;i++)
		mTextHelper->DrawTextLine(mTextBuf[i].getLPCWSTR());
	mTextHelper->End();
	mTextNum=0;
}




void TextPrinter::printData(const WString &name,const D3DXVECTOR4 &data) 
{
	WCHAR str[MAX_PATH];
	swprintf(str,L"%s:(%f,%f,%f,%f)",name.getLPCWSTR(),data.x,data.y,data.z,data.w);
	printLine(str);
}
void TextPrinter::printData(const WString &name,const D3DXVECTOR3 &data) 
{
	WCHAR str[MAX_PATH];
	swprintf(str,L"%s:(%f,%f,%f)",name.getLPCWSTR(),data.x,data.y,data.z);
	printLine(str);
}



void TextPrinter::printData(const WString &name,const D3DXVECTOR2 &data)
{
	WCHAR str[MAX_PATH];
	swprintf(str,L"%s:(%f,%f)",name.getLPCWSTR(),data.x,data.y);
	printLine(str);
}


void TextPrinter::printData(const WString &name,const float &data)
{
	WCHAR str[MAX_PATH];
	swprintf(str,L"%s:%f",name.getLPCWSTR(),data);
	printLine(str);
}