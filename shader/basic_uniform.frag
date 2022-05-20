#version 460
layout (location = 0) out vec4 FragColor;

layout(binding = 0) uniform sampler2D Tex1;

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

// Uniforms
uniform struct LightInfo
{
vec3 Position;
vec3 La;
vec3 L;
}Lights[3];

uniform struct MaterialInfo
{
vec3 Ka;
vec3 Kd;
vec3 Ks;
float Shininess;
}Material;



vec3 phongModel(int light, vec3 pos, vec3 n )
{
//calculate ambient here, to access each light La value use this:

    vec3 ambient = Lights[light].La * Material.Ka ;


//calculate diffuse here
    vec3 s = normalize(vec3(Lights[light].Position) - pos);  //find out s vector
    float sDotN = max(dot(s,n), 0.0); //calculate dot product between s & n
    vec3 diffuse = Material.Kd * sDotN;
//calculate specular here+
    vec3 spec = vec3(0.0);
    if( sDotN > 0.0 )
{
    vec3 v = normalize(-pos.xyz);
    vec3 r = reflect( -s, n );
    spec = Material.Ks * pow( max( dot(r,v), 0.0 ),
    Material.Shininess );
 }
 return ambient + Lights[light].L * (diffuse + spec);
}


vec3 blinnPhong(int light, vec3 pos, vec3 n){

    vec3 texColor = texture(Tex1, TexCoord).rgb;

   vec3 ambient = Lights[light].La * texColor;
    vec3 s = normalize(vec3(Lights[light].Position) - pos);

    float sDotN = max(dot(s,n), 0.0);

    vec3 diffuse = texColor * sDotN;
    vec3 spec = vec3(0.0);

    if( sDotN > 0.0)
    {
        vec3 v = normalize(-pos.xyz);
        vec3 h = normalize(v + s);
        spec = Material.Ks * pow(max(dot(h,n), 0.0), Material.Shininess);
    }

    return ambient + Lights[light].L * (diffuse + spec);

}

void main() 
{ 
    for(int i = 0; i < Lights.length; i++)
    {
    FragColor = vec4(blinnPhong(i, Position, normalize(Normal)), 1);}
}
