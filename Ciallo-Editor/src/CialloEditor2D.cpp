#include "CialloEditor2D.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/matrix_decompose.hpp"
#include "Platform/OpneGL/OpenGLShader.h"
#include "Ciallo/Core/Utils/platformUtils.h"
#include "ImGuizmo.h"
#include <regex>

namespace Ciallo
{
    CialloEditor2D::CialloEditor2D() :Layer("CialloEditorLayer")
    {
      
    }

    void CialloEditor2D::OnAttach()
    {
        m_Texture = Texture2D::Create("assets/Textures/checkerBox.png");
        m_GTexture = Texture2D::Create("assets/Textures/tilemap_packed2.5D.png");
        m_iconplay = Texture2D::Create("assets/Textures/play.png");
        m_iconedit = Texture2D::Create("assets/Textures/edit.png");

        m_BlockTexture = SubTexture2D::CreateFromCoords(m_GTexture, { 4,1 }, { 16,16 }, { 1,2 });

        FrameBufferSpecification spec;

        spec.Width = 1280;
        spec.Height = 720;
        spec.m_Attachements = {FrameBufferTextureFormat::RGBA8,FrameBufferTextureFormat::RED_INTEGER,FrameBufferTextureFormat::Depth};
        m_FrameBuffer = FrameBuffer::Create(spec);
        //TODO----多重采样帧缓冲不能直接用于着色器(光栅化)需要将颜色缓冲复制到无多重采样的帧缓冲中再进行渲染glblit...
        //OR 在着色器中自定义抗锯齿算法
        m_EditScene= CreatRef<Scene>();
        m_ActiveScene = m_EditScene;
        m_EditorCamera=EditorCamera(30.0f,1.778f,0.1f,1000.0f);
        m_Panel.SetContext(m_ActiveScene);
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
            m_EditorCamera.SetViewportSize(m_ViewPortSize.x, m_ViewPortSize.y);
            m_FrameBuffer->Resize((uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);
            m_ActiveScene->OnViewPortResize((uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);
        }
        
        //Updata
        {
            if (m_viewportfocus)
            {
                //Forget?
            }
            m_EditorCamera.OnUpdate(timestep);
        }

        //Render
        m_FrameBuffer->Bind();

        {
            HZ_PROFILE_SCOPE("Renderer Prep");
            RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
            RenderCommand::Clear();
            m_FrameBuffer->ClearAttachment(1, -1);
        }

        {

            HZ_PROFILE_SCOPE("Renderer Running");

            switch (m_SceneState)
            {
            case Ciallo::CialloEditor2D::SceneState::Edit:
                m_ActiveScene->OnUpdateRuntime(timestep);
                break;
            case Ciallo::CialloEditor2D::SceneState::Play:
                m_ActiveScene->OnUpdateEditor(timestep, m_EditorCamera);
                break;
            }

        }

        //PixelDataRead
        auto [mx, my] = ImGui::GetMousePos();
        mx -= m_ViewPortBounds[0].x;
        my -= m_ViewPortBounds[0].y;
        glm::vec2 ViewPortSize = m_ViewPortBounds[1] - m_ViewPortBounds[0];
        my = ViewPortSize.y - my;

        int mouseX = (int)mx;
        int mouseY = (int)my;
        if (mouseX >= 0 && mouseY >= 0 && mouseX < ViewPortSize.x && mouseY < ViewPortSize.y)
        {
            int PixelData = m_FrameBuffer->ReadPixel(1, mouseX, mouseY);
            if (PixelData == -1)
            {
                HoverdEntity = Entity();
            }
            else
            {
                HoverdEntity = Entity((entt::entity)PixelData, m_ActiveScene.get());

            }
        }

        OnOverlayRender();

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

        ImGuiStyle& style = ImGui::GetStyle();
        float minwinsizeX=style.WindowMinSize.x;
        style.WindowMinSize.x = 370.0f;

        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        style.WindowMinSize.x = minwinsizeX;

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Files"))
            {
                if (ImGui::MenuItem("New","Ctrl+N"))
                {
                    NewScene();
                }

                if (ImGui::MenuItem("Open...","Ctrl+O"))
                {
                    OpenScene();
                }

                if (ImGui::MenuItem("Save...", "Ctrl+S"))
                {
                    SaveScene();

                }

                if (ImGui::MenuItem("Save as...","Ctrl+Shift+S"))
                {
                    SaveSceneAs();

                }

                if (ImGui::MenuItem("Exit"))
                {
                    Application::Get().Close();
                }
               
                ImGui::EndMenu();
            }


            ImGui::EndMenuBar();
        }

        //PanelRender
        m_Panel.OnImGuiRender();
        m_ContentPanel.OnImGuiRender();

        //Stats
        auto& stats = Renderer2D::GetStatistics();
        ImGui::Begin("Setting");
        ImGui::SetWindowFontScale(1.1f);
        ImGui::Text("Renderer2D Stats:");
        ImGui::Text("Renderer2D Drawca lls : %d", stats.DrawCalls);
        ImGui::Text("Renderer2D Quads : %d", stats.QuadCount);
        ImGui::Text("Renderer FPS : %.1f", m_fps);

        std::string name = "None";
        if (HoverdEntity)
        {
            name = HoverdEntity.GetComponent<TagComponent>().Tag;
        }
        ImGui::Text("HoverEntity: %s", name.c_str());
        ImGui::Checkbox("Show ColliderBox", &m_colliderbox);


        ImGui::End();
     
        //ViewPort
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::Begin("ViewPort");
        m_viewportfocus = ImGui::IsWindowFocused();
        m_viewporthover = ImGui::IsWindowHovered();
        Application::Get().GetImGuiLayer()->SetBlockEvent(!m_viewportfocus&&!m_viewporthover);
       
        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        m_ViewPortSize = { viewportPanelSize.x, viewportPanelSize.y };

        uint32_t textureID = m_FrameBuffer->GetColorAttachment();
        
        ImGui::Image((void*)textureID, ImVec2{ m_ViewPortSize.x ,m_ViewPortSize.y }, ImVec2{ 0 , 1 }, ImVec2{ 1, 0 });
		//DragDropTarget
        if (ImGui::BeginDragDropTarget())
        {
            const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM");
            if (payload!=nullptr)
            {
                const wchar_t* path = (const wchar_t*)payload->Data;
                OpenScene(path);
            }

            ImGui::EndDragDropTarget();
        }
        
        ImGui::PopStyleVar();
        //WindowBounds
        auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
        auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
        auto viewportOffset = ImGui::GetWindowPos();
        m_ViewPortBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
        m_ViewPortBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

        //Gizmo
        Entity SelectedEntity = m_Panel.GetSelectedEntity();
        if (SelectedEntity&&m_GizmoType!=-1)
        {
            ImGuizmo::SetOrthographic(false);
            ImGuizmo::SetDrawlist();

            ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y,ImGui::GetWindowWidth(), ImGui::GetWindowHeight());
            
            //RuntimeCamera
           /* auto CameraEntity = m_Scene->GetPrimaryCameraEntity();
            auto& CameraComp = CameraEntity.GetComponent<CameraComponent>();
            auto Camera = CameraComp.camera;
            auto CameraView =glm::inverse(CameraEntity.GetComponent<TransformComponent>().GetTransform());
            auto CameraProjection = CameraComp.camera.GetProjection();*/
            //EditorCamera
            auto CameraView = m_EditorCamera.GetViewMatrix();
            auto CameraProjection =m_EditorCamera.GetProjection();

            //Entity
            auto& transform = SelectedEntity.GetComponent<TransformComponent>();
            glm::mat4 transformMat4 = transform.GetTransform();

            //Snap
            bool snap = Input::IsKeyPressed(HZ_KEY_LEFT_CONTROL);
            float snapValue = 0.5f;//for tranlation
            if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
            {
                snapValue = 45.0f;
            }

            float snapValues[3] = { snapValue,snapValue,snapValue };
            ImGuizmo::Manipulate(glm::value_ptr(CameraView), glm::value_ptr(CameraProjection),
                (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::MODE::LOCAL, glm::value_ptr(transformMat4),nullptr,snap?snapValues:nullptr);

            auto LastRotation=transform.Rotation;

            if (ImGuizmo::IsUsing())
            {
                glm::vec3 scale;
                glm::quat rotation;
                glm::vec3 translation;
                glm::vec3 skew;
                glm::vec4 perspective;
                glm::decompose(transformMat4, scale, rotation, translation, skew, perspective);

                transform.Translation = translation;
                transform.Rotation += glm::degrees(glm::eulerAngles(rotation)) -LastRotation;
                transform.Scale = scale;
            }
        }
        

        ImGui::End();

        UI_Toolbar();

        ImGui::End();
    }

    void CialloEditor2D::UI_Toolbar()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0,2});
        ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, { 0,0 });
        ImGui::PushStyleColor(ImGuiCol_Button, {0,0,0,0});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.15,0.15,0.15 ,1});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.3,0.3,0.3 ,1 });


        ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
        float size = ImGui::GetWindowSize().y - 5.0f;
      
        Ref<Texture2D> Currenticon = m_SceneState==SceneState::Play?m_iconplay:m_iconedit;
        ImGui::SetCursorPosX(ImGui::GetWindowContentRegionWidth()*0.5-size*0.5);
        if (ImGui::ImageButton((ImTextureID)Currenticon->GetRendererID(), ImVec2{ size,size }, {0,0}, { 1,1 },0))
        {
            if (m_SceneState == SceneState::Edit)
            {
                OnScenePlay();
            }
            else
            {
                OnSceneEdit();
            }
        }

        ImGui::End();

        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(3);
    }

    void CialloEditor2D::OnOverlayRender()
    {
        if (m_colliderbox)
        {
            if (m_SceneState == SceneState::Edit)
            {
                Entity camera = m_ActiveScene->GetPrimaryCameraEntity();
                Renderer2D::BeginScene(camera.GetComponent<CameraComponent>().camera, camera.GetComponent<TransformComponent>().GetTransform());
            }
            else
            {
                Renderer2D::BeginScene(m_EditorCamera);
            }

            auto view = m_ActiveScene->GetComponentEntity<TransformComponent, CircleCollider2DComponent>();
            for (auto e : view)
            {
                auto [tranform, collider] = view.get<TransformComponent, CircleCollider2DComponent>(e);

                glm::vec3 tranlation = tranform.Translation + glm::vec3(collider.offset, 0.04);
                glm::vec3 scale = tranform.Scale * glm::vec3(collider.size * 2.0f);

                glm::mat4 newtranform = glm::translate(glm::mat4(1.0f), tranlation) * glm::scale(glm::mat4(1.0f), scale);
                Renderer2D::DrawCircle(newtranform, { 0.0f,1.0f,0.0f,1.0f }, 0.05f);
            }

            auto view1 = m_ActiveScene->GetComponentEntity<TransformComponent, BoxCollider2DComponent>();
            for (auto e : view1)
            {
                auto [tranform, collider] = view1.get<TransformComponent, BoxCollider2DComponent>(e);

                glm::vec3 tranlation = tranform.Translation + glm::vec3(collider.offset, 0.04f);
                glm::vec3 scale = tranform.Scale * glm::vec3(collider.size * 2.0f, 1.0f);

                glm::mat4 newtranform = glm::translate(glm::mat4(1.0f), tranlation) *
                    glm::rotate(glm::mat4(1.0f), glm::radians(tranform.Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)) *
                    glm::scale(glm::mat4(1.0f), scale);
                Renderer2D::DrawRect(newtranform, { 0.0f,1.0f,0.0f,1.0f });
            }
            Renderer2D::EndScene();
        }
       
    }

    void CialloEditor2D::OnSceneEdit()
    {
      
        m_ActiveScene = Scene::Copy(m_EditScene);
        m_ActiveScene->OnRuntimeStart();

        m_SceneState = SceneState::Edit;
    }

    void CialloEditor2D::OnScenePlay()
    {
        m_ActiveScene->OnRuntimeEnd();
        m_ActiveScene = m_EditScene;

        m_SceneState = SceneState::Play;
    }

    void CialloEditor2D::OnDuplicateEntity()
    {
        if (m_SceneState == SceneState::Edit)
        {
            HZ_CORE_ERROR("CANT DUPILCATE");
            return;
        }
            


        Entity entity = m_Panel.GetSelectedEntity();
        if (entity)
        {
            m_EditScene->DuplicateEntity(entity);
        }
    }

    void CialloEditor2D::OnEvent(Event& event)
    {
        HZ_PROFILE_FUNCTION();
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(CialloEditor2D::OnKeyPressed));
        dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(CialloEditor2D::OnMousePressed));
    
        m_EditorCamera.OnEvent(event);
    }

    bool CialloEditor2D::OnMousePressed(MouseButtonPressedEvent& e)
    {
        if (Input::IsMouseButtonPressed(HZ_MOUSE_BUTTON_LEFT))
        {
            if (m_viewporthover&&!ImGuizmo::IsOver()&&!Input::IsKeyPressed(HZ_KEY_LEFT_CONTROL))
            {
                m_Panel.SetSelectedEntity(HoverdEntity);
            }
        }

        return false;
    }

    bool CialloEditor2D::OnKeyPressed(KeyPressedEvent& e)
    {
        if(e.GetRepeatCount()>0)
            return false;

        bool control = Input::IsKeyPressed(HZ_KEY_LEFT_CONTROL) || Input::IsKeyPressed(HZ_KEY_RIGHT_CONTROL);
        bool shift = Input::IsKeyPressed(HZ_KEY_LEFT_SHIFT) || Input::IsKeyPressed(HZ_KEY_RIGHT_SHIFT);

        switch (e.GetKeyCode())
        {
            case HZ_KEY_N:
            {
                if (control)
                {
                    NewScene();
                }

                break;
            }
            case HZ_KEY_O:
            {
                if (control)
                {
                    OpenScene();
                }

                break;
            }
            case HZ_KEY_S:
            {
                if (control && shift)
                {
                    SaveSceneAs();
                }

                break;
            }
            case HZ_KEY_D:
            {
                if (control)
                {
                    OnDuplicateEntity();
                }

                break;
            }
            case HZ_KEY_Q:
            {
                m_GizmoType = -1;
                break;
            }
            case HZ_KEY_W:
            {
                m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
                break;
            }
            case HZ_KEY_E:
            {
                m_GizmoType = ImGuizmo::OPERATION::ROTATE;
                break;
            }
            case HZ_KEY_R:
            {
                m_GizmoType = ImGuizmo::OPERATION::SCALE;
                break;
            }
        }
    } 

    void CialloEditor2D::NewScene()
    {
        m_EditScene->ClearEntity();
        m_EditScene = CreatRef<Scene>();
        m_EditScene->OnViewPortResize(m_ViewPortSize.x, m_ViewPortSize.y);
        m_ActiveScene = m_EditScene;
        m_Panel.SetContext(m_EditScene);
        m_CurrentPath = std::filesystem::path();
    }

    void CialloEditor2D::OpenScene()
    {
        std::string filepath = FileDialog::OpenFile("Ciallo Scene (*0.yaml)\0*.yaml\0");

        if (!filepath.empty())
        {
            OpenScene(filepath);
        }
    }

    void CialloEditor2D::OpenScene(const std::filesystem::path& path)
    {
        if (m_SceneState != SceneState::Play)
        {
            OnScenePlay();
        }

        std::string s = path.string();
        std::regex pattern("\.yaml");
        if (!std::regex_search(s,pattern))
        {
            HZ_CORE_ERROR("WRONG FIlE TYPE:{0}",s.c_str());
            return;
        }

        Ref<Scene> newScene= CreatRef<Scene>();
        SceneSerializer serializer(newScene);
        if (serializer.Deserialize(path.string()))
        {
            m_EditScene = newScene;
            m_EditScene->OnViewPortResize((uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);
            m_ActiveScene = m_EditScene;
            m_Panel.SetContext(m_EditScene);

            m_CurrentPath = path;
        }
    }

    void CialloEditor2D::SaveSceneAs()
    {
        std::string filepath = FileDialog::SaveFile("Ciallo Scene (*.yaml)\0*.yaml\0");

        if (!filepath.empty())
        {
            SceneSerializer serializer(m_ActiveScene);
            serializer.Serialize(filepath);

            m_CurrentPath = filepath;
        }
    }

    void CialloEditor2D::SaveScene()
    {
        if (!m_CurrentPath.empty())
        {
            SceneSerializer serializer(m_ActiveScene);
            serializer.Serialize(m_CurrentPath.string());
        }
    }


}
    