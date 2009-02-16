shared texture tNormalizer;
shared texture tBump;
shared texture tDiffuse;


shared float4 vPosLight;
shared float4 vPosViewer;


shared float3 vScaleLight;


shared float4 vColorLight; 

shared float4x4 mViewProjection :  WORLDVIEWPROJECTION;


texture tAtenuacionXY
<
	string name = "squarelight1.tga";
>;

texture tAtenuacionZ
<
	string name = "squarelight1a.tga";
>;


struct VS_INPUT
{
    float4 vPosition        : POSITION;
    float3  dwTangent       : TANGENT;
    float3  dwBinormal      : BINORMAL;
    float3  dwNormal        : NORMAL;
    float2 vTexCoords       : TEXCOORD;    
};


struct PS_OUTPUT
{
	float4 vColor :COLOR0;
};

struct VS_OUTPUT_BLINN
{
    float4  vPosition				          : POSITION;    
    float2  vTexCoordBumpDiffuse      : TEXCOORD0;       
    float3  vLightTangent             : TEXCOORD1;   
    float3  vHalfTangent              : TEXCOORD2;   
    float2  vLightObjectXY            : TEXCOORD3;   
    float2  vLightObjectZ             : TEXCOORD4;   
};



VS_OUTPUT_BLINN transformBlinn(VS_INPUT v)
{
  VS_OUTPUT_BLINN o;
   
  
  o.vPosition = mul(v.vPosition,mViewProjection);
  
  
  float3 vTangent;
  float3 vNormal;
  float3 vBinormal;
  
  vTangent = (v.dwTangent)*2-1;
  vNormal = (v.dwNormal)*2-1;
  vBinormal = (v.dwBinormal)*2-1;
  
  
  float3x3 mTangent = float3x3(vTangent.x, vBinormal.x, vNormal.x,
                 vTangent.y, vBinormal.y, vNormal.y, 
                 vTangent.z, vBinormal.z, vNormal.z);

  
  
  float3 vAux2 = normalize(vPosLight-v.vPosition);
  
  float3 vAux;
  
  vAux = mul(vAux2,mTangent);
  
        
  
  o.vTexCoordBumpDiffuse = v.vTexCoords;
  
  
  o.vLightTangent = vAux;
  
  
    
  
  vAux2 = normalize((vPosLight-v.vPosition) + (vPosViewer-v.vPosition));
  
    
  vAux = mul(vAux2,mTangent);
  
            
  o.vHalfTangent = vAux;
  
  float3 vLightWorldScale = (vPosLight-v.vPosition) * vScaleLight;
  
  vLightWorldScale = vLightWorldScale*0.5+0.5;
  
  o.vLightObjectXY = vLightWorldScale.xy;
  o.vLightObjectZ = float2(vLightWorldScale.z,0.f);    
          
  
  return o;
}



pixelshader psBlinn = 
asm
{
  ps_1_4
  
  def c0, 4.0f, 1.0f, 0.0f, -0.75f
  
  texld r0,t0 ;Diffuse
  texld r1,t0 ;Normal
  texld r2,t1 ;light tangent
  texld r3,t2 ;half tangent
  texld r4,t3 ;xy
  texld r5,t4 ;z
    
  dp3_sat r2.w,r1_bx2,r3_bx2 ;N*H
  dp3_sat r1.rgb,r2_bx2,r1_bx2
  +mad_x4_sat r2.w,r2.w,r2.w,c0.w  
  mul_x2_sat r5.rgb,r4,r5
  +mul r2.w,r2.w,r2.w
  mul_sat r0.rgb,r0,r1
  +mul_x2_sat r2.w,r2.w,r1.w
  add_sat r0.rgb,r0,r2.w
  mul r0.rgb,r0,c1  
  mul r0.rgb,r0,r5  
};


technique Blinn_PS_1_4
<
string technique_Info = "ps 1.4, 1 pass";
>
{    
    pass P0
    {                    
        ALPHABLENDENABLE = True;        
        SrcBlend = ONE;
        DestBlend = ONE;         

        
        AddressU[0] = Wrap;
        AddressV[0] = Wrap;
        
        AddressU[1] = Wrap;
        AddressV[1] = Wrap;
        
        AddressU[2] = Clamp;
        AddressV[2] = Clamp;
        
        AddressU[3] = Clamp;
        AddressV[3] = Clamp;
        
        AddressU[4] = Clamp;
        AddressV[4] = Clamp;
        
        AddressU[5] = Clamp;
        AddressV[5] = Clamp;
        
        MipFilter[3] = NONE;
        MipFilter[4] = NONE;
        
        Texture[0]   = <tDiffuse>; 
        Texture[1]   = <tBump>; 
        Texture[2]   = <tNormalizer>; 
        Texture[3]   = <tNormalizer>; 
        
        Texture[4]   = <tAtenuacionXY>;  
        Texture[5]   = <tAtenuacionZ>;
        
        
        PixelShaderConstant1[1] = <vColorLight>;                
  
                                
        Vertexshader = compile vs_1_1 transformBlinn();         
        PixelShader = <psBlinn>;       
    }
}

