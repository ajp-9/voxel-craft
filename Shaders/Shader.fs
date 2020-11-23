#version 330 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;
in float passBrightness;

// texture sampler
uniform sampler2D texture1;

void main()
{
	vec4 textureFinal = texture(texture1, TexCoord);
	if(textureFinal.a == 0) { discard; }

	FragColor = textureFinal * passBrightness;
}