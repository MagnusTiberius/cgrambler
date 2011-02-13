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
#include "SceneManager.h"



SceneManagerPtr SceneManager::mSingleton=new SceneManager();
bool SceneManager::mInited=false;




void SceneManager::onD3D10SwapChainResized(IDXGISwapChainPtr pSwapChain,const DXGI_SURFACE_DESCPtr pBackBufferSurfaceDesc)
{ 
	clearShaderResources();
	clearRenderTargets();
	TextureItr iter;
	for(iter=mTextures.begin();iter!=mTextures.end();iter++)
		iter->second->onD3D10SwapChainResized(pSwapChain,pBackBufferSurfaceDesc); 

	mCamera->onD3D10SwapChainResized();

	map<WString,LightPtr>::iterator i;
	for(i=mLights.begin();i!=mLights.end();i++)
		i->second->onD3D10SwapChainResized(); 

}


void SceneManager::Release()
{
	clearShaderResources();
	clearRenderTargets();
	TextureItr iter;
	for(iter=mTextures.begin();iter!=mTextures.end();iter++)
		delete iter->second;




	OnD3D10SwapChainReleasing();
	map<WString,MeshPtr>::iterator i;
	for(i=mMeshs.begin();i!=mMeshs.end();i++)
		delete i->second;


	map<WString,EffectPtr>::iterator j;
	for(j=mEffects.begin();j!=mEffects.end();j++)
		delete j->second;


	map<WString,LightPtr>::iterator k;
	for(k=mLights.begin();k!=mLights.end();k++)
		delete k->second;

	SAFE_RELEASE(mQuadBuf);
	SAFE_DELETE(mTextHelper);
	SAFE_RELEASE(mFont);
	SAFE_RELEASE(mSprite);

	SAFE_DELETE(mCamera);
	SAFE_DELETE(mSingleton);


}





void SceneManager::OnD3D10SwapChainReleasing()
{ 
}




SceneManager::SceneManager()
{
}

//ce
EffectPtr SceneManager::createEffect(const WString &name,const WString &fileName)
{


	HRESULT hr;
	DWORD flag=D3D10_SHADER_ENABLE_BACKWARDS_COMPATIBILITY;
#if defined(_DEBUG)| defined(DEBUG)
	flag|=D3D10_SHADER_DEBUG;
#endif
	::ID3D10EffectPtr effect;

	LPWSTR p=Common::getPath(fileName).getLPWSTR();
	V(D3DX10CreateEffectFromFile(p,NULL,NULL,"fx_4_0",flag,0,mDevice,NULL,NULL,&effect,NULL,NULL));



	map<WString,ParameterPtr> params;
	WString varName,semantic,varValue;
	WString anoName,anoValue;
	D3D10_EFFECT_DESC effectDesc;
	V(effect->GetDesc(&effectDesc));
	UINT varNum=effectDesc.GlobalVariables;
	UINT anoNum;
	UINT i,j;
	ID3D10EffectVariablePtr effectVar,effectAno;
	D3D10_EFFECT_VARIABLE_DESC varDesc,anoDesc;

	float fv[2];
	FLOATPtr fp=fv;

	for(i=0;i<varNum;i++)
	{
		effectVar=effect->GetVariableByIndex(i);
		effectVar->GetDesc(&varDesc);
		varName=varDesc.Name;

		if(varDesc.Semantic!=NULL)
		{
			semantic=varDesc.Semantic;
			semantic=semantic.getUpper();
		}
		else
			semantic="";

		Common::varToStr(varValue,effectVar);
		anoNum=varDesc.Annotations;

		map<WString,WString> annotations;
		for(j=0;j<anoNum;j++)
		{
			effectAno=effectVar->GetAnnotationByIndex(j);
			effectAno->GetDesc(&anoDesc);

			anoName=anoDesc.Name;
			Common::varToStr(anoValue,effectAno);

			annotations.insert(make_pair<WString,WString>(anoName,anoValue));



		}

		ParameterPtr param=new Parameter(varName,semantic,annotations,varValue,effectVar);
		params.insert(make_pair<WString,ParameterPtr>(varName,param));
	}





	ParameterPtr param;

	WString val;
	ParameterItr iter;
	for(iter=params.begin();iter!=params.end();iter++)
	{
		param=iter->second;
		WString semantic=param->getSemantic();
		if(iter->first=="colorTarget")
			int a=1;
		if(
			semantic=="RENDERCOLORTARGET"
			|| semantic=="RENDERDEPTHSTENCILTARGET"
			|| semantic=="DIFFUSE" 
			|| semantic=="DIFFUSEMAP"
			|| semantic=="NORMAL"
			|| semantic=="SPECULAR"
			|| semantic=="SPECULARMAP"
			|| semantic=="ENVMAP"
			|| semantic=="ENVIRONMENTNORMA"
			|| semantic=="ENVIRONMENT"
			)
		{



			val=param->getAnnotation("ResourceName");
			if(val!="")
			{	 
				::D3D10_EFFECT_TYPE_DESC desc;
				createFileTexture(val,param->getAnnotation("ResourceType"));
			}

			val=param->getAnnotation("Format");
			if(val!="")
			{	 
				::D3D10_EFFECT_TYPE_DESC desc;
				WString sratio=param->getAnnotation("ViewPortRatio");
				FLOAT fratio[2]={0.0f,0.0f};
				FLOATPtr pratio=fratio;
				sscanf(sratio.getLPCSTR(),"%f %f",pratio,pratio+1);
				WString sformat=param->getAnnotation("Format");
				::DXGI_FORMAT format;
				if(sformat=="X8B8G8R8" || sformat=="X8R8G8B8")
					format=DXGI_FORMAT_R8G8B8A8_UNORM;
				if(sformat=="X16B16G16R16" || sformat=="X16R16G16B16")
					format= DXGI_FORMAT_R16G16B16A16_FLOAT;
				else if(sformat=="X32B32G32R32"|| sformat=="X16R16G16B16")
					format=DXGI_FORMAT_R32G32B32A32_FLOAT;
				else if(sformat=="D24S8_SHADOWMAP" || sformat=="D24S8")
					format=  DEPTH_TEXTURE_FORMAT ;  //depth debug pos1

				createManualTexture(param->getName(),fratio[0],fratio[1],format);

			}


		}
	}



	D3D10_PASS_SHADER_DESC passShaderDesc;
	D3D10_EFFECT_SHADER_DESC effectShaderDesc;
	D3D10_SIGNATURE_PARAMETER_DESC singnatureDesc;
	::D3D10_TECHNIQUE_DESC techDesc;
	ID3D10EffectPassPtr pass;
	::D3D10_PASS_DESC passDesc;
	ID3D10InputLayoutPtr newLayout;
	map<WString,ID3D10InputLayoutPtr> inputLayouts;
	UINT numElements;
	const int MAX_INPUT_ELEMENTS_NUM=10;
	D3D10_INPUT_ELEMENT_DESC elements[MAX_INPUT_ELEMENTS_NUM];
	D3D10_INPUT_ELEMENT_DESC newElement;

	effect->GetTechniqueByIndex(0)->GetDesc(&techDesc);





	for(j=0;j<techDesc.Passes;j++)
	{
		UINT AlignedByteOffset=0;
		int elementsNum=0;

		pass=effect->GetTechniqueByIndex(0)->GetPassByIndex(j);
		V(pass->GetVertexShaderDesc(&passShaderDesc));
		V(pass->GetDesc(&passDesc));
		V(passShaderDesc.pShaderVariable->GetShaderDesc(0,&effectShaderDesc));
		numElements=effectShaderDesc.NumInputSignatureEntries;



		for(i=0;i<numElements;i++)
		{
			V(passShaderDesc.pShaderVariable->GetInputSignatureElementDesc(0,i,&singnatureDesc));

			WString semantic=singnatureDesc.SemanticName;
			if(semantic=="POSITION"
				||semantic=="NORMAL"
				|| semantic=="NORMAL0"
				|| semantic=="TANGENT"
				|| semantic=="TANGENT0"
				||semantic=="BINORMAL"
				|| semantic=="BINORMAL0" )
			{
				newElement.AlignedByteOffset=AlignedByteOffset;
				AlignedByteOffset+=12;
				newElement.Format=::DXGI_FORMAT_R32G32B32_FLOAT;
				newElement.InputSlot=0;
				newElement.InputSlotClass=::D3D10_INPUT_PER_VERTEX_DATA;
				newElement.InstanceDataStepRate=0;
				newElement.SemanticName=singnatureDesc.SemanticName;
				newElement.SemanticIndex=0;
				elements[elementsNum++]=newElement;

			}
			else if(semantic=="TEXCOORD"
				|| semantic=="TEXCOORD0"
				|| semantic=="TEXCOORD1"
				|| semantic=="TEXCOORD2"
				|| semantic=="TEXCOORD3"
				|| semantic=="TEXCOORD4"
				|| semantic=="TEXCOORD5"
				|| semantic=="TEXCOORD6"
				|| semantic=="TEXCOORD7"
				|| semantic=="TEXCOORD8"
				|| semantic=="TEXCOORD9"
				)
			{
				newElement.AlignedByteOffset=AlignedByteOffset;
				AlignedByteOffset+=8;
				newElement.Format=::DXGI_FORMAT_R32G32_FLOAT;
				newElement.InputSlot=0;
				newElement.InputSlotClass=::D3D10_INPUT_PER_VERTEX_DATA;
				newElement.InstanceDataStepRate=0;
				newElement.SemanticName=singnatureDesc.SemanticName;
				newElement.SemanticIndex=0;
				elements[elementsNum++]=newElement;
			}
		}

		mDevice->CreateInputLayout(elements,elementsNum,passDesc.pIAInputSignature,passDesc.IAInputSignatureSize,&newLayout);
		inputLayouts[passDesc.Name]=newLayout;
	}

	EffectPtr newEffect=new Effect(mDevice,name,fileName,effect,params,inputLayouts);
	mEffects[name]=newEffect;
	return newEffect;
}
















void SceneManager::init(ID3D10DevicePtr device)
{
	mTextNum=0;
	mInited=true;
	mDevice=device;
	mCamera=new Camera();





	mMediaPath="./../../Media";

	V(DXUTSetMediaSearchPath(mMediaPath.getLPCWSTR()));
	mAmbient=::D3DXVECTOR4(0.003f,0.003f,0.003f,1.0f);



	mClearColor=D3DXVECTOR4(0.0f,0.0f,0.0f,0.0f);
	mClearDepth=1.0f;


	createQuadBuf();


	D3DX10CreateFont(mDevice,15,0, FW_BOLD,1,FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH |  FF_DONTCARE, L"Arial",  &mFont);

	D3DX10CreateSprite(mDevice,512,&mSprite);
	mTextHelper=new CDXUTTextHelper(NULL,NULL,mFont,mSprite);


	TexturePtr tex;


	mMidRTV[0]="SYSTEM_MID_RTV0";
	tex=createManualTexture(mMidRTV[0],1.0f,1.0f,DXGI_FORMAT_R8G8B8A8_UNORM);

	mMidRTV[1]="SYSTEM_MID_RTV1";
	tex=createManualTexture(mMidRTV[1],1.0f,1.0f,DXGI_FORMAT_R8G8B8A8_UNORM);


	mMidDSV[0]="SYSTEM_MID_DSV0";
	tex=createManualTexture(mMidDSV[0],1.0f,1.0f,DEPTH_TEXTURE_FORMAT);


	mMidDSV[1]="SYSTEM_MID_DSV1";
	tex=createManualTexture(mMidDSV[1],1.0f,1.0f,DEPTH_TEXTURE_FORMAT);



	createEffect("Quad","Quad.fx");

}




//ofm
void SceneManager::onFrameMove(double fTime,float fElapsedTime)
{
	mCamera->FrameMove(fElapsedTime);

	map<WString,LightPtr>::iterator i;
	for(i=mLights.begin();i!=mLights.end();i++)
		i->second->FrameMove(fElapsedTime);

	map<WString,EffectPtr>::iterator iter;
	for(iter=mEffects.begin();iter!=mEffects.end();iter++)
		paramsUpdateEveryFrame(iter->second->getParams());

}






void SceneManager::handleRenderScripts(bool techScripts,EffectPtr effect,MeshPtr mesh,const WString &scriptStr,const WString &scriptClass,const WString &scriptOrder)
{




	char cname[MAX_PATH],cvalue[MAX_PATH];
	WString name,value;
	int i,len=scriptStr.size();
	i=0;
	int p,q;
	while(i<len)
	{
		p=q=0;
		while(i<len && scriptStr[i]!='=')
			cname[p++]=scriptStr[i++];
		cname[p]='\0';
		i++;
		while(i<len && scriptStr[i]!=';')
			cvalue[q++]=scriptStr[i++];
		cvalue[q]='\0';
		i++;


		name=cname;
		name=name.getUpper();
		value=cvalue;
		value=value.getUpper();





		if( name=="RENDERCOLORTARGET" ||  name=="RENDERCOLORTARGET0" )
		{


			markRenderEventBegin(effect->getName(),mCurPassName,cname);



			if( value=="")
			{
				if(scriptOrder=="STANDARD")
					setRenderTargets(mMidRTV[0],mCurDSV);
				else if(scriptOrder=="POSTPROCESS")
				{
					setRenderTargets(mMidRTV[mCurMid],mCurDSV);
					mCurMid=(mCurMid+1)%2;
				}
			}
			else
			{
				if(techScripts && scriptOrder=="POSTPROCESS")
				{
					effect->getParam(cvalue)->getEffectVariable()->AsShaderResource()->SetResource( getTexture(mMidRTV[ (mCurMid+1)%2 ])->getShaderResourceView() ); //last mid rtv
					setRenderTargets(mMidRTV[1],mCurDSV);
				}
				else
					setRenderTargets(cvalue,mCurDSV);
			}


			markRenderEventEnd();

		}
		else if( name=="RENDERDEPTHSTENCILTARGET")
		{

			markRenderEventBegin(effect->getName(),mCurPassName,cname);


			if( value=="")
				setRenderTargets(mCurRTV,mMidDSV[0]);
			else
				setRenderTargets(mCurRTV,cvalue);


			markRenderEventEnd();



		}else if( name=="CLEARSETCOLOR" )
		{
			WString tar=effect->getParam(cvalue)->getValue();

			Common::strToData(tar,&mClearColor[0],4);

		}else if( name=="CLEARSETDEPTH")
		{
			WString tar=effect->getParam(cvalue)->getValue();
			Common::strToData(tar,&mClearDepth,1);

		}else if( name=="CLEAR" )
		{

			markRenderEventBegin(effect->getName(),mCurPassName,cname);


			if( value=="DEPTH"  && mCurDSV!=mMidDSV[0] &&  mCurDSV!=mMidDSV[1])
				mDevice->ClearDepthStencilView(getTexture(mCurDSV)->getDepthStencilView(),D3D10_CLEAR_DEPTH|D3D10_CLEAR_STENCIL,mClearDepth,0);
			else if( value=="COLOR"  &&  mCurRTV!=mMidRTV[0] && mCurRTV!=mMidRTV[1])
				mDevice->ClearRenderTargetView( getTexture(mCurRTV)->getRenderTargetView() ,mClearColor);


			markRenderEventEnd();

		}else if( name=="PASS" )
		{

			markRenderEventBegin(effect->getName(),mCurPassName,cname);

			mCurPass=effect->getTech(0)->GetPassByName(cvalue);

			mCurPassName=cvalue;


			mDevice->IASetInputLayout(effect->getInputLayout(cvalue));

			::D3D10_PASS_DESC passDesc;
			::D3D10_EFFECT_VARIABLE_DESC varDesc;
			WString anoName,anoValue;
			::ID3D10EffectVariablePtr var;

			LPCSTR str;
			int i,j;
			mCurPass->GetDesc(&passDesc);




			for(i=0;i<passDesc.Annotations;i++)
			{
				var=mCurPass->GetAnnotationByIndex(0);
				var->GetDesc(&varDesc);
				anoName=varDesc.Name;
				var->AsString()->GetString(&str);
				anoValue=str;
				handleRenderScripts(false,effect,mesh,anoValue,scriptClass,scriptOrder);
			}

			markRenderEventEnd();
		} 
		else if( name=="DRAW")
		{
			markRenderEventBegin(effect->getName(),mCurPassName,cname);
			ID3D10RenderTargetViewPtr rtv[]={getTexture(mCurRTV)->getRenderTargetView()};
			mDevice->OMSetRenderTargets(1,rtv,getTexture(mCurDSV)->getDepthStencilView());
			mDevice->RSSetViewports(1,&getTexture(mCurRTV)->getViewport());
			if(scriptClass=="OBJECT" ||scriptOrder=="POSTPROCESS")
			{
				if(WString(value).getUpper()=="GEOMETRY")
					mesh->onFrameRender(mDevice,mCurPass,effect->getDiffuseType()==DIFFUSE_TYPE_DEFAULT,effect->getDiffuseVar());
				else if(WString(value).getUpper()=="BUFFER")
					drawQuad(mCurPass);
			}
			else //scene   sceneorobject
			{
				for(UINT i=0;i<effect->getMeshsNum();i++)
				{
					mesh=effect->getMesh(i);
					paramsUpdateEveryMesh(effect->getParams(),mesh);
					if(WString(value).getUpper()=="GEOMETRY")
						mesh->onFrameRender(mDevice,mCurPass,effect->getDiffuseType()==DIFFUSE_TYPE_DEFAULT,effect->getDiffuseVar());
					else if(WString(value).getUpper()=="BUFFER")
						drawQuad(mCurPass);

				}
			}
			markRenderEventEnd();

		}


	}







}






//ofr
void SceneManager::onFrameRender(double fTime,float fElapsedTime)
{



	mCurMid=0;
	mDevice->ClearRenderTargetView(getTexture(mMidRTV[0])->getRenderTargetView(),mClearColor);
	mDevice->ClearDepthStencilView(getTexture(mMidDSV[0])->getDepthStencilView(),D3D10_CLEAR_DEPTH|D3D10_CLEAR_STENCIL,mClearDepth,0.0f);

	setRenderTargets(mMidRTV[0],mMidDSV[0]);



	UINT i,j;
	map<WString,EffectPtr>::iterator effectIter;
	::ID3D10EffectTechniquePtr tech;
	::D3D10_TECHNIQUE_DESC techDesc;
	::ID3D10EffectVariablePtr anoVar;
	::D3D10_EFFECT_VARIABLE_DESC varDesc;
	WString name,value;
	::ID3D10EffectPassPtr pass;
	::D3D10_PASS_DESC passDesc;
	UINT meshNum;



#define STANDARD 0
#define POSTPROCESS 1
	//standard
	for(UINT k=0;k<2;k++)
		for(effectIter=mEffects.begin();effectIter!=mEffects.end();effectIter++)
		{
			EffectPtr effect=effectIter->second;
			meshNum=effect->getMeshsNum();
			if(meshNum==0) continue;

			WString scriptOrder=effect->getParam("Script")->getAnnotation("ScriptOrder");
			scriptOrder=scriptOrder.getUpper();

			if(k==STANDARD && scriptOrder=="POSTPROCESS") continue;

			if(k==POSTPROCESS && scriptOrder=="STANDARD") continue;


			WString scriptClass=effect->getParam("Script")->getAnnotation("ScriptClass");
			scriptClass=scriptClass.getUpper();
			if(scriptClass!="OBJECT" || k==POSTPROCESS)
				meshNum=1;
			for(i=0;i<meshNum;i++)
			{
				paramsUpdateEveryMesh(effect->getParams(),effect->getMesh(i));
				tech=effect->getTech(0);
				tech->GetDesc(&techDesc);
				for(j=0;j<techDesc.Annotations;j++)
				{
					anoVar=tech->GetAnnotationByIndex(i);
					anoVar->GetDesc(&varDesc);
					name=varDesc.Name;
					Common::varToStr(value,anoVar);
					handleRenderScripts(true,effect,effect->getMesh(i),value,scriptClass,scriptOrder);
				}
			} 

			clearShaderResources();
			clearRenderTargets();

			if(k==STANDARD)
			{
				mDevice->ClearDepthStencilView(getTexture(mMidDSV[0])->getDepthStencilView(),D3D10_CLEAR_DEPTH|D3D10_CLEAR_STENCIL,mClearDepth,0.0f);
				mCurMid=1;
			}
		}



		presentToScreen(   mMidRTV[ (mCurMid+1)%2]   );


}




EffectPtr SceneManager::getEffect(const WString &name)
{
	assert(mEffects.find(name)!=mEffects.end());
	return mEffects[name];
}


//fileName是否在mMeshs的mFileName成员中已出现
MeshPtr SceneManager::createMesh(const WString &name,const WString &meshFileName,const WString &skeFileName,const D3DXVECTOR4 &pos,const D3DXVECTOR4 &dir,const D3DXVECTOR4 &size)
{


	MeshPtr newMesh=new Mesh(mDevice,name,meshFileName,true);
	newMesh->setPosition(pos);
	newMesh->setDirection(dir);
	newMesh->setSize(size);
	if(skeFileName!="")
		newMesh->loadSkeleton(skeFileName);
	mMeshs[name]=newMesh;
	return newMesh;
}


MeshPtr SceneManager::getMesh(const WString &name)
{
	assert(mMeshs.find(name)!=mMeshs.end());
	return mMeshs[name];
}


void SceneManager::setAmbientColor(const D3DXVECTOR4 &color)
{
	mAmbient=color;
}

//检查texBuf的mFileName是否在mTexture的成员中已出现
TexturePtr SceneManager::createFileTexture(const WString &fileName,const WString &type)
{
	TextureItr iter;
	iter=mTextures.find(fileName);
	if(iter==mTextures.end())
	{
		TexturePtr tex=new Texture(mDevice,fileName,type);	
		mTextures[fileName]=tex;
		return tex;
	}
	else
		return iter->second;
}


TexturePtr SceneManager::createManualTexture(const WString &fileName,FLOAT widthRatio,FLOAT heightRatio,DXGI_FORMAT format)
{
	TexturePtr tex=new Texture(mDevice,fileName,widthRatio,heightRatio,format);
	mTextures[fileName]=tex;
	return tex;
}


void SceneManager::msgProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	mCamera->HandleMessages(hWnd,uMsg,wParam,lParam);


	map<WString,LightPtr>::iterator i;
	for(i=mLights.begin();i!=mLights.end();i++)
		i->second->HandleMessages(hWnd,uMsg,wParam,lParam);
}

LightPtr SceneManager::createLight(const WString &name,const D3DXVECTOR4 &pos,const D3DXVECTOR4 &lookAt,const D3DXVECTOR4 &specular,FLOAT falloffAngle,FLOAT falloffExponent)
{
	LightPtr light=new Light(name,pos,lookAt,specular,falloffAngle,falloffExponent);

	mLights[name]=light;
	return light;
}

void SceneManager::deleteLight(const WString &name)
{
	mLights.erase(name);
}


LightPtr SceneManager::getLight(const WString &name)
{
	assert(mLights.find(name)!=mLights.end());
	return mLights[name];
}


void SceneManager::setMediaPath(const WString &path)
{

	DXUTSetMediaSearchPath(path.getLPCWSTR());
}

SceneManagerPtr SceneManager::getSingleton()
{
	return mSingleton;
}

FLOAT SceneManager::getTime()
{
	return mTime;
}

FLOAT SceneManager::getElapsedTime()
{
	return mElapsedTime;
}

const D3DXVECTOR3 &SceneManager::getMousePos()
{
	return mMousePos;
}


const D3DXVECTOR3 &SceneManager::getMouseBut()
{
	return mMouseBut;
}










//puef
void SceneManager::paramsUpdateEveryFrame(map<WString,ParameterPtr> &params)
{


	ParameterPtr param;
	int i,j; 
	WString tar;
	ParameterItr iter;
	for(iter=params.begin();iter!=params.end();iter++)
	{
		param=iter->second;

		WString semantic=param->getSemantic();


		//Time
		if(semantic=="TIME")
		{
			param->getEffectVariable()->AsScalar()->SetFloat(mTime);
		}
		else if(semantic=="ELAPSEDTIME")
		{
			param->getEffectVariable()->AsScalar()->SetFloat(mElapsedTime);
		}
		else if(semantic=="MOUSEPOSITION")
		{
			param->getEffectVariable()->AsVector()->SetFloatVector(mMousePos);
		}
		else if(semantic=="MOUSEDOWN")
		{
			param->getEffectVariable()->AsVector()->SetFloatVector(mMouseBut);
		}else if(semantic=="AMBIENT")
		{
			param->getEffectVariable()->AsVector()->SetFloatVector(mAmbient);
		}else if(semantic=="VIEWPORTPIXELSIZE")
		{
			D3D10_VIEWPORT viewPort;
			UINT num=1;
			mDevice->RSGetViewports(&num,&viewPort);
			FLOAT vp[]={viewPort.Width,viewPort.Height};
			param->getEffectVariable()->AsVector()->SetFloatVector(vp);
		}
		//Light begin
		else if(semantic=="POSITION")
		{
			tar=param->getAnnotation("Object");
			D3DXVECTOR4 val=getLight(tar)->getPosition();

			param->getEffectVariable()->AsVector()->SetFloatVector(val);
		}
		else if(semantic=="DIRECTION")
		{
			tar=param->getAnnotation("Object");
			D3DXVECTOR4 val=getLight(tar)->getLookAt()-getLight(tar)->getPosition();
			param->getEffectVariable()->AsVector()->SetFloatVector(val);
		}
		else if(semantic=="SPECULAR")
		{
			tar=param->getAnnotation("Object");
			D3DXVECTOR4 val=getLight(tar)->getSpecular();
			param->getEffectVariable()->AsVector()->SetFloatVector(val);
		}
		else if(semantic=="FALLOFFANGLE")
		{
			tar=param->getAnnotation("Object");
			FLOAT val=getLight(tar)->getFalloffAngle();
			param->getEffectVariable()->AsScalar()->SetFloat(val);
		}
		else if(semantic=="FALLOFFEXPONENT")
		{
			tar=param->getAnnotation("Object");
			FLOAT val=getLight(tar)->getFalloffExponent();
			param->getEffectVariable()->AsScalar()->SetFloat(val);
		}

		//Light end
		//srv begin
		else if(	semantic=="RENDERCOLORTARGET"
			|| semantic=="RENDERDEPTHSTENCILTARGET"
			|| semantic=="DIFFUSE" 
			|| semantic=="DIFFUSEMAP"
			|| semantic=="NORMAL"
			|| semantic=="SPECULAR"
			|| semantic=="SPECULARMAP"
			|| semantic=="ENVMAP"
			|| semantic=="ENVIRONMENTNORMA"
			|| semantic=="ENVIRONMENT")
		{
			WString texName;
			if(param->getAnnotation("ResourceName")!="")
				texName=param->getAnnotation("ResourceName");
			else if(param->getAnnotation("Format")!="")
				texName=param->getName();
			else if(param->getAnnotation("External")!="")
				texName=param->getAnnotation("External");

			if(texName!="")
				param->getEffectVariable()->AsShaderResource()->SetResource( getTexture( texName )->getShaderResourceView() );
		}
		//srv end
		//matrix begin
		else if( (tar=param->getAnnotation("Object"))!="")
		{
			LightPtr light=getLight(tar);

			::D3DXMATRIX world;
			::D3DXMatrixIdentity(&world);

			updataMatrixs(param->getEffectVariable()->AsMatrix(),semantic,world,*light->GetViewMatrix(),*light->GetProjMatrix());
		}
		//matrix end




	}
}




void SceneManager::paramsUpdateEveryMesh(map<WString,ParameterPtr> &params,MeshPtr mesh)
{
	::D3DXMATRIX world,view,proj;

	world=mesh->getWorldMatrix();
	view=*mCamera->GetViewMatrix();
	proj=*mCamera->GetProjMatrix();
	ParameterPtr param;
	int i,j,numParams=params.size();
	ParameterItr iter;
	ID3D10EffectMatrixVariablePtr var;


	for(iter=params.begin();iter!=params.end();iter++)
	{
		param=iter->second;
		var=param->getEffectVariable()->AsMatrix();
		if(param->getAnnotation("Object")!="")
			continue;
		updataMatrixs(var,param->getSemantic(),world,view,proj);
	}
}




CameraPtr SceneManager::getCamera()
{
	return mCamera;
}








void SceneManager::updataMatrixs(::ID3D10EffectMatrixVariablePtr var,const WString& semantic,  D3DXMATRIX  world,   D3DXMATRIX  view,  D3DXMATRIX  proj)
{
	::D3DXMATRIX mres;
	FLOAT fres;
	if(semantic=="WORLD")
	{
		var->SetMatrix(world);
	}
	else if(semantic=="VIEW")
	{
		var->SetMatrix(view);
	}
	else if(semantic=="PROJECTION")
	{
		var->SetMatrix(proj);
	}
	else if(semantic=="WORLDVIEW")
	{
		var->SetMatrix(world*view);
	}
	else if(semantic=="VIEWPROJECTION")
	{
		var->SetMatrix(view*proj);
	}
	else if(semantic=="WORLDVIEWPROJECTION")
	{
		var->SetMatrix(world*view*proj);
	}
	else if(semantic=="WORLDINVERSE")
	{
		var->SetMatrix(*D3DXMatrixInverse(&mres,&fres,&world));
	}
	else if(semantic=="VIEWINVERSE")
	{
		var->SetMatrix(*D3DXMatrixInverse(&mres,&fres,&view));
 

	}
	else if(semantic=="PROJECTIONINVERSE")
	{
		var->SetMatrix(*D3DXMatrixInverse(&mres,&fres,&proj));
	}
	else if(semantic=="WORLDVIEWINVERSE")
	{
		var->SetMatrix(*D3DXMatrixInverse(&mres,&fres,&(world*view)));
	}
	else if(semantic=="VIEWPROJECTIONINVERSE")
	{
		var->SetMatrix(*D3DXMatrixInverse(&mres,&fres,&(view*proj)));
	}
	else if(semantic=="WORLDVIEWPROJECTIONINVERSE")
	{
		var->SetMatrix(*D3DXMatrixInverse(&mres,&fres,&(world*view*proj)));
	}
	else if(semantic=="WORLDTRANSPOSE")
	{
		var->SetMatrix(*D3DXMatrixTranspose(&mres,&world));
	}
	else if(semantic=="VIEWTRANSPOSE")
	{
		var->SetMatrix(*D3DXMatrixTranspose(&mres,&view));
	}
	else if(semantic=="PROJECTIONTRANSPOSE")
	{
		var->SetMatrix(*D3DXMatrixTranspose(&mres,&proj));
	}
	else if(semantic=="WORLDVIEWTRANSPOSE")
	{
		var->SetMatrix(*D3DXMatrixTranspose(&mres,&(world*view)));
	}
	else if(semantic=="VIEWPROJECTIONTRANSPOSE")
	{
		var->SetMatrix(*D3DXMatrixTranspose(&mres,&(view*proj)));
	}
	else if(semantic=="WORLDVIEWPROJECTIONTRANSPOSE")
	{
		var->SetMatrix(*D3DXMatrixTranspose(&mres,&(world*view*proj)));
	}

	else if(semantic=="WORLDINVERSETRANSPOSE")
	{
		var->SetMatrix(*D3DXMatrixTranspose(&mres,D3DXMatrixInverse(&mres,&fres,&world)));
	}
	else if(semantic=="VIEWINVERSETRANSPOSE")
	{
		var->SetMatrix(*D3DXMatrixTranspose(&mres,D3DXMatrixInverse(&mres,&fres,&view)));
	}
	else if(semantic=="PROJECTIONINVERSETRANSPOSE")
	{
		var->SetMatrix(*D3DXMatrixTranspose(&mres,D3DXMatrixInverse(&mres,&fres,&proj)));
	}
	else if(semantic=="WORLDVIEWINVERSETRANSPOSE")
	{
		var->SetMatrix(*D3DXMatrixTranspose(&mres,D3DXMatrixInverse(&mres,&fres,&(world*view))));
	}
	else if(semantic=="VIEWPROJECTIONINVERSETRANSPOSE")
	{
		var->SetMatrix(*D3DXMatrixTranspose(&mres,D3DXMatrixInverse(&mres,&fres,&(view*proj))));
	}
	else if(semantic=="WORLDVIEWPROJECTIONINVERSETRANSPOSE")
	{
		var->SetMatrix(*D3DXMatrixTranspose(&mres,D3DXMatrixInverse(&mres,&fres,&(world*view*proj))));
	}

}



void SceneManager::createQuadBuf()
{



	QuadVertex Quad[]=
	{
		D3DXVECTOR3(-1.0f,1.0f,0.0f),D3DXVECTOR2(0.0f,0.0f),
		D3DXVECTOR3(1.0f,1.0f,0.0f),D3DXVECTOR2(1.0f,0.0f),

		D3DXVECTOR3(-1.0f,-1.0f,0.0f),D3DXVECTOR2(0.0f,1.0f),
		D3DXVECTOR3(1.0f,-1.0f,0.0f),D3DXVECTOR2(1.0f,1.0f),

	};

	D3D10_BUFFER_DESC desc;
	desc.BindFlags=D3D10_BIND_VERTEX_BUFFER;
	desc.ByteWidth=sizeof(QuadVertex)*4;
	desc.CPUAccessFlags=0;
	desc.MiscFlags=0;
	desc.Usage=D3D10_USAGE_DEFAULT;

	::D3D10_SUBRESOURCE_DATA data;
	data.pSysMem=Quad;
	data.SysMemPitch=0;
	data.SysMemSlicePitch=0;
	mDevice->CreateBuffer(&desc,&data,&mQuadBuf);

}



void SceneManager::drawQuad( ID3D10EffectPassPtr pass)
{
	mDevice->ClearDepthStencilView( getTexture(mCurDSV)->getDepthStencilView() ,::D3D10_CLEAR_DEPTH | ::D3D10_CLEAR_STENCIL,mClearDepth,0.0f);
	 
	UINT Strides[]={sizeof(QuadVertex)};
	UINT Offsets[]={0};
	mDevice->IASetVertexBuffers(0,1,&mQuadBuf,Strides,Offsets);
	mDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	 
	pass->Apply(0);
	mDevice->Draw(4,0);
}




TexturePtr SceneManager::getTexture(const WString &name)
{
	assert(mTextures.find(name)!=mTextures.end());
	return mTextures[name];
}










void SceneManager::clearShaderResources()
{ 
	ID3D10ShaderResourceViewPtr pNull[]={NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
	mDevice->VSSetShaderResources(0,8,pNull);
	mDevice->GSSetShaderResources(0,8,pNull);
	mDevice->PSSetShaderResources(0,8,pNull);
}

void SceneManager::clearRenderTargets()
{
	::ID3D10RenderTargetViewPtr pRTV_NULL[]={NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
	::ID3D10DepthStencilViewPtr pDSV_NULL=NULL;
	mDevice->OMSetRenderTargets(8,pRTV_NULL,pDSV_NULL);
}


void SceneManager::drawLine(const WString& str)
{
	mTextBuf[mTextNum++]=str;
	if(mTextNum==MAX_PATH)
		mTextNum=0;
}


void SceneManager::drawText(const D3DXVECTOR4 &color)
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






void SceneManager::drawData(const WString &name,const D3DXVECTOR4 &data) 
{
	WCHAR str[MAX_PATH];
	swprintf(str,L"%s:%f %f %f %f",name.getLPCWSTR(),data.x,data.y,data.z,data.w);
	drawLine(str);
}





void  SceneManager::setRenderTargets(const WString &rtv,const WString &dsv)
{
	mCurRTV=rtv;
	mCurDSV=dsv;

}







const D3DXVECTOR4 &SceneManager::getAmbient()
{
	return mAmbient;
}






void SceneManager::presentToScreen(const WString &target)
{

	ID3D10RenderTargetViewPtr rtv[]={DXUTGetD3D10RenderTargetView()};

	mDevice->ClearRenderTargetView(rtv[0],mClearColor);
	mDevice->ClearDepthStencilView(::DXUTGetD3D10DepthStencilView(),D3D10_CLEAR_DEPTH|D3D10_CLEAR_STENCIL,mClearDepth,0.0f);
	getEffect("Quad")->getParam("tex")->getEffectVariable()->AsShaderResource()->SetResource(   getTexture( target )->getShaderResourceView() );

	mDevice->OMSetRenderTargets(1, &rtv[0], DXUTGetD3D10DepthStencilView());
	mDevice->IASetInputLayout(getEffect("Quad")->getInputLayout("p0"));
	mDevice->IASetPrimitiveTopology(::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	drawQuad(getEffect("Quad")->getTech(0)->GetPassByName("p0"));

	drawText();		
	clearRenderTargets();
	clearShaderResources();
}




void SceneManager::markRenderEventBegin(const WString &effectName,const WString &passName,const WString &events)
{
#if defined(DEBUG) || defined(_DEBUG)
	WCHAR str[MAX_PATH];
	swprintf(str,L"CurEffectName:%s mCurPassName:%s evnts:%s",effectName.getLPCWSTR(),passName.getLPCWSTR(),events.getLPCWSTR());
	::D3DPERF_BeginEvent(0xff,str);
#endif
}



void SceneManager::markRenderEventEnd()
{
#if defined(DEBUG) || defined(_DEBUG)
	D3DPERF_EndEvent();
#endif
}