#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 texcoord;
layout(location = 3) in float textindex;
layout(location = 4) in float tilingFactor;

out vec4 v_Color;
out vec3 v_Position;
out vec2 v_Texcoord;
out float v_TextIndex;
out float v_TilingFactor;

uniform mat4 u_viewProjection;

void main()
{
	v_TilingFactor=tilingFactor;
	v_Texcoord=texcoord;
	v_Position=position.xyz;
	v_Color=color;
	v_TextIndex=textindex;
	gl_Position=u_viewProjection*position;
};



#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec4 v_Color;
in vec3 v_Position;
in vec2 v_Texcoord;
in float v_TextIndex;
in float v_TilingFactor;

uniform sampler2D u_texture[32];


void main()
{
	color=texture(u_texture[int(v_TextIndex)],v_Texcoord*v_TilingFactor)*v_Color;
};
