uniform sampler2D tex1;
uniform float randValue;
float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233+randValue))) * 43758.5453);
}

void main()
{
 float NoiseAmount = 0.03;
 vec3 Color;
 Color= vec3(texture2D(tex1,gl_TexCoord[0].xy));   
 float gray = dot(Color, float4(0.3, 0.59, 0.11, 0)); 
 Color = float4(gray * float3(0.9, 0.8, 0.6) , 1);
 float r = rand(gl_TexCoord[0]);
 if(r>0.97)
 {
   Color.r = 0.4;
   Color.g = 0.4;
   Color.b = 0.4;
 }
  int t = int(5000*gl_TexCoord[0].x);
  int k1 = randValue*randValue*rand(vec2(gl_TexCoord[0].x,randValue))+81;
 if((t/k1)*k1==t&&r>0.6)
 {
   Color.r = 0.6;
   Color.g = 0.6;
   Color.b = 0.6;
 }
 gl_FragColor = vec4(Color, 1.0);

}