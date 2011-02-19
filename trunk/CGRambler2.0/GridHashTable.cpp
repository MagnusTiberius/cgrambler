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
#include "GridHashTable.h"



int GridHashTable::mDi[2][2]=
{
	{0,-1},
	{0,1},
};


int GridHashTable::mHashPrime[]=
{
	3,
	17,
	101,
};

int GridHashTable::mTableSizes[]={

	5003,
	17011,
	53003,
	107021,
	233021,
	472019,
	600169,
	1111189,
	2222177,
	4444147,
	9374153,
	20123119,
	30123139,
	50123011,
	70123117,
	100123171,
	140123143,
	200123111,
	400123123,
	800123119,

};



int GridHashTable::getGridIndex(int hashIndices[3])
{
	int i,res;
	res=0;
	for(i=0;i<3;i++)
		res+=hashIndices[i]*mHashPrime[i];
	if(res<0)
		res+=mTableSize;
	res%=mTableSize;
	return res;
}


BYTE* GridHashTable::getCellsVB()
{
	return mVertices;
}


void GridHashTable::createCell(int id,const D3DXVECTOR3Ptr vertex,int hashIndices[3],float weight)
{
	int grid=getGridIndex(hashIndices);
	memcpy(locateVertex(mCurID),vertex,mStride); //����vertex block���ÿ���ȥ��vertexֻ�����block�Ŀ��˶���

	//Common::log("Cell Created",*locateVertex(mCurID));

	GridHashTableCellPtr cell=&mCellsPool[mCurID];
	cell->mPos=locateVertex(mCurID);
	cell->mNext=mCells[grid];
	cell->mID=mCurID;
	cell->mWeight=weight;
	mCells[grid]=cell;

	mReplacedID[id]=mCurID++;
}


D3DXVECTOR3 *GridHashTable::locateVertex(int id)
{
	return (D3DXVECTOR3*)(mVertices+id*mStride);
}

GridHashTableCellPtr GridHashTable::searchCell(int id,const D3DXVECTOR3Ptr vertex,const int *lowIndex,const int *curIndex)
{
	int deubgCellNum=0;

	float dx,dy,dz,ds;
	int i,j,k,p;
	int grid;
	int nextIndex[3];
	float dis=FLT_MAX;
	const D3DXVECTOR3 *vp=vertex;
	const D3DXVECTOR3 *cp;
	GridHashTableCellPtr res=NULL;
	//�˸������п����а���vertex��cell
	for(i=0;i<2;i++)
		for(j=0;j<2;j++)
			for(k=0;k<2;k++)
			{
				nextIndex[0]=curIndex[0]+mDi[ lowIndex[0] ][i];
				nextIndex[1]=curIndex[1]+mDi[ lowIndex[1] ][j];
				nextIndex[2]=curIndex[2]+mDi[ lowIndex[2] ][k];

				grid=getGridIndex(nextIndex);
				GridHashTableCellPtr cell=mCells[grid];

				while(cell!=NULL)
				{ 
					cp=cell->mPos;

					dx=fabs(vp->x-cp->x); dy=fabs(vp->y-cp->y); dz=fabs(vp->z-cp->z);
					if( dx<mToleranceHalf && dy<mToleranceHalf && dz<mToleranceHalf )
					{
						ds=dx+dy+dz;
						if(dis>ds)
						{
							dis=ds;
							res=cell;
						}
					}
					cell=cell->mNext;
					deubgCellNum++;
				}

			}

			return res;
}


void GridHashTable::addVertex(int id, D3DXVECTOR3Ptr vertex,float weight)
{
	//	Common::log("V id",id);
	//	Common::log("addVertex",*vertex);


	int i,j,grid;
	int hashIndices[3];

	for(i=0;i<3;i++)
		hashIndices[i]=abs((*vertex)[i]*mScale);


	//	Common::log("hashIndices x",hashIndices[0]);
	//	Common::log("hashIndices y",hashIndices[1]);
	//	Common::log("hashIndices z",hashIndices[2]);


	int lowIndices[3];
	for(i=0;i<3;i++)
		lowIndices[i]=fmod((*vertex)[i],mTolerance)>(mTolerance*0.5f);


	///	Common::log("***lowIndices x",lowIndices[0]);
	//	Common::log("***lowIndices y",lowIndices[1]);
	//	Common::log("***lowIndices z",lowIndices[2]);


	GridHashTableCellPtr cell;

	if( (cell=searchCell(id,vertex,lowIndices,hashIndices))!=NULL )
	{

		mReplacedID[id]=cell->mID;

		return;
	}

	createCell(id,vertex,hashIndices,weight);

}


void GridHashTable::init(float tolerance)
{

	mCurID=0;

	mToleranceHalf=tolerance*0.5f;
	mTolerance=tolerance;
	mScale=1.0f/mTolerance;


	for(int i=0;i<mTableSize;i++)
		mCells[i]=NULL;


}


GridHashTable::GridHashTable(int verticesNum,UINT stride)
{
	mVerticesNum=verticesNum;
	mStride=stride;
	int i;
	for(i=0;i<20;i++)
		if(mTableSizes[i]>=mVerticesNum)
			break;
	if(i==20) i--;
	mTableSize=mTableSizes[i];

	mReplacedID=new int[mVerticesNum];
	mCells=new GridHashTableCellPtr[mTableSize];
	mVertices=(BYTE*)malloc(mStride*mVerticesNum); 
	mCellsPool=new GridHashTableCell[mVerticesNum];

}


UINT GridHashTable::getCellsNum()
{
	return mCurID;
}

INT GridHashTable::getReplacedID(int id)
{
	return mReplacedID[id];
}

GridHashTable::~GridHashTable(void)
{
	delete []mCells;
	delete []mReplacedID;
	delete []mVertices; 
	delete []mCellsPool;
}

