
float Scripts:STANDARDSGLOBAL
<
 string ScriptClass="object";
 string ScriptOrder="standard";
 string ScriptOuput="color";
 string Script="Technique=Quad?tech;";
> = 0.8;

float2 vp:VIEWPORTPIXELSIZE;

texture2D tex:DIFFUSE;

SamplerState SamplerPoint
{
	Filter=MIN_MAG_MIP_POINT;
	AddressU=Wrap;
	AddressV=Wrap;
};

struct PSIN
{
	 float4 pos:SV_POSITION;
	 float2 uv:TEXCOORD0;
};

PSIN quadVS(float3 pos:POSITION,float2 uv:TEXCOORD0)
{
  //  float2 duv=0.5f*1.0f/vp;
	PSIN psin;
	psin.pos=float4(pos,1.0f);
//	psin.uv=uv-duv;
    psin.uv=uv;
	return psin;
}

float4 quadPS(PSIN psin):SV_TARGET
{
	 return tex.Sample(SamplerPoint,psin.uv);
}


RasterizerState SolidState
{
	CullMode=None;
	FillMode=Solid;
};


technique10 tech
<
 string Script=
 "Pass=p0;";
>
{
	pass p0
	<
	 string Script=
	 "Draw=buffer;";
	>
	{
	  SetVertexShader(CompileShader(vs_4_0,quadVS()));
	  SetGeometryShader(NULL);
	  SetPixelShader(CompileShader(ps_4_0,quadPS()));
	  SetRasterizerState(SolidState);
	}
} 