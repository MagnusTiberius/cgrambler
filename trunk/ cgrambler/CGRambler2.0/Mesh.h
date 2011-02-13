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

#include "DXUT.h"
#include "DXUTcamera.h"
#include "DXUTsettingsdlg.h"
#include "SDKmisc.h"
#include "SDKmesh.h"
#include "resource.h"
#include "SmartPtr.h"
#include "WString.h"
#include "Common.h"
#include <map>
using namespace std;

#define MAX_BONE_MATRICES 255



class Mesh
{

	HRESULT hr;
	D3DXVECTOR4 mPosition;
	D3DXVECTOR4 mDirection;
	D3DXVECTOR4 mSize;
	WString mName;
	WString mFileName;
	ID3D10BufferPtr mSkeletonBuffer;
	ID3D10DevicePtr mDevice;
	ID3D10ShaderResourceViewPtr mSkeletonSRV;
	CDXUTSDKMeshPtr mMesh;

public:
	Mesh(ID3D10DevicePtr device,const WString &name,const WString &fileName,bool bCreateAdj);
	virtual ~Mesh();
	void setPosition(const D3DXVECTOR4 &position);
	void setDirection(const D3DXVECTOR4 &direction);
	void setSize(const D3DXVECTOR4 &size);
	void loadSkeleton(const WString &sktPath);
	const D3DXVECTOR4& getPosition();
	const D3DXVECTOR4& getDirection();
	const D3DXVECTOR4& getSize();
	const D3DXMATRIX& getWorldMatrix();
	const CDXUTSDKMeshPtr getSDKMesh();
	void onFrameRender(::ID3D10DevicePtr device,ID3D10EffectPassPtr pass,bool updateDiffuse,ID3D10EffectVariablePtr effectVar);
};

#ifdef USE_SMART_POINTER
typedef SmartPtr<Mesh> MeshPtr;
#else
typedef  Mesh* MeshPtr;
#endif