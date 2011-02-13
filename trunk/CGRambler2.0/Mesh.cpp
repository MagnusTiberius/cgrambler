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
#include "Mesh.h"
#include "Common.h"


Mesh::Mesh(ID3D10DevicePtr device,const WString &name,const WString &fileName,bool bCreateAdj):
mPosition(D3DXVECTOR4(0.0f,0.0f,0.0f,0.0f)),
mDirection(D3DXVECTOR4(0.0f,0.0f,1.0f,0.0f)),
mSize(D3DXVECTOR4(1.0f,1.0f,1.0f,0.0f)),
mName(name),
mDevice(device),
mFileName(fileName)
{
	mMesh=new CDXUTSDKMesh();
	V(mMesh->Create(mDevice,fileName.getLPCWSTR(),bCreateAdj));
}

Mesh::~Mesh(void)
{
	mMesh->Destroy();
	delete mMesh;
}

void Mesh::setPosition(const D3DXVECTOR4 &position)
{ 
	mPosition=position;
}

void Mesh::setDirection(const D3DXVECTOR4 &direction)
{
	mDirection=direction;
}

void Mesh::setSize(const D3DXVECTOR4 &size)
{
	mSize=size;
}

void Mesh::loadSkeleton(const WString &sktPath)
{

	WString tmp=sktPath;
	V(mMesh->LoadAnimation(tmp.getLPWSTR()));

	::D3D10_BUFFER_DESC bufferDesc=
	{
		sizeof(::D3DXMATRIX)*MAX_BONE_MATRICES,
		::D3D10_USAGE_DYNAMIC,
		::D3D10_BIND_CONSTANT_BUFFER,
		::D3D10_CPU_ACCESS_WRITE,
		0
	};

	V(mDevice->CreateBuffer(&bufferDesc,NULL,&mSkeletonBuffer));


	::D3D10_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format=::DXGI_FORMAT_R32G32B32A32_FLOAT;
	srvDesc.ViewDimension=::D3D10_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.ElementOffset=0;
	srvDesc.Buffer.ElementWidth=sizeof(::D3DXMATRIX)*4;
	V(mDevice->CreateShaderResourceView(mSkeletonBuffer,&srvDesc,&mSkeletonSRV));

}


const D3DXVECTOR4& Mesh::getPosition()
{
	return mPosition;
}

const D3DXVECTOR4& Mesh::getDirection()
{
	return mDirection;
}

const D3DXVECTOR4& Mesh::getSize()
{
	return mSize;
}

const D3DXMATRIX& Mesh::getWorldMatrix()
{
	::D3DXMATRIX Scale;
	::D3DXMatrixScaling(&Scale,mSize.x,mSize.y,mSize.z);

	::D3DXMATRIX Rotate;
	Common::getRotationTo(Rotate,::D3DXVECTOR4(0.0f,0.0f,1.0f,0.0f),mDirection);

	::D3DXMATRIX Translate;
	::D3DXMatrixTranslation(&Translate,mPosition.x,mPosition.y,mPosition.z);

	::D3DXMATRIX  mWorld=Translate*Scale*Rotate;

	return mWorld;
}



void Mesh::onFrameRender(ID3D10DevicePtr device,ID3D10EffectPassPtr pass,bool updateDiffuse,ID3D10EffectVariablePtr effectVar)
{
	UINT Strides[1];
	UINT Offsets[1];
	ID3D10BufferPtr vertexBuf[1];
	vertexBuf[0]=mMesh->GetVB10(0,0);
	Strides[0]=(UINT)mMesh->GetVertexStride(0,0);
	Offsets[0]=0;
	mDevice->IASetVertexBuffers(0,1,vertexBuf,Strides,Offsets);
	mDevice->IASetIndexBuffer(mMesh->GetIB10(0),mMesh->GetIBFormat10(0),0);

	::SDKMESH_SUBSETPtr pSubset;
	::D3D10_PRIMITIVE_TOPOLOGY PrimType;
	ID3D10ShaderResourceViewPtr pDiffuseRV;

	for(UINT subset=0;subset<mMesh->GetNumSubsets(0);subset++)
	{

		pSubset=mMesh->GetSubset(0,subset);
		PrimType=::CDXUTSDKMesh::GetPrimitiveType10((SDKMESH_PRIMITIVE_TYPE)pSubset->PrimitiveType);
		mDevice->IASetPrimitiveTopology(PrimType);
		if(updateDiffuse)
		{ 
			pDiffuseRV=mMesh->GetMaterial(pSubset->MaterialID)->pDiffuseRV10;
			effectVar->AsShaderResource()->SetResource(pDiffuseRV);
		}

		pass->Apply(0);
		device->DrawIndexed( (UINT)pSubset->IndexCount,0,(UINT)pSubset->VertexStart);


	}

}




const CDXUTSDKMeshPtr Mesh::getSDKMesh()
{
	return mMesh;
} 