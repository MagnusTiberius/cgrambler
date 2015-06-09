# About the official page: #
http://code.google.com/p/cgrambler/ is the official page of CGRambler2.0, you can download the newest source code and screenshots of this project from that site. Please comment this project at the bottom of this page, so that I can reply.


# Brief Introduction to CGRambler2.0: #
CGRambler2.0 is a DirectX 10 based rendering engine under developing since 2011/1/18, for the previous version of CGRambler1.0, please see http://user.qzone.qq.com/499482794/blog/1285834895
Compare with CGrambler1.0, CGRambler2.0 have been designed from the very beginning. It will focus on the architecture of the engine itself, not the gorgeous shaders. In addition, this project is completely open source, you can use it for commercial or non-commercial without permission.


# Configuration: #
You need windows 7, with the newest DirectX 10 SDK installed. Compile the DXUT first, then add the include and lib directories of DirectX SDK and DXUT to visual studio C++. Copy the fxc.exe in Utilities to Windows/System32/. If you compile in Debug mode, visual memory leak in needed, you can also remove it.


# About the author: #
South China Normal University, Grade 2008, HaiQuan Li, cgrambler@gmail.com



---





# Class Diagram #
![http://img460.ph.126.net/7HzgnyNovPwKVbtjlE6UjA==/1026257765088298215.jpg](http://img460.ph.126.net/7HzgnyNovPwKVbtjlE6UjA==/1026257765088298215.jpg)



---



# Module 3  Geometry Clipmaps #
Finished Time 2011/3/2

## Algorithm : ##
(1) To load huge height map and texture, you can just load a small area around the camera. In the paper, this is call Stack. As the picture show below.

![http://http.developer.nvidia.com/GPUGems2/elementLinks/02_clipmaps_01.jpg](http://http.developer.nvidia.com/GPUGems2/elementLinks/02_clipmaps_01.jpg)

> (2) As the camera move, update the Stack as below.

![http://img161.ph.126.net/voC9hBMyFvoVeW-41Rbrgw==/2151031772024365500.jpg](http://img161.ph.126.net/voC9hBMyFvoVeW-41Rbrgw==/2151031772024365500.jpg)

> (3) Build different LOD of grids, the one which nearest to the camera should be in most detail. Synchronize the center of the grids to the center of the Stack.

![http://http.developer.nvidia.com/GPUGems2/elementLinks/02_clipmaps_02.jpg](http://http.developer.nvidia.com/GPUGems2/elementLinks/02_clipmaps_02.jpg)

## Reference: ##
《Terrain Rendering Using GPU-Based Geometry Clipmaps》

http://http.developer.nvidia.com/GPUGems2/gpugems2_chapter02.html

## Quote: ##
The technique is easy to implement, and allows interactive flight over a 20-billion-sample grid of the United States stored in just 355 MB of memory, at around 90 frames per second.

## Test Map: ##
Mars

# Terrain\_Solid\_Down #

![http://img853.ph.126.net/W589dNInxd2aIb_10fWBog==/2737062673535435241.jpg](http://img853.ph.126.net/W589dNInxd2aIb_10fWBog==/2737062673535435241.jpg)

# Terrain\_Level\_Down #

![http://img699.ph.126.net/NoCZGXG19znGNIWo4NXluA==/2841489889895263699.jpg](http://img699.ph.126.net/NoCZGXG19znGNIWo4NXluA==/2841489889895263699.jpg)

# Terrain\_WireFrame\_Down #

![http://img115.ph.126.net/oB35IvB2XGFdNOn0vlLLFA==/685391568292393833.jpg](http://img115.ph.126.net/oB35IvB2XGFdNOn0vlLLFA==/685391568292393833.jpg)

# Terrain\_Solid\_Up #

![http://img153.ph.126.net/Thj4XqT2k1NrCR1QUufr3A==/1646628613759221087.jpg](http://img153.ph.126.net/Thj4XqT2k1NrCR1QUufr3A==/1646628613759221087.jpg)

# Terrain\_Level\_Up #

![http://img617.ph.126.net/fd44zG2daBnH0-bRE0nb8A==/1913185416704390199.jpg](http://img617.ph.126.net/fd44zG2daBnH0-bRE0nb8A==/1913185416704390199.jpg)

# Terrain\_WireFrame\_Up #

![http://img116.ph.126.net/tPxcVso6-l9AFuFLffwAvg==/713539065963616603.jpg](http://img116.ph.126.net/tPxcVso6-l9AFuFLffwAvg==/713539065963616603.jpg)





---

# Module 2  Mesh Lod #
Finished Time 2011/2/19

## Algorithm: ##
(1)Traverse all triangles, calculate weight of all vertex. Weight factor 1, cos(a), a is the maximum angle of incident edges on the vertex. Weight factor 2, the length of the longest among all the edge upon the vertex.

(2)Sort all vertices in the vertex-list in non-increasing order on their weights.

(3)The vertex with the highest weight will be the center of a new clustering-cell, and all vertices till within the cell are removed from the vertex-list and replaced by a unique representative vertex.
repeat (2) for the next highest weighted vertex in the vertex-list.


## Reference： ##
《Multi-resolution 3D approximations for rendering complex scenes》

http://www.cs.uu.nl/docs/vakken/ddm/slides/papers/rossignac.pdf

《 Model simplification using vertex-clustering》

http://portal.acm.org/citation.cfm?id=253284.253310


## Test Data： ##
|FileName：goof\_knight.sdkmesh  |FileSize：183kb 	    |TrianglesNum：6043    |VerticesNum：3455|
|:--------------------------------|:---------------------|:----------------------|:-----------------|

|FileName：Warrior.sdkmesh	  |FileSize：2.57 MB    |TrianglesNum：11160   |VerticesNum：33480|
|:----------------------------|:---------------------|:----------------------|:------------------|

|FileName：exoticcar.sdkmesh     |FileSize：4.16 MB    | TrianglesNum：40112    | VerticesNum：120336|
|:--------------------------------|:---------------------|:------------------------|:--------------------|

|FileName：dragon.sdkmesh	  | FileSize：23.4 MB   |TrianglesNum：871318   | VerticesNum：441491|
|:---------------------------|:---------------------|:-----------------------|:--------------------|



# LOD\_Solid\_Knight #
![http://img839.ph.126.net/5mWm6F363S6_UOJhnlznZg==/779685685490634657.jpg](http://img839.ph.126.net/5mWm6F363S6_UOJhnlznZg==/779685685490634657.jpg)


# LOD\_WireFrame\_Knight #
![http://img165.ph.126.net/ZTvafkJsc5AJ1ZE2COF9fg==/2257147838244296753.jpg](http://img165.ph.126.net/ZTvafkJsc5AJ1ZE2COF9fg==/2257147838244296753.jpg)


# LOD\_Solid\_Knight\_Auto #
![http://img234.ph.126.net/rK5whKnJqHAUlCMOLAEw9w==/2122602799376877866.jpg](http://img234.ph.126.net/rK5whKnJqHAUlCMOLAEw9w==/2122602799376877866.jpg)


# LOD\_Solid\_Warrior #
![http://img768.ph.126.net/596tItkeQBXNtvmJZECoVw==/2991234577505445476.jpg](http://img768.ph.126.net/596tItkeQBXNtvmJZECoVw==/2991234577505445476.jpg)


# LOD\_WireFrame\_Warrior #
![http://img839.ph.126.net/TDTyCt8g1cxKfUIZ3Utozg==/779685685490634710.jpg](http://img839.ph.126.net/TDTyCt8g1cxKfUIZ3Utozg==/779685685490634710.jpg)


# LOD\_Solid\_exoticcar #
![http://img533.ph.126.net/5qS2aF3t13yAv94d_OiYcA==/1278740819197697883.jpg](http://img533.ph.126.net/5qS2aF3t13yAv94d_OiYcA==/1278740819197697883.jpg)


# LOD\_WireFrame\_exoticcar #
![http://img843.ph.126.net/q6KIU2nRBA8NrAjj-yDDPw==/1875186294848358806.jpg](http://img843.ph.126.net/q6KIU2nRBA8NrAjj-yDDPw==/1875186294848358806.jpg)


# LOD0\_Solid\_dragon #
![http://img687.ph.126.net/dlzfCkSXzvq4SOBNNh-20w==/1111544683031658622.jpg](http://img687.ph.126.net/dlzfCkSXzvq4SOBNNh-20w==/1111544683031658622.jpg)


# LOD\_MAX\_Solid\_dragon #
![http://img165.ph.126.net/O5W-PJEZlM3hrSpEpnmC8Q==/2257147838244296707.jpg](http://img165.ph.126.net/O5W-PJEZlM3hrSpEpnmC8Q==/2257147838244296707.jpg)



---



# Module 1 FX Composer SAS Based Shader Loader #
Finished time 2011/2/14

FX Composer is one of the best Shader IDEs, it has powerful SAS. which SAS, developers can develop shaders without writing C++. The functions of this Module is to bind variables, update variables, load resources(e.g textures), control rendering process. With the help of this Module, loading shaders can be very easy.
For example, to build the scene as the picture Phong\_Shadowed\_Glow showed below, you just need to write those code.


void InitCGRambler()
{
> g\_pSceneManager=SceneManager::getSingleton();
> g\_pSceneManager->init( DXUTGetD3D10Device());

> g\_pCamera=g\_pSceneManager->getCamera();
> g\_pCamera->setPosition(D3DXVECTOR4(1.356086f,0.049764f,-9.334431f,1.0f));
> g\_pCamera->setLookAt(D3DXVECTOR4(1.13701f,0.069804,-8.358729,1.0f));

> LightPtr light=g\_pSceneManager->createLight("SpotLight",D3DXVECTOR4(10.0f,10.0f,-9.0f,0.0f),D3DXVECTOR4(0.0f,0.0f,0.0f,0.0f), D3DXVECTOR4(1.0f,1.0f,1.0f,1.0f),30.0f,2.0f);

> EffectPtr pEffect=g\_pSceneManager->createEffect("effectName","PhongShadowed.fx");
> EffectPtr pGlow=g\_pSceneManager->createEffect("glow","Glow.fx");

> MeshPtr pMesh=g\_pSceneManager->createMesh("meshName","goof\_knight.sdkmesh","");
> pMesh->setSize(D3DXVECTOR4(10.0f,10.0f,10.0f,10.0f));
> pMesh->setDirection(D3DXVECTOR4(-0.8f,0.0f,-1.0f,1.0f));
> pMesh->setPosition(D3DXVECTOR4(0.0f,0.05f,0.0f,1.0f));

> MeshPtr pFloor=g\_pSceneManager->createMesh("floor","seafloor.sdkmesh","");
> pFloor->setSize(D3DXVECTOR4(0.3f,0.3f,0.3f,0.3f));

> pEffect->addMesh(pMesh);
> pEffect->addMesh(pFloor);

> pGlow->addMesh(pMesh);
> pGlow->addMesh(pFloor);
}


# New\_Shaders\_InFXC #
![http://img764.ph.126.net/syLssQA-o3hi1rSzFXC59A==/1245526771945386828.jpg](http://img764.ph.126.net/syLssQA-o3hi1rSzFXC59A==/1245526771945386828.jpg)


# Gooch\_Bump\_Reflect\_InFXC #
![http://img692.ph.126.net/XKaRRwgl53WzRG6noWETrQ==/2883429661425156089.jpg](http://img692.ph.126.net/XKaRRwgl53WzRG6noWETrQ==/2883429661425156089.jpg)


# Phong\_Shadowed\_InFXC #
![http://img764.ph.126.net/V_VjH-Iz-GqmHLCHiuQHBQ==/1245526771945386842.jpg](http://img764.ph.126.net/V_VjH-Iz-GqmHLCHiuQHBQ==/1245526771945386842.jpg)


# Phong\_Shadowed\_Glow\_InFXC #
![http://img775.ph.126.net/a12-s48E-KhdgaL02sycEg==/4844184349190786611.jpg](http://img775.ph.126.net/a12-s48E-KhdgaL02sycEg==/4844184349190786611.jpg)


# Gooch\_Bump\_Reflect\_InCGRambler2.0 #
![http://img229.ph.126.net/fVMXpvs5uEmRrhVcKutHXA==/1461981029036716455.jpg](http://img229.ph.126.net/fVMXpvs5uEmRrhVcKutHXA==/1461981029036716455.jpg)


# Phong\_Shadowed\_InCGRambler2.0 #
![http://img685.ph.126.net/JqKSeq48rsIrRDaBCTxDbg==/2797298318551807002.jpg](http://img685.ph.126.net/JqKSeq48rsIrRDaBCTxDbg==/2797298318551807002.jpg)


# Phong\_Shadowed\_Glow\_InCGRambler2.0 #
![http://img849.ph.126.net/uYcWeInJjDr-8aLBsEyMkA==/2695967326935761404.jpg](http://img849.ph.126.net/uYcWeInJjDr-8aLBsEyMkA==/2695967326935761404.jpg)


# <font color='#FF0000'> Please comment this project at the bottom of this page, so that I can reply.</font> #