#include "clpch.h"
#include "RenderCommand.h"
#include "Platform/OpneGL/OpenGLRendererAPI.h"

namespace Ciallo
{
	RendererAPI* RenderCommand:: m_RenderAPI= new OpenGLRendererAPI();
}