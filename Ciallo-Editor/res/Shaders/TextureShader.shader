#shader vertex
#version 450 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 texcoord;
layout(location = 3) in float textindex;
layout(location = 4) in float tilingFactor;
layout(location = 5) in int Entity_Id;
layout(std140,binding=0) uniform Matrices
{
	mat4 u_viewProjection;
};

struct VertexOutput
{
	vec4 v_Color;
	vec2 v_Texcoord;
	float v_TilingFactor;
};

layout(location = 0) out VertexOutput Output;
layout(location = 4) out flat int v_EntityId;
layout(location = 5) out flat float v_TextIndex;

void main()
{
	Output.v_TilingFactor=tilingFactor;
	Output.v_Texcoord=texcoord;
	Output.v_Color=color;
	
	v_TextIndex=textindex;
	v_EntityId=Entity_Id;
	
	gl_Position=u_viewProjection*position;
};



#shader fragment
#version 450 core

layout(location = 0) out vec4 color;
layout(location = 1) out int  color2;

struct VertexOutput
{
	vec4 v_Color;
	vec2 v_Texcoord;
	float v_TilingFactor;
};

layout(location=0) in VertexOutput Input;
layout(location=4) in flat int v_EntityId;
layout(location = 5) in flat float v_TextIndex;

uniform sampler2D u_texture[32];

void main()
{
	color=texture(u_texture[int(v_TextIndex)],Input.v_Texcoord*Input.v_TilingFactor)*Input.v_Color;
	color2=v_EntityId;
};
