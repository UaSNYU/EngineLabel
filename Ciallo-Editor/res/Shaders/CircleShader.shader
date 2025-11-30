#shader vertex
#version 450 core

layout(location = 0) in vec4 Worldposition;
layout(location = 1) in vec3 Localposition;
layout(location = 2) in vec4 color;
layout(location = 3) in float thickness;
layout(location = 4) in float fade;
layout(location = 5) in int Entity_Id;
layout(std140,binding=0) uniform Matrices
{
	mat4 u_viewProjection;
};

struct VertexOutput
{
	vec3 v_LocalPostion;
	vec4 v_Color;
	float v_Thickness;
	float v_Fade;
};

layout(location = 0) out VertexOutput Output;
layout(location = 4) out flat int v_EntityId;

void main()
{
	Output.v_Color=color;
	Output.v_LocalPostion=Localposition;
	Output.v_Thickness=thickness;
	Output.v_Fade=fade;
	
	v_EntityId=Entity_Id;	
	gl_Position=u_viewProjection*Worldposition;
};



#shader fragment
#version 450 core

layout(location = 0) out vec4 o_Color;
layout(location = 1) out int  o_EntityID;

struct VertexOutput
{
	vec3 v_LocalPostion;
	vec4 v_Color;
	float v_Thickness;
	float v_Fade;
};

layout(location=0) in VertexOutput Input;
layout(location=4) in flat int v_EntityId;

uniform sampler2D u_texture[32];

void main()
{
	float distance=1.0-length(Input.v_LocalPostion);

	float color= smoothstep(0.0,Input.v_Fade,distance);
	color*=smoothstep(Input.v_Thickness+Input.v_Fade,Input.v_Thickness,distance);

	if(color==0.0)
	{
		discard;
	}

	o_Color=Input.v_Color;
	o_Color.a*=color;

	o_EntityID=v_EntityId;
};
