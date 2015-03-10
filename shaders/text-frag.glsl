#version 120
 
uniform sampler2D sampler;
uniform vec4 color;

void main(void) {
	gl_FragColor.rgb = vec3(1,1,1);
	gl_FragColor.a = texture2D(sampler, gl_TexCoord[0].xy).r * color.a;
	gl_FragColor.rgb = color.rgb;
}