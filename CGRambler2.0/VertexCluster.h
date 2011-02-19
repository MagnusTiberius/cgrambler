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
#pragma once
#include "GridHashTable.h"
#include "TextPrinter.h"
#include "Common.h"
#include <climits> 
#include <algorithm>

using namespace std;

#define MAX_MESH_LODS 3
#define MESH_LOD_STRENGTH 20.0f
#define  MESH_LOD_ANGLE_FACTOR    10.0f 
#define  MESH_LOD_EDGE_LENGTH_FACTOR  1.0f 

struct VertexWeight
{
	int mID;
	float mWeight;

	bool operator<(const VertexWeight& b)
	{
		return mWeight>b.mWeight;
	}
};

class VertexCluster
{

	VertexWeight *mWeights;

	bool mVertexValueCreated;
	BYTE *mVertices;
	DWORD *mIndices;
	UINT64 mVerticesNum;
	UINT64 mIndicesNum;

	float mAvgEdgeLength;
	float *mEdgeLengths;
	float *mAngles;

	float mLODLevel;
	GridHashTable *mHashTable;


	float mTolerance;
	DWORD *mLODIB;

	::ID3D10Device *mDevice;
	UINT mStride;

	::ID3D10BufferPtr mOriginalVB;
	::ID3D10BufferPtr mOriginalIB;
private:
	void calculateWeights();
	void quickSort(int begin,int end);
	void sortVertices();
	bool WeightCmp(const VertexWeight& a,const VertexWeight& b);

public:
	D3DXVECTOR3 *locateVertex(int id);
	VertexCluster( ID3D10Device *device,UINT verticesNum,BYTE *rawVB,::ID3D10BufferPtr VB,UINT stride,UINT indicesNum,DWORD *rawIB,::ID3D10BufferPtr IB);
	//level,0表示原始mesh
	//level,0 is original mesh
	void generateLOD(float level,ID3D10BufferPtr &vb,ID3D10BufferPtr &ib,UINT &ibLength); //mVertexValueCreated=true
	float getTolerance();
	float getGridSize(float level);

	~VertexCluster(void);
};


typedef VertexCluster* MeshLODGeneratorPtr;