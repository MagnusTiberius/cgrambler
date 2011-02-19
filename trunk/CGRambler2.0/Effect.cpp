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
#include "Effect.h"



Effect::Effect(ID3D10DevicePtr device,const WString &name,const WString &fileName,ID3D10EffectPtr effect, map<WString,ParameterPtr> &params,map<WString,ID3D10InputLayoutPtr> &inputLayouts)
{
	mFileName=fileName;
	mDevice=device;
	mName=name;
	mEffect=effect;
	mParams=params;
	mInputLayouts=inputLayouts;
	mDiffuseType=DIFFUSE_TYPE_NONE;

	ParameterItr iter;
	for(iter=params.begin();iter!=params.end();iter++)
	{
		ParameterPtr param= iter->second;
		if( param->getSemantic()=="DIFFUSE") 
		{
			if(param->getAnnotation("ResourceName")!="")
				mDiffuseType=DIFFUSE_TYPE_SPECIFICED;
			else
				mDiffuseType=DIFFUSE_TYPE_DEFAULT;
			mDiffuseVar=param->getEffectVariable()->AsShaderResource();
		}
	}
}



Effect::~Effect()
{
	::D3D10_EFFECT_DESC desc;
	mEffect->GetDesc(&desc);

	map<WString,ID3D10InputLayoutPtr>::iterator i;
	for(i=mInputLayouts.begin();i!=mInputLayouts.end();i++)
		SAFE_RELEASE(i->second);


	ParameterItr j;
	for(j=mParams.begin();j!=mParams.end();j++)
		SAFE_DELETE(j->second);


	SAFE_RELEASE(mEffect);

}


void Effect::addMesh(MeshInstancePtr mesh)
{
	mMeshs.push_back(mesh);
}

void Effect::deleteMesh(MeshInstancePtr mesh)
{
	vector<MeshInstancePtr>::iterator iter;
	for(iter=mMeshs.begin();iter!=mMeshs.end();iter++)
		if(*iter==mesh)
		{
			mMeshs.erase(iter);
			break;
		}
}


const WString& Effect::getFileName()
{
	return mFileName;
}

map<WString,ParameterPtr> &Effect::getParams()
{
	return mParams;
}


MeshInstancePtr Effect::getMesh(UINT index)
{
	assert(0<=index && index<=mMeshs.size());
	return mMeshs[index];
}

UINT Effect::getMeshsNum()
{
	return mMeshs.size();
}


ID3D10InputLayoutPtr Effect::getInputLayout(const WString &name)
{
	assert(mInputLayouts.find(name)!=mInputLayouts.end());
	return mInputLayouts[name];
}


::ID3D10EffectTechniquePtr Effect::getTech(int index)
{
	return mEffect->GetTechniqueByIndex(index);
}


::ID3D10EffectShaderResourceVariablePtr Effect::getDiffuseVar()
{
	return mDiffuseVar;
}







DIFFUSE_TYPE Effect::getDiffuseType()
{
	return mDiffuseType;
}




ParameterPtr Effect::getParam(const WString &name)
{
	//assert(mParams.find(name)!=mParams.end());
	return mParams[name];
}


const WString &Effect::getName()
{
	return mName;
}