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

struct VS_OUTPUT_BLINN_NO_ATENUACION
{
    float4  vPosition		       : POSITION;    
    float2  vTexCoordBump      : TEXCOORD0;   
    float2  vTexCoordDiffuse   : TEXCOORD1;   
    float3  vLightTangent      : TEXCOORD2;   
    float3  vHalfTangent       : TEXCOORD3;   
};

struct VS_OUTPUT_BLINN_ATENUACION
{
    float4  vPosition		   : POSITION;        
    float2  vTexCoord0     : TEXCOORD0;       
    float2  vTexCoord1     : TEXCOORD1;       
};




VS_OUTPUT_BLINN_NO_ATENUACION transformBlinnNoAtenuacion(VS_INPUT v)
{
  VS_OUTPUT_BLINN_NO_ATENUACION o;
   

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
  
        
  
  o.vTexCoordBump = v.vTexCoords;
  o.vTexCoordDiffuse = v.vTexCoords;
  
  o.vLightTangent = vAux;
  
      
  
  vAux2 = normalize((vPosLight-v.vPosition) + (vPosViewer-v.vPosition));
  
  vAux = mul(vAux2,mTangent);  
  
            
  o.vHalfTangent = vAux;
          
  
  return o;
}

VS_OUTPUT_BLINN_ATENUACION transformBlinnAtenuacion(VS_INPUT v)
{
  VS_OUTPUT_BLINN_ATENUACION o;
   

  o.vPosition = mul(v.vPosition,mViewProjection);
  
  float3 vLightWorldScale = (vPosLight-v.vPosition) * vScaleLight;
  
  vLightWorldScale = vLightWorldScale*0.5+0.5;
  
  o.vTexCoord0 = vLightWorldScale.xy;
  o.vTexCoord1 = float2(vLightWorldScale.z,0.f);    
  
            
  
  return o;
}

pixelshader psBlinnNoatenuacion = 
asm
{
  ps_1_1
  
  def c0, 4.0f, 1.0f, 0.0f, -0.75f
  
  tex t0 ;Normal
  tex t1 ;diffuse
  tex t2 ;L
  tex t3;H
  
  dp3_sat r1.xyzw,t3_bx2,t0_bx2
  dp3_sat r0.rgb,t2_bx2,t0_bx2
  +mad_x4_sat r1.w,r1.w,r1.w,c0.w  
  mul r1.w,r1.w,r1.w
  mul_sat r0.rgb,r0,t1
  +mul_x2_sat r1.w,r1.w,t0.w
  add_sat r0.rgb,r0,r1.w
  mul r0.rgb,r0,c1
  +mov r0.a,t1.a
};






technique blinnPS_1_1
<
string technique_Info = "ps 1.1, 2 passes";
>
{
	pass P0
    {                            
        ALPHABLENDENABLE = False;
        ColorWriteEnable = 0x8;
                   
        Texture[0]   = <tAtenuacionXY>; 
        Texture[1]   = <tAtenuacionZ>; 
                          
                          
        AddressU[0] = Clamp;
        AddressV[0] = Clamp;
        
        AddressU[1] = Clamp;
        AddressV[1] = Clamp;
        // Assign texture color to be used        
        ColorOp[0]   = SelectArg1;        
        ColorArg1[0] = Texture;        

        AlphaOp[0]   = SelectArg1;
        AlphaArg1[0] = Texture;
        
        ColorOp[1]   = MODULATE2X; 
        ColorArg1[1] = Current;
        ColorArg2[1] = Texture;        
                                        
        
        AlphaOp[1]   = MODULATE2X; 
        AlphaArg1[1] = Current;
        AlphaArg2[1] = Texture;        
                
        AlphaOp[2]   = Disable;      
        ColorOp[2]   = Disable;      
               

        Vertexshader = compile vs_1_1 transformBlinnAtenuacion();                 
        PixelShader = null;       
    }
    
    
    pass P1
    {                    
        ALPHABLENDENABLE = True;
        SrcBlend = DestAlpha;        
        DestBlend = ONE;         
    
        
        ColorWriteEnable = 0xF;
        
        AddressU[0] = Wrap;
        AddressV[0] = Wrap;
        
        AddressU[1] = Wrap;
        AddressV[1] = Wrap;
        
        AddressU[2] = Clamp;
        AddressV[2] = Clamp;
        
        AddressU[3] = Clamp;
        AddressV[3] = Clamp;
        
        Texture[0]   = <tBump>; 
        Texture[1]   = <tDiffuse>; 
        Texture[2]   = <tNormalizer>; 
        Texture[3]   = <tNormalizer>; 
        
        PixelShaderConstant1[1] = <vColorLight>;                
  
                                
        Vertexshader = compile vs_1_1 transformBlinnNoAtenuacion();         
        PixelShader = <psBlinnNoatenuacion>;       
    }
}

