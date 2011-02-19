float Script:STANDARDSGLOBAL
<
 string ScriptClass="object";
 string ScriptOrder="standard";
 string ScriptOutput="color";
 string Script="Technique=t0;";
> =0.8;

 

float4x4 WVP:WORLDVIEWPROJECTION;

float4 wireFrameVS(float3 pos:POSITION):SV_POSITION
{
	return mul( float4(pos,1.0f) ,WVP);
}

float4 wireFramePS(float4 pos:SV_POSITION):SV_TARGET
{
	return float4(1,1,1,1);
}

RasterizerState wireFrameState
{
	CullMode=None;
	FillMode=WireFrame;
};


technique10 t0
<
 string Script=
 "RenderColorTarget=;"
 "RenderDepthTarget=;"
 "SetClearColor=clearColor;"
 "SetClearDepth=clearDepth;"
 "Clear=Color;"
 "Clear=Depth;"
 "Pass=p0;";
> 
{
	pass p0
	<
  string Script=
  "Draw=geometry;";
    > 
	{
	 SetVertexShader(CompileShader(vs_4_0,wireFrameVS()));
	 SetGeometryShader(NULL);
	 SetPixelShader(CompileShader(ps_4_0,wireFramePS()));
	 SetRasterizerState(wireFrameState);
	}
}