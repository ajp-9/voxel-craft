#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in float aBrightness;

out vec2 TexCoord;
out float passBrightness;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float timeLight;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
	
	// when reading: light then bright
	
	int tempBrightness = int(aBrightness);
	
	int light = tempBrightness - 1000 * (tempBrightness / 1000);
	tempBrightness /= 1000;
	int brightness = tempBrightness - 1000 * (tempBrightness / 1000);

	if((float(brightness) / 100) * timeLight > float(light) / 100)
	{
		passBrightness = (float(brightness) / 100) * timeLight;
	}
	else
	{
		passBrightness = float(light) / 100;
	}
}