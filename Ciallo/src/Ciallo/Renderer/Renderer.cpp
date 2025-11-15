#include "clpch.h"
#include "Renderer.h"
#include "Renderer2D.h"
#include "RenderCommand.h"
#include "Shader.h"
#include "Platform/OpneGL/OpenGLShader.h"

namespace Ciallo
{
	Renderer::SceneData* Renderer::m_Scenedata=new Renderer::SceneData;

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewPort(0,0,width,height);
	}

	void Renderer::Init()
	{
		HZ_PROFILE_FUNCTION();

		RenderCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::BeginScene(const OrthographicCamera& camera)
	{
		m_Scenedata->viewProjection = camera.GetViewProjectionMatrix();
	}
	void Renderer::Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::mat4& transform)
	{
		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniform4fv("u_viewProjection", m_Scenedata->viewProjection);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniform4fv("u_transform", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
	void Renderer::EndScene()
	{
	}
}