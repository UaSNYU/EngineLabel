#include "clpch.h"
#include "Scene.h"
#include "Components.h"
#include "Ciallo/Renderer/Renderer2D.h"
#include "Entity.h"

namespace Ciallo
{
	

	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}


	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity  ( m_Registry.create(),this );
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Untitled Entity" : name;

		return entity;
	}

	void Scene::OnUpdate(TimeStep ts)
	{

		//Render Sprites
		Camera* maincamera = nullptr;
		glm::mat4* cameratransform = nullptr;
		{
			auto group = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto [entity,transform, camera] : group.each())
			{
				if (camera.Primary)
				{
					maincamera = &camera.camera;
					cameratransform = &transform.Transform;
					break;
				}
			}
		}

		if (maincamera)
		{
			Renderer2D::BeginScene(maincamera->GetProjection(), *cameratransform);

			auto group = m_Registry.view<TransformComponent,SpriteComponent>();
			for (auto [entity, transform, sprite] : group.each())
			{
				auto& [transform, sprite] = group.get<TransformComponent, SpriteComponent>(entity);

	
				Renderer2D::DrawQuad(transform.Transform, sprite.Color);
			}

			Renderer2D::EndScene();
		}

		
	}

	void Scene::OnViewPortResize(uint32_t height, uint32_t width)
	{
		m_ViewPortheight = height;
		m_ViewPortwidth -= width;

		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& Cameracomponent = view.get<CameraComponent>(entity);

			if (!Cameracomponent.FixedAspectRadio)
			{
				Cameracomponent.camera.SetViewPort(height, width);
			}
		}
	}
}