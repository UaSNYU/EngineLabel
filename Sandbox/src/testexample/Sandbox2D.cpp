#include "Sandbox2D.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Platform/OpneGL/OpenGLShader.h"



Sandbox2D::Sandbox2D() :Layer("Sandbox2D"), m_CarmeraController(16.0f / 9.0f, true)
{
	m_ParticleSystem = Ciallo::CreatRef<ParticleSystem>();
}

void Sandbox2D::OnAttach()
{
	m_CarmeraController.SetZoomLevel(5.0f);
	m_Texture = Ciallo::Texture2D::Create("res/Textures/checkerBox.png");
	m_GTexture = Ciallo::Texture2D::Create("assets/Gameasserts/tilemap_packed2.5D.png");
	m_BlockTexture = Ciallo::SubTexture2D::CreateFromCoords(m_GTexture, { 4,1 }, { 16,16 }, {1,2});
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Ciallo::TimeStep timestep)
{
	
	m_fps = 1.0f / timestep;

	HZ_PROFILE_FUNCTION();
	Ciallo::Renderer2D::ResetStats();

	//Updata
	{
		HZ_PROFILE_SCOPE("CarmeraController::OnUpdata");
		m_CarmeraController.OnUpdata(timestep);
		m_ParticleSystem->OnUpdate(timestep);
	}
	//Render
	{
        HZ_PROFILE_SCOPE("Renderer Prep");
		Ciallo::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
		Ciallo::RenderCommand::Clear();

	}
	
	{
	
		HZ_PROFILE_SCOPE("Renderer Running");
		
		Ciallo::Renderer2D::BeginScene(m_CarmeraController.GetCamera());
		Ciallo::Renderer2D::DrawQuad({ 0.0f,0.0f,0.0f }, { 1.0f,2.0f }, m_BlockTexture, 1.0f);
		Ciallo::Renderer2D::EndScene();
	}
	

	Ciallo::Renderer2D::BeginScene(m_CarmeraController.GetCamera());
	m_ParticleSystem->CursorParticleEmit(m_CarmeraController);
	m_ParticleSystem->OnRenderer();
	Ciallo::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	HZ_PROFILE_FUNCTION();

	auto& stats = Ciallo::Renderer2D::GetStatistics();
	ImGui::Begin("Settings");
	ImGui::SetWindowFontScale(1.1f);

	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Renderer2D Drawcalls : %d", stats.DrawCalls);
	ImGui::Text("Renderer2D Quads : %d", stats.QuadCount);
	ImGui::Text("Renderer FPS : %.1f", m_fps);
	ImGui::End();
}

void Sandbox2D::OnEvent(Ciallo::Event& event)
{
	HZ_PROFILE_FUNCTION();
	m_CarmeraController.OnEvent(event);
}
