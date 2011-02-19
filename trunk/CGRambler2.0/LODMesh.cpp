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
#include "LODMesh.h"

LODMesh::LODMesh(ID3D10Device *device,const WString &fileName)
{
	mDevice=device;
	mFileName=fileName;
	memset(mLODIBs,NULL,sizeof(mLODIBs));
	memset(mLODVBs,NULL,sizeof(mLODVBs));
	memset(mLODIBLengths,0,sizeof(mLODIBLengths));

	Create(mDevice,Common::getPath(mFileName).getLPCWSTR());


	//VertexCluster可以用于非LODMesh的,另外LOD生成不仅有VertexClustering算法，所以不写LODMesh合成一个类
	mLODGenerator=new VertexCluster(mDevice,GetNumVertices(0,0),GetRawVerticesAt(0), GetVB10At(0),GetVertexStride(0,0),GetNumIndices(0),(DWORD *)GetRawIndicesAt(0),GetIB10At(0));



}


void LODMesh::getLOD(int level,ID3D10BufferPtr *vb,ID3D10BufferPtr *ib,UINT *numIndices)
{
	assert(0<=level && level<MAX_MESH_LODS);

	if(mLODVBs[level]==NULL)
		mLODGenerator->generateLOD(level,mLODVBs[level],mLODIBs[level],mLODIBLengths[level]);
	*vb=mLODVBs[level];		 
	*ib=mLODIBs[level];	
	*numIndices=mLODIBLengths[level];
}


LODMesh::~LODMesh(void)
{
	int i;
	for(i=1;i<MAX_MESH_LODS;i++)
	{
		SAFE_RELEASE(mLODIBs[i]);  
		SAFE_RELEASE(mLODVBs[i]);  
	}

	delete mLODGenerator;
	Destroy();
}






int LODMesh::getAutoLevel(const D3DXVECTOR3& camPos,const D3DXVECTOR3 &meshPos)
{
	float distance=Common::getDistance(camPos,meshPos);
	const DXGI_SURFACE_DESCPtr desc=DXUTGetDXGIBackBufferSurfaceDesc();
	float height=desc->Height;
	float R=2.0f*distance*tan(D3DX_PI/4.0f/2.0f)/height;

	int i;
	for(i=0;i<MAX_MESH_LODS;i++)
	{
		if(mLODGenerator->getGridSize(i)>=R*MESH_LOD_GRIDSIZE_PIXEL_RATIO)
			break;
	}
	if(i==MAX_MESH_LODS) i--;
	if(i==1 && mLODGenerator->getGridSize(i) >=2.0f*R*MESH_LOD_GRIDSIZE_PIXEL_RATIO) i=0;

	return i;
}