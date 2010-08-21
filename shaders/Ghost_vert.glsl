varying vec3	ReflectDir;
varying float	LightIntensity;

uniform vec3	LightPos;

varying vec3    EyeDir;


void main()
{
	vec3 normal = normalize(gl_NormalMatrix * gl_Normal);
	vec4 pos	= gl_ModelViewMatrix * gl_Vertex;

    EyeDir.xyz	= normal.xyz;
	
    ReflectDir	= reflect( EyeDir, normal );
	LightIntensity = max( dot( normalize( LightPos - EyeDir ), normal ), 0.0 );
	
	gl_Position = ftransform();
}