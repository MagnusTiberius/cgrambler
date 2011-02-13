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
#include "SmartPtr.h"
#include "Texture.h"
#include <vector>
#include "WString.h"
#include "Parameter.h"
#include "Mesh.h"
using namespace std;


enum DIFFUSE_TYPE
{
	DIFFUSE_TYPE_DEFAULT,
	DIFFUSE_TYPE_SPECIFICED,
	DIFFUSE_TYPE_NONE
};


class Effect
{
	ID3D10EffectPtr mEffect;
	WString mName;
	WString mFileName;
	ID3D10DevicePtr mDevice;
	map<WString,ParameterPtr> mParams;
	vector<MeshPtr> mMeshs;
	map<WString,ID3D10InputLayoutPtr> mInputLayouts; 
	::ID3D10EffectShaderResourceVariablePtr mDiffuseVar;
	DIFFUSE_TYPE mDiffuseType;
public:
	Effect(ID3D10DevicePtr device,const WString &name,const WString &fileName,ID3D10EffectPtr effect, map<WString,ParameterPtr> &params,map<WString,ID3D10InputLayoutPtr> &inputLayout);
	virtual ~Effect();
	void addMesh(MeshPtr mesh);
	void deleteMesh(MeshPtr mesh);
	map<WString,ParameterPtr> &getParams();
	MeshPtr getMesh(UINT index);
	UINT getMeshsNum();
	const WString &getFileName();
	ID3D10InputLayoutPtr getInputLayout(const WString &name);
	::ID3D10EffectTechniquePtr getTech(int index);
	::ID3D10EffectShaderResourceVariablePtr getDiffuseVar();

	DIFFUSE_TYPE getDiffuseType();
	ParameterPtr getParam(const WString &name);
	const WString &getName();
};

#ifdef USE_SMART_POINTER
typedef SmartPtr<Effect> EffectPtr;
#else
typedef Effect* EffectPtr;
#endif

typedef map<WString,EffectPtr>::iterator EffectItr;