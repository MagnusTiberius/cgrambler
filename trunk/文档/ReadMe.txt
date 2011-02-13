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
/////////////////////////////////////////////////////////////////////////////
=ģ��һ ����FX Composer SAS��Shader���� = 
���ʱ�� 2011/2/14
FX Composer��Ŀǰ����Shader��IDE֮һ�����к�ǿ���SAS���Ӷ�ʹ�������ڱ�дShadersʱ�ò���дC++����ģ������þ��Ǹ���FX Composer��SAS��ȫ�Զ��ؽ���Shader�����İ󶨣����£���Դ���������������룬������Ⱦ���̡����˱�ģ�飬Shader�����뽫��ü��淽�㡣
���磬��������ͼƬ�У�Phong_Shadowed_Glow�ĳ�������ֻ��Ҫ��д���´���

=Module 1 FX Composer SAS Based Shader Loader=
Finished time 2011/2/14
FX Composer is one of the best Shader IDEs, it has powerful SAS. which SAS, developers can develope shaders without writing C++. The functions of this Module is to bind variables, update variables, load resources(e.g textures), control rendering process. With the help of this Module, loading shaders can be very easy.
For example, to build the scene as the picture Phong_Shadowed_Glow showed below, you just need to write those code.
 
void InitCGRambler()
{
	g_pSceneManager=SceneManager::getSingleton();
	g_pSceneManager->init( DXUTGetD3D10Device());

	g_pCamera=g_pSceneManager->getCamera();
	g_pCamera->setPosition(D3DXVECTOR4(1.356086f,0.049764f,-9.334431f,1.0f));
	g_pCamera->setLookAt(D3DXVECTOR4(1.13701f,0.069804,-8.358729,1.0f));

	LightPtr light=g_pSceneManager->createLight("SpotLight",D3DXVECTOR4(10.0f,10.0f,-9.0f,0.0f),D3DXVECTOR4(0.0f,0.0f,0.0f,0.0f), D3DXVECTOR4(1.0f,1.0f,1.0f,1.0f),30.0f,2.0f);

	EffectPtr pEffect=g_pSceneManager->createEffect("effectName","PhongShadowed.fx");
	EffectPtr pGlow=g_pSceneManager->createEffect("glow","Glow.fx");

	MeshPtr pMesh=g_pSceneManager->createMesh("meshName","goof_knight.sdkmesh","");
	pMesh->setSize(D3DXVECTOR4(10.0f,10.0f,10.0f,10.0f));
	pMesh->setDirection(D3DXVECTOR4(-0.8f,0.0f,-1.0f,1.0f));
	pMesh->setPosition(D3DXVECTOR4(0.0f,0.05f,0.0f,1.0f));

	MeshPtr pFloor=g_pSceneManager->createMesh("floor","seafloor.sdkmesh","");
	pFloor->setSize(D3DXVECTOR4(0.3f,0.3f,0.3f,0.3f));

	pEffect->addMesh(pMesh);
	pEffect->addMesh(pFloor);

	pGlow->addMesh(pMesh);
	pGlow->addMesh(pFloor);
}


 