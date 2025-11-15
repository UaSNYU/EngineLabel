#include "CialloEditor2D.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Platform/OpneGL/OpenGLShader.h"

namespace Ciallo
{
    CialloEditor2D::CialloEditor2D() :Layer("CialloEditorLayer"), m_CarmeraController(16.0f / 9.0f, true)
    {
      
    }

    void CialloEditor2D::OnAttach()
    {

        m_CarmeraController.SetZoomLevel(5.0f);
        m_Texture = Texture2D::Create("res/Textures/checkerBox.png");
        m_GTexture = Texture2D::Create("assets/Gameasserts/tilemap_packed2.5D.png");
        m_BlockTexture = SubTexture2D::CreateFromCoords(m_GTexture, { 4,1 }, { 16,16 }, { 1,2 });


        FrameBufferSpecification spec = { 1280,720 };
        m_FrameBuffer = FrameBuffer::Create(spec);

        m_Scene = CreatRef<Scene>();
        m_Camera = m_Scene->CreateEntity("Default_Camera");
        m_Camera.AddComponent<CameraComponent>();

        m_SecondCamera = m_Scene->CreateEntity("Default_Camera");
        m_SecondCamera.AddComponent<CameraComponent>();
        m_SecondCamera.GetComponent<CameraComponent>().Primary = false;

        m_SquareEntity = m_Scene->CreateEntity("Square");
        m_SquareEntity.AddComponent<SpriteComponent>(glm::vec4{ 1.0f,1.0f, 0.0f, 1.0f });


    }

    void CialloEditor2D::OnDetach()
    {

       
    }

    void CialloEditor2D::OnUpdate(TimeStep timestep)
    {
        m_fps = 1.0f / timestep;
        Renderer2D::ResetStats();
        //Resize
        if (FrameBufferSpecification sepc = m_FrameBuffer->GetSpecification();
            m_ViewPortSize.x>0.0f&&m_ViewPortSize.y>0.0f&&
            (sepc.Width != m_ViewPortSize.x || sepc.Height != m_ViewPortSize.y))
        {
           
            m_CarmeraController.SetAspectRatio(m_ViewPortSize.x / m_ViewPortSize.y);
            m_FrameBuffer->Resize((uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);
            m_Scene->OnViewPortResize((uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);
        }
        
        //Updata
        {
            if(m_viewportfocus)m_CarmeraController.OnUpdata(timestep);

        }

        //Render
        m_FrameBuffer->Bind();
        {
            HZ_PROFILE_SCOPE("Renderer Prep");
            RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
            RenderCommand::Clear();

        }

        {

            HZ_PROFILE_SCOPE("Renderer Running");

            m_Scene->OnUpdate(timestep);

        }

        m_FrameBuffer->Unbind();
    }

    void CialloEditor2D::OnImGuiRender()
    {
        HZ_PROFILE_FUNCTION();

        static bool dockspace_open = true;
        static bool opt_fullscreen = true;
        static bool opt_padding = false;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->GetWorkPos());
            ImGui::SetNextWindowSize(viewport->GetWorkSize());
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }
        else
        {
            dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;


        if (!opt_padding)
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", &dockspace_open, window_flags);
        if (!opt_padding)
            ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // DockSpace
        ImGuiIO& io = ImGui::GetIO();
        io.WantCaptureKeyboard = true;
        io.WantCaptureMouse = true;
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Options"))
            {
                if (ImGui::MenuItem("Close"))
                {
                    Application::Get().Close();
                }

                ImGui::EndMenu();
            }


            ImGui::EndMenuBar();
        }

        auto& stats = Renderer2D::GetStatistics();
        ImGui::Begin("Setting");
        ImGui::SetWindowFontScale(1.1f);

        ImGui::Text("Renderer2D Stats:");
        ImGui::Text("Renderer2D Drawca lls : %d", stats.DrawCalls);
        ImGui::Text("Renderer2D Quads : %d", stats.QuadCount);
        ImGui::Text("Renderer FPS : %.1f", m_fps);

        //SceneSetting
        if (m_SquareEntity)
        {
            auto& squarecolor = m_SquareEntity.GetComponent<SpriteComponent>().Color;
            ImGui::ColorEdit4("Square_color", glm::value_ptr(squarecolor));

        }
        auto& m_CameraTransform = m_Camera.GetComponent<TransformComponent>().Transform[3];
        ImGui::DragFloat3("Camera Transform",glm::value_ptr(m_CameraTransform));


        ////////
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::Begin("ViewPort");
        m_viewportfocus = ImGui::IsWindowFocused();
        m_viewporthover = ImGui::IsWindowHovered();
        Application::Get().GetImGuiLayer()->SetBlockEvent(!m_viewportfocus||!m_viewporthover);
       
        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        m_ViewPortSize = { viewportPanelSize.x, viewportPanelSize.y };

        uint32_t textureID = m_FrameBuffer->GetColorAttachment();
        ImGui::Image((void*)textureID, ImVec2{ m_ViewPortSize.x ,m_ViewPortSize.y }, ImVec2{ 0 , 1 }, ImVec2{ 1, 0 });

        ImGui::End();
		ImGui::PopStyleVar();

        ImGui::End();

        ImGui::End();
    }

    void CialloEditor2D::OnEvent(Event& event)
    {
        HZ_PROFILE_FUNCTION();
        m_CarmeraController.OnEvent(event);
    }


}
    