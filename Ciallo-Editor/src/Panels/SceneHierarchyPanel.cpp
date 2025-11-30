#include "SceneHierarchyPanel.h"
#include "imgui.h"
#include "Ciallo/Scene/Components.h"
#include "glm/gtc/type_ptr.hpp"
#include "imgui_internal.h"
#include <filesystem>

namespace Ciallo
{
	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene)
	{
		SetContext(scene);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& scene)
	{
		m_Context = scene;
		SelectedEntity = {};
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");
		m_Context->m_Registry.each([&](auto entityID)
			{
				Entity entity{ entityID,m_Context.get() };
				DrawEntityNode(entity);
			});


		if (ImGui::IsMouseClicked(0) && ImGui::IsWindowHovered())
		{
            SelectedEntity = {};
		}

		if (ImGui::BeginPopupContextWindow(0,1,false))
		{
			if(ImGui::MenuItem("Create Empty Entity"))
                m_Context->CreateEntity("Empty Entity");

			ImGui::EndPopup();
		}

		ImGui::End();

		ImGui::Begin("Properties");

		if (SelectedEntity)
		{
			DrawComponent(SelectedEntity);

		}

		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>();

		ImGuiTreeNodeFlags flags = ((SelectedEntity==entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.Tag.c_str());
		if (ImGui::IsItemClicked())
		{
			SelectedEntity = entity;
		}

		bool EntityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
				EntityDeleted = true;

			ImGui::EndPopup();
		}
		if (opened)
		{
            ImGui::TreePop();
		}

		if (EntityDeleted)
		{
            m_Context->DestroyEntity(entity);
			SelectedEntity = {};
		}
	}

	static void DrawTransUI(const std::string& name,glm::vec3& values,float resetvalue=0.0f,float columnwidth=140.0f )
	{ 
		ImGui::PushID(name.c_str());
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnwidth);
		ImGui::Text(name.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0.0f,0.0f});

		float lineheight=GImGui->Font->FontSize+GImGui->Style.FramePadding.y*2.0f;
		ImVec2 buttonSize = {lineheight+3.0f,lineheight};

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.2f, 0.2f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
		if(ImGui::Button("X",buttonSize))
            values.x=resetvalue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f);
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f, 0.3f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
		if (ImGui::Button("Y", buttonSize))
			values.y = resetvalue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f);
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.35f, 0.9f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
		if (ImGui::Button("Z", buttonSize))
			values.z = resetvalue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f);
		ImGui::PopItemWidth();
		
		ImGui::PopStyleVar();
		ImGui::Columns(1);
		ImGui::PopID();
	}
	template<typename T, typename Func>
	static void DrawCompUI(const std::string& name,Entity entity,Func func)
	{ 
		if (entity.HasComponent<T>())
		{
			const ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_AllowItemOverlap
				| ImGuiTreeNodeFlags_SpanAvailWidth;
			auto& Component = entity.GetComponent<T>();

			ImGui::Separator();

			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), flags, name.c_str());
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4,-1 });
			ImGui::SameLine(ImGui::GetWindowWidth() - 45.0f);
			if (ImGui::Button("+", ImVec2{ 25.0f,25.0f }))
			{
				ImGui::OpenPopup(name.c_str());
			}
			ImGui::PopStyleVar();

			bool ComponentDelete = false;
			if (ImGui::BeginPopup(name.c_str()))
			{
				if (ImGui::MenuItem("Delete Component"))
				{
					ComponentDelete = true;
				}

				ImGui::EndPopup();
			}

			if (open)
			{
				func(Component);
				ImGui::TreePop();
			}

			if (ComponentDelete)
			{
				entity.DeleteComponent<T>();
			}
		}
	}

	void SceneHierarchyPanel::DrawComponent(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag= entity.GetComponent<TagComponent>().Tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			memcpy(buffer, tag.c_str(), tag.size());

			if (ImGui::InputText("##<<Entity_Tag", buffer, sizeof(buffer)))
			{
				tag=std::string(buffer);
			}
		}
		
		if (ImGui::Button("Add Component"))
		{
			ImGui::OpenPopup("Add Component");
		}

		if (ImGui::BeginPopup("Add Component"))
		{
			if (ImGui::MenuItem("Add Sprite"))
			{
				SelectedEntity.AddComponent<SpriteComponent>();
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItem("Add Camera"))
			{
				SelectedEntity.AddComponent<CameraComponent>();
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItem("Add Rigid2D"))
			{
				SelectedEntity.AddComponent<Rigidbody2DComponent>();
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItem("Add Box Collider"))
			{
				SelectedEntity.AddComponent<BoxCollider2DComponent>();
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItem("Add CircleSprite"))
			{
				SelectedEntity.AddComponent<CircleSpriteComponent>();
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItem("Add Circle Collider"))
			{
				SelectedEntity.AddComponent<CircleCollider2DComponent>();
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		DrawCompUI<TransformComponent>("TranslateComponent", entity, [](auto& comp)
			{
				DrawTransUI("Translation:", comp.Translation);
				DrawTransUI("Rotation:", comp.Rotation);
				DrawTransUI("Scale:", comp.Scale, 1.0f);
			});

		DrawCompUI<CircleSpriteComponent>("CircleSpriteComponent", entity, [](auto& comp)
			{
				ImGui::ColorEdit4("Sprite Color", glm::value_ptr(comp.Color));
				ImGui::DragFloat("Thickness", &comp.Thickness, 0.025f, 0.0f, 1.0f);
				ImGui::DragFloat("fade", &comp.fade, 0.00025f, 0.0f, 1.0f);

			});

		DrawCompUI<CameraComponent>("CameraComponent", entity, [](auto& comp)
			{
				auto& camera = comp.camera;
				std::string cameraType[] = { "Perspective" ,"Orthographic" };
				int CurrentType = (int)(camera.GetCameraType());
				if (ImGui::BeginCombo("Projection", cameraType[CurrentType].c_str()))
				{
					for (int i = 0; i < 2; i++)
					{
						bool isSelected = CurrentType == i;
						if (ImGui::Selectable(cameraType[i].c_str(), isSelected))
						{
							CurrentType = i;
							camera.SetCameraType((CameraType)CurrentType);
						}

						if (isSelected)
						{
							ImGui::SetItemDefaultFocus();
						}
					}
					ImGui::EndCombo();
				}

				if (camera.GetCameraType() == CameraType::Orthographic)
				{
					if (ImGui::DragFloat("Camera Size", &camera.GetOrthographicCameraSize(), 0.1f, 0.0f))
					{
						camera.SetOrthographicCameraSize(camera.GetOrthographicCameraSize());
					}

					if (ImGui::DragFloat("Near Clip", &camera.GetOrthographicNearClip(), 0.1f, 0.0f))
					{
						camera.SetOrthographicNearClip(camera.GetOrthographicNearClip());
					}

					if (ImGui::DragFloat("Far Clip", &camera.GetOrthographicFarClip(), 0.1f, 0.0f))
					{
						camera.SetOrthographicFarClip(camera.GetOrthographicFarClip());
					}

					ImGui::Checkbox("Fixed AspectRadio", &comp.FixedAspectRadio);

				}

				if (camera.GetCameraType() == CameraType::Perspective)
				{
					if (ImGui::DragFloat("Camera FOV", &camera.GetPerpectiveCameraFov(), 0.1f, 0.0f))
					{
						camera.SetPerpectiveCameraFov(camera.GetPerpectiveCameraFov());
					}

					if (ImGui::DragFloat("Near Clip", &camera.GetPerpectiveNearClip(), 0.1f, 0.0f))
					{
						camera.SetPerpectiveNearClip(camera.GetPerpectiveNearClip());
					}

					if (ImGui::DragFloat("Far Clip", &camera.GetPerpectiveFarClip(), 0.1f, 0.0f))
					{
						camera.SetPerpectiveFarClip(camera.GetPerpectiveFarClip());
					}

					ImGui::Checkbox("Fixed AspectRadio", &comp.FixedAspectRadio);
				}
			});

		DrawCompUI<SpriteComponent>("SpriteComponent", entity, [](auto& comp)
			{
				ImGui::ColorEdit4("Sprite Color", glm::value_ptr(comp.Color));
				ImGui::DragFloat("TillingFactor", &comp.TillingFactor,0.1f,0.0f,100.0f);
				
				ImGui::Button("Texture", {120.0f,120.0f});
				//DragDropTarget
				if (ImGui::BeginDragDropTarget())
				{
					const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM");
					if (payload != nullptr)
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						std::filesystem::path f_path = path;


						comp.texture = Texture2D::Create(f_path.string());
					}

					ImGui::EndDragDropTarget();
				}

			});
	
		DrawCompUI<Rigidbody2DComponent>("RigidComponent", entity, [](auto& comp)
			{
				std::string RigidType[] = { "Static" ,"Dynamic","Kinematic"};
				std::string CurrentString = RigidType[(int)comp.Type];

				if (ImGui::BeginCombo("BodyType",CurrentString.c_str()))
				{
					for (int i = 0; i < 3; i++)
					{
						bool isSelected = CurrentString == RigidType[i];
						if (ImGui::Selectable(RigidType[i].c_str(), isSelected))
						{
							CurrentString = RigidType[i];
							comp.Type = ((Rigidbody2DComponent::BodyType)i);
						}

						if (isSelected)
						{
							ImGui::SetItemDefaultFocus();
						}
					}
					ImGui::EndCombo();
				}

				ImGui::Checkbox("Fixed Rotation", &comp.FixedRotation);

			});

		DrawCompUI<BoxCollider2DComponent>("BoxColliderComponent", entity, [](auto& comp)
			{
				ImGui::DragFloat2("Offset",glm::value_ptr(comp.offset));
				ImGui::DragFloat2("Size", glm::value_ptr(comp.size));
				ImGui::DragFloat("Density",&comp.Density,0.1f,0.0f,1.0f);
				ImGui::DragFloat("Friction", &comp.Friction, 0.1f, 0.0f, 1.0f);
				ImGui::DragFloat("Restitution", &comp.Restitution, 0.1f, 0.0f, 1.0f);
				ImGui::DragFloat("RestitutionThreshold", &comp.RestitutionThreshold, 0.1f, 0.0f, 1.0f);

			});

		DrawCompUI<CircleCollider2DComponent>("CircleColliderComponent", entity, [](auto& comp)
			{
				ImGui::DragFloat2("Offset", glm::value_ptr(comp.offset));
				ImGui::DragFloat("Size", &comp.size);
				ImGui::DragFloat("Density", &comp.Density, 0.1f, 0.0f, 1.0f);
				ImGui::DragFloat("Friction", &comp.Friction, 0.1f, 0.0f, 1.0f);
				ImGui::DragFloat("Restitution", &comp.Restitution, 0.1f, 0.0f, 1.0f);
				ImGui::DragFloat("RestitutionThreshold", &comp.RestitutionThreshold, 0.1f, 0.0f, 1.0f);

			});
	}
}
