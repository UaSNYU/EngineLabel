#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 0) in vec2 texcoord;

out vec3 v_position;
out vec2 Texcoord;

uniform mat4 u_viewProjection;
uniform mat4 u_transform;

void main()
{
	Texcoord=texcoord;
	v_position=position.xyz;
	gl_Position=u_viewProjection*u_transform*position;
};



#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_position;
in vec2 Texcoord;

uniform vec3 u_color;
uniform sampler2D u_texture;

void main()
{
	color=texture(u_texture,Texcoord);
};
