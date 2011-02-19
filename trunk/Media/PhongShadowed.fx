 
//#define CAMERA_MODE 1
//#define FLIP_Y 1
#define UV_REPEAT 1.0f


float Script:STANDARDSGLOBAL <
string ScriptClass = "scene";
string ScriptOrder = "standard";
string ScriptOutput = "color";
 string Script="Technique=shadow;";
> =0.8;

 
 float depthBias=0.002f; 

#define  SHADOW_MAP_RATIO float2(2.0f,2.0f)
 
float2 vpSize:ViewPortPixelSize;

float4x4 VI:ViewInverse;
float4x4 WVP:WorldViewProjection;
float4x4 VP:ViewProjection;
float4x4 WIT:WorldInverseTranspose;
float4x4 W:World;


float4x4 LVP:ViewProjection
<
  string Object="SpotLight";
>;
  

SamplerState SamplerLinear
{
	Filter=MIN_MAG_MIP_LINEAR;
	AddressU=Wrap;
	AddressV=Wrap;
};

 
SamplerComparisonState SamplerCmp
{
	Filter=COMPARISON_MIN_MAG_MIP_LINEAR;
	AddressU=Clamp;
	AddressV=Clamp;
	ComparisonFunc=Less_equal;
 };
 
 
float4 lightPos:POSITION
<
  string Object="SpotLight";
>;
 
float4 lightColor:SPECULAR
<
 string Object="SpotLight";
>;

float4 ambientColor:AMBIENT={0.01,0.01,0.01,1.0f};

 
float specularE
<
 string UIWidget="slider";
  float UIMin=0.0;
  float UIMax=10.0;
  float UIStep=0.05;
  string UIName="specularE"; 
> = 55.0f;

float specularK
<
   string UIWidget="slider";
  float UIMin=0.0;
  float UIMax=1.0;
  float UIStep=0.05;
  string UIName="specularK"; 
> = 0.4f;


float4 clearColor={0.0f,0.0f,0.0f,0.0f};
float clearDepth=1.0f;



Texture2D colorTarget:RenderColorTarget
<
    float2 ViewPortRatio=SHADOW_MAP_RATIO;
   string Format="X8B8G8R8";
>;

Texture2D depthTarget:RenderDepthStencilTarget
<
    float2 ViewPortRatio=SHADOW_MAP_RATIO;
   string Format="D24S8_SHADOWMAP";
>;


Texture2D diffuseTex:Diffuse
<
 string ResourceName="default_color.dds";
 string ResourceType="2D";
>;
 

struct VS_IN
{
	float3 pos:POSITION;
	float3 nor:NORMAL;
	float2 uv:TEXCOORD0;
};

 
struct PS_IN
{
	float4 pos:SV_POSITION;
	float4 nor:NORMAL;
	float2 uv:TEXCOORD0;
	float4 worldPos:TEXCOORD1;
};
  

float4 genShadowVS(float3 Pos:POSITION):SV_POSITION
{
    float4 pos=float4(Pos,1.0f);
	pos=mul(pos,W);
	pos=mul(pos,LVP);
	return pos; 
}

float4 genShadowPS(float4 pos:SV_POSITION):SV_TARGET
{
	   return float4(pos.z,0,0,1.0f);
}


 
PS_IN useShadowVS(VS_IN vsin)
{
	PS_IN psin;
	psin.pos=mul(float4(vsin.pos,1.0f),WVP);
	psin.nor=mul(float4(vsin.nor,1.0f),WIT);
	psin.uv=vsin.uv*UV_REPEAT;
	psin.worldPos=mul(float4(vsin.pos,1.0f),W);
	return psin;
}
  
  
  
  
  void phong_shading(
		    float3 LightColor,
		    float3 Nn,
		    float3 Ln,
		    float3 Vn,
		    out float3 DiffuseContrib,
		    out float3 SpecularContrib)
{
    float3 Hn = normalize(Vn + Ln);
    float4 litV = lit(dot(Ln,Nn),dot(Hn,Nn),specularE);
    DiffuseContrib = litV.y * LightColor;
    SpecularContrib = litV.y * litV.z * specularK * LightColor;
}



float4 useShadowPS(PS_IN psin):SV_TARGET
{
 
	
	#ifdef FLIP_Y
	psin.uv.y=1.0f-psin.uv.y;
	#endif
	
	float4 lightProj=psin.worldPos;
	
	lightProj=mul(lightProj,LVP);
	
	 
    
	float2 lightUV;
	float depth;
	
	 
	
	lightProj.xyz/=lightProj.w;
	 lightProj.z-=depthBias;
	lightUV=lightProj.xy;
	lightUV.y=-lightUV.y;
	lightUV=lightUV*0.5+0.5;
	depth=lightProj.z; 
	 
 
	
	float shadow=depthTarget.SampleCmpLevelZero(SamplerCmp,lightUV,depth);
	
    float3 diffContrib;
    float3 specContrib;
	
    float3 Ln = normalize(lightPos-psin.worldPos);
    float3 Vn = normalize(VI[3].xyz-psin.worldPos);
    float3 Nn = normalize(psin.nor);
     
    
	phong_shading(lightColor,Nn,Ln,Vn,diffContrib,specContrib);
    float3 diffuseColor = diffuseTex.Sample(SamplerLinear,psin.uv).rgb;
    float3 result = specContrib+(diffuseColor*(diffContrib+ambientColor));
 

	 
      result=result*shadow;
     
    return float4(result,1);
	 
}

 
RasterizerState backCull
{
	CullMode=Back;
	FillMode=Solid;
};

 
RasterizerState frontCull
{
	CullMode=Front;
	FillMode=Solid;
};

DepthStencilState depthEnable
{
	DepthEnable=true;
	 DepthFunc=Less;
	 DepthWriteMask=All;
};

BlendState DisableBlend { BlendEnable[0] = FALSE; };

RasterizerState DisableCulling { CullMode = NONE; };
 



  
 
 
 
 
 
technique10 shadow
 <
 string Script=  
   "Pass=p0;"    
   "Pass=p1;";
>   
{
	 
	pass p0
 	<
	string Script=
	"RenderColorTarget=colorTarget;"
	"RenderDepthStencilTarget=depthTarget;"
	"RenderPort=SpotLight;"
	"ClearSetColor=clearColor;"
	"ClearSetDepth=clearDepth;"
	"Clear=Color;"
	"Clear=Depth;"
	"Draw=geometry;";
	> 
	{
	 SetVertexShader(CompileShader(vs_4_0,genShadowVS()));
	 SetGeometryShader(NULL);   
	 SetPixelShader(CompileShader(ps_4_0,genShadowPS()));
     SetDepthStencilState(depthEnable,0);
	 SetRasterizerState(DisableCulling);
     SetBlendState(DisableBlend, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF);
	}
 
	pass p1
	<
	string Script=
	"RenderColorTarget=;"
	"RenderDepthStencilTarget=;"
	"RenderPort=;"
	"ClearSetColor=clearColor;"
	"ClearSetDepth=clearDepth;"
	"Clear=Color;"
	"Clear=Depth;"
	"Draw=geometry;";
	>
	{
	 SetVertexShader(CompileShader(vs_4_0,useShadowVS()));
	 SetGeometryShader(NULL);   
	 SetPixelShader(CompileShader(ps_4_0,useShadowPS()));
	  SetDepthStencilState(depthEnable,0);
	  SetRasterizerState(DisableCulling);
	  SetBlendState(DisableBlend, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF);
	}  
 
	 
	
};
  
 
 