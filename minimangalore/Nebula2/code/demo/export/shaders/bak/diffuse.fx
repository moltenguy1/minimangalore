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







struct PS_OUTPUT
{
	float4 vColor :COLOR0;
};



struct VS_OUTPUT_ATENUACION
{
    float4  vPosition       : POSITION;    
    float2  vTexCoordXY     : TEXCOORD0;   // attenuation XY
    float2  vTexCoordZ      : TEXCOORD1;   // attenuation Z
};



struct VS_INPUT
{
    float4 vPosition        : POSITION;
    float3  dwTangent        : TANGENT;
    float3  dwBinormal       : BINORMAL;
    float3  dwNormal         : NORMAL;
    float2 vTexCoords       : TEXCOORD;    
};

VS_OUTPUT_ATENUACION transformAtenuacion(VS_INPUT v)
{
  VS_OUTPUT_ATENUACION o;
   

  o.vPosition = mul(v.vPosition,mViewProjection);
  
  
  float3 vLightWorldScale = (vPosLight-v.vPosition) * vScaleLight;
  
  vLightWorldScale = vLightWorldScale*0.5+0.5;
  
  
  o.vTexCoordXY = vLightWorldScale.xy;
  o.vTexCoordZ = float2(vLightWorldScale.z,0.f);    
              
  return o;
}

struct VS_OUTPUT_NL
{
    float4  vPosition       : POSITION;    
    float3  vLightTangent   : TEXCOORD0;   // Light vector in tangent space    
    float2  vTexCoord       : TEXCOORD1;   // texture coordinates for normal map
};


VS_OUTPUT_NL transformNL(VS_INPUT v)
{
  VS_OUTPUT_NL o;
   

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

  vAux = mul(vAux2, mTangent);  

  o.vTexCoord = v.vTexCoords;  
  o.vLightTangent = vAux;
              
  return o;
}


struct VS_OUTPUT_TEXTURE
{
    float4  vPosition       : POSITION;        
    float2  vTexCoord       : TEXCOORD0;   // texture coordinates for diffuse texture
    float4  color0		      : COLOR0;	     // light color
};


VS_OUTPUT_TEXTURE transformTexture(VS_INPUT v)
{
  VS_OUTPUT_TEXTURE o;
   

  o.vPosition = mul(v.vPosition,mViewProjection);
  
  
  o.vTexCoord = v.vTexCoords;  
  o.color0 = vColorLight;
          
  
  return o;
}





technique diffuseLighting
<
string technique_Info = "NO PIXEL SHADERS, 3 passes";
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

                                
        Vertexshader = compile vs_1_1 transformAtenuacion();         
        PixelShader = NULL;
    }
    
    pass P1
    {            
        ALPHABLENDENABLE = True;
        ColorWriteEnable = 0x8;
        
        SrcBlend = DestAlpha;        
        DestBlend = ZERO;         

        Texture[0]   = <tNormalizer>;         
        Texture[1]   = <tBump>;         
                        
        AddressU[0] = Clamp;
        AddressV[0] = Clamp;
        
        AddressU[1] = Wrap;
        AddressV[1] = Wrap;
        
        ColorOp[0]   = SelectArg1;        
        ColorArg1[0] = Texture;        

        AlphaOp[0]   = SelectArg1;
        AlphaArg1[0] = Texture;
        
        ColorOp[1]   = DOTPRODUCT3; 
        ColorArg1[1] = Current;
        ColorArg2[1] = Texture;        
                                        
        
        AlphaOp[1]   = SelectArg1; 
        AlphaArg1[1] = Current;
        AlphaArg2[1] = Texture;        
                
        AlphaOp[2]   = Disable;      
        ColorOp[2]   = Disable;      

                                
        Vertexshader = compile vs_1_1 transformNL();         
        PixelShader = NULL;
    }
    
    pass P2
    {            
        ALPHABLENDENABLE = True;
        SrcBlend = DestAlpha;        
        DestBlend = ONE;                 
        ColorWriteEnable = 0xf;
		
		    Texture[0]   = <tDiffuse>;                 
		
        AddressU[0] = Wrap;
        AddressV[0] = Wrap;
        
        ColorOp[0]   = MODULATE;        
        ColorArg1[0] = Texture;        
        ColorArg2[1] = Diffuse;        

        AlphaOp[0]   = MODULATE;
        AlphaArg1[0] = Texture;
        AlphaArg2[1] = Diffuse;        
        
        AlphaOp[1]   = Disable;      
        ColorOp[1]   = Disable;      
                
        
        Vertexshader = compile vs_1_1 transformTexture();         
        PixelShader = NULL;
    }
}

