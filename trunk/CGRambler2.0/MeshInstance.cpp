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
#include "MeshInstance.h"
#include "Common.h"



int MeshInstance::getLODLevel()
{
	return mLODLevel;
}


void MeshInstance::setLOD(bool enable,int level)
{
	mLODEnabled=enable;
	mLODLevel=level;





}



MeshInstance::MeshInstance(ID3D10DevicePtr device,::LODMeshPtr mesh,CameraPtr lodCamera,const WString &name,const WString &fileName):
mPosition(D3DXVECTOR3(0.0f,0.0f,0.0f)),
mDirection(D3DXVECTOR3(0.0f,0.0f,1.0f)),
mSize(D3DXVECTOR3(1.0f,1.0f,1.0f)),
mName(name),
mDevice(device),
mFileName(fileName)
{
	//fileName�Ƿ���mMeshs��mFileName��Ա���ѳ���
	mLODCamera=lodCamera;
	mMesh=mesh;

	mLODEnabled=false;
	mLODLevel=0;

}

const WString &MeshInstance::getFileName()
{
	return mFileName;
}

MeshInstance::~MeshInstance(void)
{
	int i;




}

void MeshInstance::setPosition(const D3DXVECTOR3 &position)
{ 
	mPosition=position;
}

void MeshInstance::setDirection(const D3DXVECTOR3 &direction)
{
	mDirection=direction;
}

void MeshInstance::setSize(const D3DXVECTOR3 &size)
{
	mSize=size;
}

void MeshInstance::loadSkeleton(const WString &sktPath)
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


const D3DXVECTOR3& MeshInstance::getPosition()
{
	return mPosition;
}

const D3DXVECTOR3& MeshInstance::getDirection()
{
	return mDirection;
}

const D3DXVECTOR3& MeshInstance::getSize()
{
	return mSize;
}

const D3DXMATRIX& MeshInstance::getWorldMatrix()
{
	::D3DXMATRIX Translate;
	::D3DXMatrixTranslation(&Translate,mPosition.x,mPosition.y,mPosition.z);

	::D3DXMATRIX Rotate;
	Common::getRotationTo(Rotate,::D3DXVECTOR3(0.0f,0.0f,1.0f),mDirection);

	::D3DXMATRIX Scale;
	::D3DXMatrixScaling(&Scale,mSize.x,mSize.y,mSize.z);

	::D3DXMATRIX  mWorld=Scale*Rotate*Translate;

	return mWorld;
}


//ofr
void MeshInstance::onFrameRender(ID3D10DevicePtr device,ID3D10EffectPassPtr pass,bool updateDiffuse,ID3D10EffectVariablePtr effectVar)
{

	int level;
	if(mLODEnabled)
	{
		D3DXVECTOR3 camPos=*mLODCamera->GetEyePt();
		level=mMesh->getAutoLevel(camPos,mPosition);
	}
	else
		level=mLODLevel;

	UINT Strides[1];
	UINT Offsets[1];
	ID3D10BufferPtr verticesBuf[1];
	::ID3D10BufferPtr indicesBuf;
	Strides[0]=mMesh->GetVertexStride(0,0);
	Offsets[0]=0;
	UINT indicesNum;
	mMesh->getLOD(level,&verticesBuf[0],&indicesBuf,&indicesNum);

	WCHAR str[100];
	wsprintf(str,L"LOD level:%d triangles:%d",level,indicesNum/3);
	TextPrinter::getSingleton()->printLine(str);

	mDevice->IASetVertexBuffers(0,1,verticesBuf,Strides,Offsets);
	mDevice->IASetIndexBuffer(indicesBuf,DXGI_FORMAT_R32_UINT,0);


	ID3D10ShaderResourceViewPtr pDiffuseRV;

	mDevice->IASetPrimitiveTopology(::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	if(updateDiffuse)
	{ 
		pDiffuseRV=mMesh->GetMaterial(0)->pDiffuseRV10;
		effectVar->AsShaderResource()->SetResource(pDiffuseRV);
	}
	pass->Apply(0);
	device->DrawIndexed(indicesNum,0,0);
}




const LODMeshPtr MeshInstance::getLODMesh()
{
	return mMesh;
} 