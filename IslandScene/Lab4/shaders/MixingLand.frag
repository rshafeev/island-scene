uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D tex3;
uniform sampler2D texVec;
uniform int EnableFog;

varying vec2 vertex_src;


varying vec2 tex_src1;
varying vec2 tex_src2;
varying vec2 tex_src3;
varying  vec4 Vpos;
void main()
{

vec4 smesh;
vec3 ColorVec;
ColorVec = vec3(texture2D(texVec,vec2(vertex_src)));   
       smesh = vec4(vec3(vec3(texture2D(tex1  ,tex_src1))*ColorVec.r+
                             vec3(texture2D(tex2  ,tex_src2))*ColorVec.g+
                             vec3(texture2D(tex3  ,tex_src3))*ColorVec.b),1.0);
float fog = clamp(gl_Fog.scale*(gl_Fog.end + Vpos.z), 0.0, 1.0);

smesh=gl_Color*smesh;
if(EnableFog==1)
	smesh = mix(gl_Fog.color,smesh,fog);


       if(smesh[0]>1.0) smesh[0]=1.0;
       if(smesh[1]>1.0) smesh[1]=1.0;
       if(smesh[2]>1.0) smesh[2]=1.0;


gl_FragColor = smesh;


}