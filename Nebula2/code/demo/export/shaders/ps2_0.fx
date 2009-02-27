shared texture tNormalizer;
shared texture tBump;
shared texture tDiffuse;


shared float4 vPosLight;
shared float4 vPosViewer;

shared float3 vScaleLight;



shared float4 vColorLight; 

shared float4x4 mViewProjection :  WORLDVIEWPROJECTION;

struct VS_INPUT
{
    float4 vPosition        : POSITION;
    float3  dwTangent       : TANGENT;
    float3  dwBinormal      : BINORMAL;
    float3  dwNormal        : NORMAL;
    float2 vTexCoords       : TEXCOORD;    
};

texture tAtenuacionXY
<
	string name = "squarelight1.tga";
>;

texture tAtenuacionZ
<
	string name = "squarelight1a.tga";
>;




pixelshader psPrueba = 
asm
{
  ps_2_0
  
  def c31, 2.0f, 1.0f, 0.0f, 4.0f         //helper constant
  def c30, 2.0f, 1.5f, 0.0f, -0.75f       //helper constant

  
  dcl         t0.xy                       //texture coordinates
  dcl         t1.xyz                      //light vector tangent  
  dcl         t2.xyz                      //eye vector tangent
  dcl		      t3.xy					              //coords atenuacion xy
  dcl		      t4.xy					              //coords atenuacion z  
    
  dcl_2d      s0                          //diffuse texture (gloss in alpha)
  dcl_2d      s1                          //normal texture      
  dcl_2d      s2					                //attenuation xy
  dcl_2d      s3					                //attenuation z
  
  
  texld       r0, t0, s1				          //load normal    
  mad         r2.rgb, r0, c31.r, -c31.g   //bias normal to range -1,1  
  mov		      r0.rgb,r2
  
       
  nrm		      r1,t2   
  
  dp3_sat	    r1.w,r1,r0
    
  nrm		      r2,t1
    
  
  dp3_sat	    r2,r2,r0				            // dot(N,L)  
  mad_sat	    r1.w,r1.w,r1.w,c30.w		    // saturate(dot(N,H)^2-0.75)
  mul		      r1.w,r1.w,c31.w
  
  texld		    r3,t0,s0					          // diffuse
  
  mul_sat     r1.w,r1.w,r0.w			        // multiply by the gloss
  add_sat     r1.w,r1.w,r1.w
  
  
  texld		    r4,t3,s2					          // attenuation xy
  texld		    r5,t4,s3					          // attenuation z
  
  mul		      r4.w,r4.w,r5.w
  
  add_sat		  r4.w,r4.w,r4.w
    
  mad_sat		  r2.rgb,r2,r3,r1.w
    
  mul		      r2.rgb,r2,c1
  
  
  mul		      r3.rgb,r2,r4.w
    
        
  mov         oC0, r3                 //color output   
  
};




struct VS_OUTPUT_BLINN
{
    float4  vPosition       : POSITION;    
    float2  vTexCoord       : TEXCOORD0;   // texture coordinates
    float3  vLightVector    : TEXCOORD1;   // vector Light in tangent space
    float3  vHalfVector     : TEXCOORD2;   // half vector in world    
    float2  vAtenuacionXY   : TEXCOORD3;   // attenuation xy
    float2  vAtenuacionZ    : TEXCOORD4;   // attenuation z
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

  vAux = mul(vAux2, mTangent);  
        
  
  o.vTexCoord = v.vTexCoords;  
  o.vLightVector = vAux;
  
  
  float3 vLightWorldScale = (vPosLight-v.vPosition) * vScaleLight;
  
    
  vAux = vLightWorldScale * 0.5 + 0.5;     
  
  o.vAtenuacionXY = vAux;
  o.vAtenuacionZ = float2(vAux.z,0.f);    
  
  
  
  vAux2 = normalize((vPosLight-v.vPosition) + (vPosViewer-v.vPosition));
  
    
  vAux = mul(vAux2, mTangent);
  
            
  o.vHalfVector = vAux;
          
  
  return o;
}



technique blinn_ps20
<
string technique_Info = "ps2.0, 1 pass";
>
{
    pass P0
    {            
        Texture[0]   = <tDiffuse>;         
        Texture[1]   = <tBump>;         
        Texture[2]   = <tAtenuacionXY>;   
        Texture[3]   = <tAtenuacionZ>;
        
        AddressU[0] = Wrap;
        AddressV[0] = Wrap;
        AddressU[1] = Wrap;
        AddressV[1] = Wrap;
        
        AddressU[2] = Clamp;
        AddressV[2] = Clamp;
        AddressU[3] = Clamp;
        AddressV[3] = Clamp;
        
        
        
        ALPHABLENDENABLE = True;
        SrcBlend = One;
        DestBlend = One;
        
                
        PixelShaderConstant2[1] = <vColorLight>;                

        Vertexshader = compile vs_1_1 transformBlinn();         
        PixelShader = <psPrueba>;
    }
}

