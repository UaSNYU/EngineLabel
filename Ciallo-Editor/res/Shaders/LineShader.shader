#shader vertex
#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in int Entity_Id;
layout(std140,binding=0) uniform Matrices
{
	mat4 u_viewProjection;
};

struct VertexOutput
{
	vec4 v_Color;
};

layout(location = 0) out VertexOutput Output;
layout(location = 1) out flat int v_EntityId;

void main()
{
	Output.v_Color=color;	
	v_EntityId=Entity_Id;	
	gl_Position=u_viewProjection*vec4(position,1.0);
};



#shader fragment
#version 450 core

layout(location = 0) out vec4 o_Color;
layout(location = 1) out int  o_EntityID;

struct VertexOutput
{
	vec4 v_Color;
};

layout(location=0) in VertexOutput Input;
layout(location=1) in flat int v_EntityId;

void main()
{
	o_Color=Input.v_Color;

	o_EntityID=v_EntityId;
};
