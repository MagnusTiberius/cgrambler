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
#include "Texture.h"


Texture::~Texture(void)
{
	SAFE_RELEASE(mTexture);
	SAFE_RELEASE(mRenderTargetView);
	SAFE_RELEASE(mDepthStencilView);
	SAFE_RELEASE(mShaderResourceView);
}



void Texture::onD3D10SwapChainResized(::IDXGISwapChainPtr pSwapChain,const ::DXGI_SURFACE_DESCPtr pBackSurfaceDesc)
{
	if(mIsManual)
	{
		SAFE_RELEASE(mTexture);
		SAFE_RELEASE(mRenderTargetView);
		SAFE_RELEASE(mDepthStencilView);
		SAFE_RELEASE(mShaderResourceView);

		init();
		createManualTexture();
	}
}



Texture::Texture(ID3D10DevicePtr device,const WString &fileName,const WString &type)
{

	mIsManual=false;
	HRESULT hr;
	mType=type;
	mDevice=device;
	mFileName=fileName;

	mWidthRatio=mHeightRatio=1.0f;
	init();
	mSRVCreated=true;

	V(D3DX10CreateShaderResourceViewFromFile(mDevice,Common::getPath(fileName).getLPCWSTR(),NULL,NULL,&mShaderResourceView,NULL));
	mShaderResourceView->GetResource((ID3D10Resource **)&mTexture);
	mTexture->GetDesc(&mDesc);

}


Texture::Texture(ID3D10DevicePtr device,const WString &fileName,FLOAT widthRatio,FLOAT heightRatio,DXGI_FORMAT format)
{
	mIsManual=true;
	mDevice=device;
	mFileName=fileName;
	mWidthRatio=widthRatio;
	mHeightRatio=heightRatio;
	mFormat=format;
	init();
	createManualTexture();
}






void Texture::createManualTexture()
{

	ZeroMemory(&mDesc,sizeof(D3D10_TEXTURE2D_DESC));

	mDesc.Format=mFormat;

	if(mFormat==DEPTH_TEXTURE_FORMAT) //depth debug pos 4
		mDesc.BindFlags=D3D10_BIND_SHADER_RESOURCE|D3D10_BIND_DEPTH_STENCIL;
	else
		mDesc.BindFlags=D3D10_BIND_SHADER_RESOURCE|D3D10_BIND_RENDER_TARGET;

	mDesc.Width=mViewport.Width;
	mDesc.Height=mViewport.Height;
	mDesc.MipLevels=1;
	mDesc.SampleDesc.Count=1;
	mDesc.Usage=::D3D10_USAGE_DEFAULT;

	mDesc.ArraySize=1;
	mDesc.CPUAccessFlags=0;
	mDesc.MiscFlags=0;
	mDesc.SampleDesc.Quality=0;

	mDevice->CreateTexture2D(&mDesc,NULL,&mTexture);

}

void Texture::createShaderResourceView()
{
	HRESULT hr;
	::D3D10_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc,sizeof(D3D10_SHADER_RESOURCE_VIEW_DESC));

	D3D10_RESOURCE_DIMENSION type;
	mTexture->GetType( &type );
	switch( type )
	{
	case D3D10_RESOURCE_DIMENSION_BUFFER:
		srvDesc.ViewDimension=::D3D10_SRV_DIMENSION_BUFFER;
		break;
	case D3D10_RESOURCE_DIMENSION_TEXTURE1D:
		srvDesc.Texture1D.MipLevels=mDesc.MipLevels;
		srvDesc.Texture1D.MostDetailedMip=0;
		srvDesc.ViewDimension=::D3D10_SRV_DIMENSION_TEXTURE1D;
		break;
	case D3D10_RESOURCE_DIMENSION_TEXTURE2D:
		srvDesc.Texture2D.MipLevels=mDesc.MipLevels;
		srvDesc.Texture2D.MostDetailedMip=0;
		if(mType=="CUBE")
			srvDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURECUBE;
		else
			srvDesc.ViewDimension=::D3D10_SRV_DIMENSION_TEXTURE2D;
		break;
	case D3D10_RESOURCE_DIMENSION_TEXTURE3D:
		srvDesc.Texture3D.MipLevels=mDesc.MipLevels;
		srvDesc.Texture3D.MostDetailedMip=0;
		srvDesc.ViewDimension=::D3D10_SRV_DIMENSION_TEXTURE3D;
		break;
	default:
		break;
	}

	if(mDesc.Format==DEPTH_TEXTURE_FORMAT) //depth debug pos3
		srvDesc.Format=::DXGI_FORMAT_R24_UNORM_X8_TYPELESS; //depth debug pos3
	else
		srvDesc.Format=mDesc.Format;
	mDevice->CreateShaderResourceView(mTexture,&srvDesc,&mShaderResourceView);

}


void Texture::createRenderTargetView()
{
	D3D10_RENDER_TARGET_VIEW_DESC rtvDesc;
	ZeroMemory(&rtvDesc,sizeof(D3D10_RENDER_TARGET_VIEW_DESC));
	rtvDesc.Format=mDesc.Format;
	rtvDesc.ViewDimension=::D3D10_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice=0;
	V(mDevice->CreateRenderTargetView(mTexture,&rtvDesc,&mRenderTargetView));
}


void Texture::createDepthStencilView()
{
	::D3D10_DEPTH_STENCIL_VIEW_DESC dsvDesc; 
	dsvDesc.Format=::DXGI_FORMAT_D24_UNORM_S8_UINT; //depth debug pos2
	dsvDesc.ViewDimension= D3D10_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice=0;
	mDevice->CreateDepthStencilView(mTexture,&dsvDesc,&mDepthStencilView);
}




ID3D10ShaderResourceViewPtr Texture::getShaderResourceView()
{

	if(!mSRVCreated)
	{
		createShaderResourceView();
		mSRVCreated=true;
	}
	return mShaderResourceView;
}

ID3D10RenderTargetViewPtr Texture::getRenderTargetView()
{
	if(!mRTVCreated)
	{
		createRenderTargetView();
		mRTVCreated=true;
	}
	return mRenderTargetView;
}

::ID3D10DepthStencilViewPtr Texture::getDepthStencilView()
{

	if(!mDSVCreated)
	{
		createDepthStencilView();
		mDSVCreated=true;
	}
	return mDepthStencilView;
}



void Texture::init()
{
	mTexture =NULL;
	mRenderTargetView =NULL;
	mDepthStencilView = NULL;
	mShaderResourceView =NULL;

	mSRVCreated=mRTVCreated=mDSVCreated=false;
	DXGI_SURFACE_DESCPtr desc;
	desc=DXUTGetDXGIBackBufferSurfaceDesc();
	FLOAT viewportWidth=desc->Width;
	FLOAT viewportHeight=desc->Height;

	mViewport.Height=mHeightRatio*viewportHeight;
	mViewport.Width=mWidthRatio*viewportWidth;
	mViewport.MaxDepth=1.0f;
	mViewport.MinDepth=0.0f;
	mViewport.TopLeftX=mViewport.TopLeftY=0;

}


UINT Texture::getWidth()
{
	return mViewport.Width;
}


UINT Texture::getHeight()
{
	return mViewport.Height;
}



const ::D3D10_VIEWPORT &Texture::getViewport()
{
	return mViewport;
}