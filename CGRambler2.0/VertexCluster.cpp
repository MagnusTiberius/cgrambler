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
#include "VertexCluster.h"







float VertexCluster::getTolerance()
{
	return mTolerance;
}



void VertexCluster::calculateWeights()
{
	//为了速度，尽可能不调用外部函数,少用循环
	//Common::timeBegin();

	int i,j,k;
	int a,b,c;
	D3DXVECTOR3 vab,vbc,vca;
	float lab,lbc,lca;
	float abc,bca,cab;
	double lsum=0.0f;

	for(i=0;i<mIndicesNum;i+=3)
	{
		a=mIndices[i];
		b=mIndices[i+1];
		c=mIndices[i+2];

		vab=*locateVertex(b)-*locateVertex(a);
		vbc=*locateVertex(c)-*locateVertex(b);
		vca=*locateVertex(c)-*locateVertex(a);
		//为了速度，把sqrt省了
		lab=sqrt(vab.x*vab.x+vab.y*vab.y+vab.z*vab.z);  lsum+=lab;
		lbc=sqrt(vbc.x*vbc.x+vbc.y*vbc.y+vbc.z*vbc.z);  lsum+=lbc;
		lca=sqrt(vca.x*vca.x+vca.y*vca.y+vca.z*vca.z);  lsum+=lca;

		mEdgeLengths[a]=max(mEdgeLengths[a],lab);  mEdgeLengths[b]=max(mEdgeLengths[b],lab);
		mEdgeLengths[b]=max(mEdgeLengths[b],lbc);  mEdgeLengths[c]=max(mEdgeLengths[c],lbc);
		mEdgeLengths[c]=max(mEdgeLengths[c],lca);  mEdgeLengths[a]=max(mEdgeLengths[a],lca);

		vab/=lab; vbc/=lbc; vca/=lca;

		abc=-vab.x*vbc.x-vab.y*vbc.y-vab.z*vbc.z;
		bca=-vbc.x*vca.x-vbc.y*vca.y-vbc.z*vca.z;
		cab=-vca.x*vab.x-vca.y*vab.y-vca.z*vab.z;

		mAngles[b]=max(mAngles[b],abc);
		mAngles[c]=max(mAngles[c],bca);
		mAngles[a]=max(mAngles[a],cab);
	}
	mAvgEdgeLength=lsum/double(mIndicesNum);

	for(i=0;i<mVerticesNum;i++)
	{
		mWeights[i].mID=i;
		mWeights[i].mWeight=mAngles[i]*MESH_LOD_ANGLE_FACTOR+mEdgeLengths[i]*MESH_LOD_EDGE_LENGTH_FACTOR;
		// Common::log("*****id ",i);
		// Common::log("mAngles ",mAngles[i]);
		// Common::log("mEdgeLengths ",mEdgeLengths[i]);
		// Common::log("mWeight ",mWeights[i].mWeight);
	}
	//Common::log("mAvgEdgeLength",mAvgEdgeLength);

	//Common::log("dTime", Common::timeEnd());
}



VertexCluster::VertexCluster( ID3D10Device *device,UINT verticesNum,BYTE *rawVB,::ID3D10BufferPtr VB,UINT stride,UINT indicesNum,DWORD *rawIB,::ID3D10BufferPtr IB)
{
	//Common::timeBegin();
	
	mOriginalVB=VB;
	mOriginalIB=IB;

	mDevice=device;
	mStride=stride;

	mIndicesNum=indicesNum;
	mVerticesNum=verticesNum;

	mVertices=rawVB;
	mIndices=rawIB;

	mHashTable=new GridHashTable(mVerticesNum,mStride);

	int i,j;
	//Common::log("mIndicesNum",mIndicesNum);
	//Common::log("mVerticesNum",mVerticesNum);
	// for(i=0;i<mVerticesNum;i++)
	//	Common::log("Original Vertex",*locateVertex(i));

	mLODIB=new DWORD[mIndicesNum];
	mWeights=new VertexWeight[mVerticesNum];
	mEdgeLengths=new float[mVerticesNum];
	mAngles=new float[mVerticesNum];

	memset(mEdgeLengths,0.0f,sizeof(float)*mVerticesNum);
	memset(mAngles,-1.0f,sizeof(float)*mVerticesNum);
	//mSortedVertexIDs=new int[mVerticesNum];

	calculateWeights();
	sortVertices();

	//Common::log("init time",Common::timeEnd());
}



float VertexCluster::getGridSize(float level) 
{
	return mAvgEdgeLength*MESH_LOD_STRENGTH*level/float(MAX_MESH_LODS-1.0f);
}


//level,0表示原始mesh
//level,0 is original mesh

//debug!vb也应该生成一个新的！
void VertexCluster::generateLOD(float level,ID3D10BufferPtr &vb,ID3D10BufferPtr &ib,UINT &ibLength)  //mVertexValueCreated=true
{

	 //Common::timeBegin();


	D3D10_BUFFER_DESC desc;
	desc.BindFlags= D3D10_BIND_INDEX_BUFFER;
	desc.CPUAccessFlags=0;
	desc.MiscFlags=0;
	desc.Usage=D3D10_USAGE_DEFAULT;

	D3D10_SUBRESOURCE_DATA initData;

	if(level<=0.0f)
	{
		vb=mOriginalVB;
		ib=mOriginalIB;
		ibLength=mIndicesNum;
		return ;
	}

	mTolerance=getGridSize(level);
	mHashTable->init(mTolerance);

	//Common::log("mTolerance",mTolerance);

	int i,ri;
	for(i=0;i<mVerticesNum;i++)
	{
		ri=mWeights[i].mID;
		mHashTable->addVertex(ri,locateVertex(ri),mWeights[i].mWeight); 
	}

	ibLength=0;
	int a,b,c;
	int ra,rb,rc;
	for(i=0;i<mIndicesNum;i+=3)
	{
		a=mIndices[i];
		b=mIndices[i+1];
		c=mIndices[i+2];

		ra=mHashTable->getReplacedID(a);
		rb=mHashTable->getReplacedID(b);
		rc=mHashTable->getReplacedID(c);

		if(ra!=rb && ra!=rc && rb!=rc)
		{
			mLODIB[ibLength++]=ra;
			mLODIB[ibLength++]=rb;
			mLODIB[ibLength++]=rc;


			//Common::log("lod face index a",ra);
			//Common::log("lod face index b",rb);
			//Common::log("lod face index c",rc);
		}
	}

	//for(i=0;i<mVerticesNum;i++)
	//	Common::log("ReplacedID",mHashTable->getReplacedID(i));

	//Common::log("LOD generated! with Indices num",ibLength);
	if(ibLength==0)
	{
		vb=NULL;
		ib=NULL;
		return;
	}
	desc.ByteWidth=sizeof(DWORD)*ibLength;
	initData.pSysMem=mLODIB;
	mDevice->CreateBuffer(&desc,&initData,&ib);


	// for(i=0;i<ibLength;i++)
	//	Common::log("IB",mLODIB[i]);


	desc.BindFlags=D3D10_BIND_VERTEX_BUFFER;
	desc.ByteWidth=mStride*mHashTable->getCellsNum();
	initData.pSysMem=mHashTable->getCellsVB();
	mDevice->CreateBuffer(&desc,&initData,&vb);



	//  for(i=0;i<mHashTable->getCellsNum();i++)
	//	Common::log("LOD VB",*mHashTable->locateVertex(i));



	//Common::log("LOD generating time",Common::timeEnd());
}

D3DXVECTOR3 *VertexCluster::locateVertex(int id)
{
	return (D3DXVECTOR3*)(mVertices+id*mStride);
}

/*
void VertexCluster::quickSort(int begin,int end)
{
if(begin>=end) return;
int a,b;
float x;
a=begin,b=end,x=mWeights[begin];
int id=mSortedVertexIDs[begin];
while(a<b)
{
while(a<b && mWeights[b]<=x ) b--;
mWeights[a]=mWeights[b] ;
mSortedVertexIDs[a]=mSortedVertexIDs[b];
while(a<b && mWeights[a]>=x ) a++;
mWeights[b]=mWeights[a] ;
mSortedVertexIDs[b]=mSortedVertexIDs[a];
}
mWeights[b]=x;
mSortedVertexIDs[b]=id;
quickSort(begin,b-1);
quickSort(b+1,end);
}
*/



void VertexCluster::sortVertices()
{ 
	sort(mWeights,mWeights+mVerticesNum);
	//Common::timeBegin();
	//	int i;
	//	for(i=0;i<mVerticesNum;i++)
	//		mSortedVertexIDs[i]=i;
	//	quickSort(0,mVerticesNum-1);
	//
	////for(i=0;i<mVerticesNum;i++)
	//	Common::log("mSortedVertexIDs",mSortedVertexIDs[i]);
	//Common::log("dTime", Common::timeEnd());
}


VertexCluster::~VertexCluster(void)
{

	delete []mWeights;

	delete []mEdgeLengths;
	delete []mAngles;

	delete  mHashTable;


	delete []mLODIB;

}