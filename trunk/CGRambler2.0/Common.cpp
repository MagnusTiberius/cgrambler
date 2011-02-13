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
#include "Common.h"



WCHAR Common::path[MAX_PATH]={};
WCHAR Common::name[MAX_PATH]={};
char Common::aStr[MAX_PATH]={};
char Common::bStr[MAX_PATH]={};





WString  Common::getPath(const WString &fileName)
{
	DXUTFindDXSDKMediaFileCch(path,MAX_PATH,fileName.getLPCWSTR());

	return path;
}





void Common::getRotationTo(D3DXMATRIX& res,const D3DXVECTOR4 &Src,const D3DXVECTOR4 &Dest)
{
	::D3DXVECTOR3 src;
	::D3DXVECTOR3 dest;
	Common::v4tov3(src,Src);
	Common::v4tov3(dest,Dest);
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