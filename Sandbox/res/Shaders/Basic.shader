#shader vertex
#version 330 core

layout(location = 0) in vec4 position;

out vec3 v_position;

uniform mat4 u_viewProjection;
uniform mat4 u_transform;

void main()
{
	v_position=position.xyz;
	gl_Position=u_viewProjection*u_transform*position;
};



#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_position;

uniform vec3 u_color;

void main()
{
	color=vec4(u_color,1.0f);
};
