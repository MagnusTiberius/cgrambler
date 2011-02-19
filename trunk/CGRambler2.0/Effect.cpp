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