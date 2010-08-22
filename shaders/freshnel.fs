/*
Built-in variables:
- vec4  gl_FragCoord
- bool  gl_FrontFacing
- vec4  gl_FragColor
- vec4  gl_FragData[gl_MaxDrawBuffers]
- float gl_FragDepth

Varying variables:
- vec4  gl_Color
- vec4  gl_SecondaryColor
- vec4  gl_TexCoord[]
- float gl_FogFragCoord
- vec2  gl_PointCoord
*/

varying float cameraDiff;

void main() {
	float intensity = cameraDiff; //clamp(cameraDiff, 0., 2.0);
	gl_FragColor = vec4(intensity/3. ,intensity/2. ,intensity, 1.);
}

