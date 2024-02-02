#version 330 core
#pragma optimize(off)
varying vec3 color;
varying vec2 TexCoord;
uniform int vertexInteger;
uniform sampler2D ourTexture;
void main()
{
	if (vertexInteger == 0) {
	 	gl_FragColor = vec4(color, 1.0);
	} else {
		gl_FragColor = texture(ourTexture, vec2(TexCoord.x,1.0 -TexCoord.y));
	}
}