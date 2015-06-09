# 关于官方网站： #
http://code.google.com/p/cgrambler/ 是CGRambler2.0的官方网站，你可以在那下载到本项目的最新源码，高清截图。请统一在本页面最底端发表评论，以便本人回复。


# 项目简介： #
CGRambler2.0是继CGRambler1.0之后，于2011年1月18号开始开发的一款基于DirectX 10的图形渲染引擎，关于CGRambler1.0，请浏览http://user.qzone.qq.com/499482794/blog/1285834895
相比CGRambler1.0，CGRambler2.0经过重新架构（几乎是重写），将更加注重引擎构架本身，即“看不见的渲染艺术”，而不是华丽的Shader。另外，本项目采用开源方式，可自由用于商业或非商业用途。


# 项目配置: #
首先要有win7,然后要安装最新版的DirectX 10 SDK,先去编译SDK里面的DXUT,然后把SDK的include目录和lib目录（包括刚才的DXUT在内）增加到VC。再把SDK的Utilities里面的fxc(Shader编译器）复制到Windows/system32/下面。另外，如果在Debug模式下编译时，你还虽然增加Visual Leak Detector来检查内存泄漏。当然，这个也可以去掉。


# 关于作者： #
华南师范大学 08级 李海全 cgrambler@gmail.com



---



# Class Diagram #

![http://img460.ph.126.net/7HzgnyNovPwKVbtjlE6UjA==/1026257765088298215.jpg](http://img460.ph.126.net/7HzgnyNovPwKVbtjlE6UjA==/1026257765088298215.jpg)



---



# 模块3  无限地形 #
完成时间 2011/3/2

## 算法步骤 ##
(1)	要载入超巨型高度图和纹理图，可以以摄象机为中心，在各个mip中载入周围的一小片。在论文中，这些载入到内存的数据，叫Stack，如下图所示。

![http://http.developer.nvidia.com/GPUGems2/elementLinks/02_clipmaps_01.jpg](http://http.developer.nvidia.com/GPUGems2/elementLinks/02_clipmaps_01.jpg)

(2)	当摄象机移动时，以下面的方法更新Stack。

![http://img161.ph.126.net/voC9hBMyFvoVeW-41Rbrgw==/2151031772024365500.jpg](http://img161.ph.126.net/voC9hBMyFvoVeW-41Rbrgw==/2151031772024365500.jpg)

(3)	建立密度不同的网格，离摄象机越近的网格越密。网格的中心和Stack的中心同步。

![http://http.developer.nvidia.com/GPUGems2/elementLinks/02_clipmaps_02.jpg](http://http.developer.nvidia.com/GPUGems2/elementLinks/02_clipmaps_02.jpg)

## 参考文献： ##
《Terrain Rendering Using GPU-Based Geometry Clipmaps》

http://http.developer.nvidia.com/GPUGems2/gpugems2_chapter02.html

## 引用原文: ##
这个技术容易实现，并且可以仅仅消耗355MB的内存，在一个高度图有20亿采样点的美国地形上进行交互式的飞行模拟，帧率在90左右。

## 测试地图： ##
火星

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






# 模块2  Mesh Lod #
完成时间 2011/2/19

## 算法步骤: ##
(1)遍历所有三角形，计算出每个顶点的权重。权重因素1，cos(a)，其中a为以顶点为中心的所有对边的最大夹角。权重因素2，经过顶点的所有边的最大边长。

(2)根据权重对顶点进行非升排序。

(3)将空间平均分成分割成多个grid.

(4)建立grid的哈希表。

(5)遍历所有顶点，将坐标除以grid大小，进行小数截断，哈希到对应的grid中.

(6)根据其所在的grid和邻近的7个grid，查找是否已存在包含它的cell(cell的大小和grid一样). 若存在，则该顶点被该cell的中心顶点所取代。否则，以该顶点为中心，建立新的cell，并保存在包含它的grid中。

(7)遍历所有三角形，将没有退化的三角形的Index保存到一个新的IndicesBuffer, 过滤出没有退化的三角形的Vertex, 作为一个LOD。

(8)设定grid的不同大小，重复(3)(4)(5)(6)(7)建立多个LOD。

(9)设D为Mesh到Camera的距离，FOV为视角，windowsHeight为窗口高度，根据R=(2Dtan(FOV/2))/widowsHeight, 计算出屏幕上一个像素对应在世界空间的大小R。

(10)找到grid大小刚才小于或等于R的LOD进行渲染。

## 参考文献： ##
《Multi-resolution 3D approximations for rendering complex scenes》

http://www.cs.uu.nl/docs/vakken/ddm/slides/papers/rossignac.pdf

《 Model simplification using vertex-clustering》

http://portal.acm.org/citation.cfm?id=253284.253310

## 测试数据： ##
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



# 模块1  基于FX Composer SAS的Shader载入 #
完成时间 2011/2/14

FX Composer是目前两大Shader的IDE之一，它有很强大的SAS，从而使开发者在编写Shaders时用不着写C++。本模块的作用就是根据FX Composer的SAS，全自动地进行Shader变量的绑定，更新，资源（比如纹理）的载入，控制渲染流程。有了本模块，Shader的载入将变得极奇方便。
比如，建立下面图片中，Phong\_Shadowed\_Glow的场景，你只需要编写如下代码


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


# <font color='#FF0000'> 请统一在本页面最底端发表评论，以便本人回复。 </font> #