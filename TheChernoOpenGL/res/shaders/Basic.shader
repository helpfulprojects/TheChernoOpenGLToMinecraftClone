#shader vertex
#version 330 core

layout(location=0) in vec4 position;
layout(location=1) in vec2 texCoords;
layout(location=2) in vec3 color;

out vec2 v_TexCoords;
out vec3 v_Color;
uniform mat4 u_MVP;

void main()
{
   gl_Position = u_MVP*position;
   v_TexCoords = texCoords;
   v_Color = color;
};

#shader fragment
#version 330 core

in vec2 v_TexCoords;
in vec3 v_Color;

out vec4 color;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
	vec4 texColor = texture(u_Texture,v_TexCoords);
	color = texColor*vec4(v_Color,1.0);
};
