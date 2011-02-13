

float Script:STANDARDSGLOBAL <
string ScriptClass = "scene";
string ScriptOrder = "standard";
string ScriptOutput = "color";
 string Script="Technique=shadow;";
> =0.8;

SamplerState SamplerLinear
{
	Filter=MIN_MAG_MIP_LINEAR;
	AddressU=Wrap;
	AddressV=Wrap;
};

#define  SHADOW_MAP_RATIO float2(1.0f,1.0f)

float4 clearColor={0.0f,0.0f,0.0f,0.0f};
float clearDepth=1.0f;

float4x4 WVP:WorldViewProjection;

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


struct VS_IN
{
	float3 pos:POSITION;
	float2 uv:TEXCOORD0;
};


struct PS_IN
{
	float4 pos:SV_POSITION;
	float2 uv:TEXCOORD0;
};
  

float4 genShadowVS(float3 Pos:POSITION):SV_POSITION
{
    float4 pos=mul(float4(Pos,1.0f),WVP);
	return pos;
}

float4 genShadowPS(float4 pos:SV_POSITION):SV_TARGET
{
	 return float4(1,1,1,1);
}


 
PS_IN useShadowVS(VS_IN vsin)
{
	PS_IN psin;
	psin.pos=mul(float4(vsin.pos,1.0f),WVP);
	psin.uv=vsin.uv;
	return psin;
}
  
 

float4 useShadowPS(PS_IN psin):SV_TARGET
{ 
	//return float4(1,1,1,1);
	return colorTarget.Sample(SamplerLinear,psin.uv);
}
 
  
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
	  
	}  
 
	 
	
};
  
 
 