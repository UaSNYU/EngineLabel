#include <Ciallo.h>

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Platform/OpneGL/OpenGLShader.h"

class ExampleLayer : public Ciallo::Layer
{
public:
	ExampleLayer()
		: Layer("Example"),m_CarmeraController(16.0f/9.0f,true)
	{
		float vertexData[] = {-0.5f, -0.5f, 0.0f ,0.0f,0.0f,
							   0.5f, -0.5f, 0.0f,1.0f,0.0f,
							   0.5f,  0.5f, 0.0f ,1.0f,1.0f,
		                      -0.5f,  0.5f, 0.0f,0.0f,1.0f
		                     };

		unsigned int indices[] = { 0,1,2,
		                           0,3,2 };



		m_SquareaVA.reset(Ciallo::VertexArray::Create());

		Ciallo::Ref<Ciallo::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Ciallo::VertexBuffer::create(vertexData, sizeof(vertexData)));
		Ciallo::Ref<Ciallo::IndexBuffer> indexBuffer;
		indexBuffer.reset(Ciallo::IndexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t)));

		Ciallo::BufferLayout layout = {
			{"a_position",Ciallo::ShaderDateType::Float3},
			{"textcoord",Ciallo::ShaderDateType::Float2}
		};

		vertexBuffer->SetLayout(layout);

		m_SquareaVA->AddVertexBuffer(vertexBuffer);
		m_SquareaVA->AddIndexBuffer(indexBuffer);

		m_Shader=Ciallo::Shader::Create("res/Shaders/Basic.shader");
		m_TextureShader=Ciallo::Shader::Create("res/Shaders/TextureShader.shader");

		m_Texture = Ciallo::Texture2D::Create("res/Textures/checkerBox.png");
		m_Texture->Bind(0);

		std::dynamic_pointer_cast<Ciallo::OpenGLShader> (m_TextureShader)->Bind();
		std::dynamic_pointer_cast<Ciallo::OpenGLShader> (m_TextureShader)->UploadUniformInt("u_texture", 0);

	}

	void OnUpdate(Ciallo::TimeStep timestep)override
	{
		m_CarmeraController.OnUpdata(timestep);

		Ciallo::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
		Ciallo::RenderCommand::Clear();

		Ciallo::Renderer::BeginScene(m_CarmeraController.GetCamera());

		glm::mat4 scale= glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<Ciallo::OpenGLShader> (m_Shader)->Bind();
        std::dynamic_pointer_cast<Ciallo::OpenGLShader> (m_Shader)->UploadUniform3f("u_color", m_SquareColor);
		
		
		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 pos = {x*0.11, y*0.11,0.0f};
                glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos)*scale;

				Ciallo::Renderer::Submit(m_SquareaVA, m_Shader,transform);
			}
		}

		Ciallo::Renderer::Submit(m_SquareaVA,m_TextureShader,glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		Ciallo::Renderer::EndScene();
	}

	void OnImGuiRender()
	{
		ImGui::Begin("Settings");
		
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));

		ImGui::End();
	}

	void OnEvent(Ciallo::Event& e) override
	{
		m_CarmeraController.OnEvent(e);
	}

private:
	Ciallo::ShaderLibrary m_ShaderLibrary;
	Ciallo::Ref<Ciallo::Shader> m_Shader,m_TextureShader;
	Ciallo::Ref<Ciallo::VertexArray> m_SquareaVA;
	Ciallo::Ref<Ciallo::Texture2D> m_Texture;
	Ciallo::OrthographicCameraController m_CarmeraController;


	glm::vec3 m_SquarePosition;
	glm::vec3 m_SquareColor = {0.2f, 0.3f, 0.8f };
};

class Sandbox : public Ciallo::Application
{
public:
	Sandbox()
	{
		pushLayer(new ExampleLayer());
	}

	~Sandbox() {}

};

Ciallo::Application* Ciallo::CreateApplication()
{
	return new Sandbox();
}