#include "clpch.h"
#include "Scene.h"
#include "Components.h"
#include "ScriptableEntity.h"
#include "Ciallo/Renderer/Renderer2D.h"
#include "Entity.h"
#include "EditorCamera.h"
#include "box2d/b2_world.h"
#include "box2d/b2_polygon_shape.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_body.h"
#include "box2d/b2_circle_shape.h"

namespace Ciallo
{
	static b2BodyType RigidBodyTypeToBox2D(Rigidbody2DComponent::BodyType type)
	{
		switch (type)
		{
		case Ciallo::Rigidbody2DComponent::BodyType::Static:
			return b2_staticBody;
		case Ciallo::Rigidbody2DComponent::BodyType::Dynamic:
			return b2_dynamicBody;
		case Ciallo::Rigidbody2DComponent::BodyType::Kinematic:
			return b2_kinematicBody;
		}
	}

	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
		
	}

	template<typename Component>
	static void CopyComponent(entt::registry& dest, entt::registry& src, std::unordered_map<UUID, entt::entity>& enttmap)
	{
		auto view = src.view<Component>();
		for (auto e : view)
		{
			UUID srcID = src.get<UUIDComponent>(e).ID;
			entt::entity desId = enttmap.at(srcID);

			auto& comp = src.get<Component>(e);
			dest.emplace_or_replace<Component>(desId,comp);
		}
	}

	template<typename Component>
	static void CopyComponentIfExists(Entity dest,Entity src)
	{
		if (src.HasComponent<Component>())
		{
			dest.AddOrReplaceComponent<Component>(src.GetComponent<Component>());
		}
	}

	Ref<Scene> Scene::Copy(Ref <Scene> other)
	{
		Ref<Scene> newScene = CreatRef<Scene>();

		newScene->m_ViewPortwidth = other->m_ViewPortwidth;
		newScene->m_ViewPortheight = other->m_ViewPortheight;

		std::unordered_map<UUID, entt::entity> enttMap;

		auto& srcSceneRegistry = other->m_Registry;
		auto& desSceneRegistry = newScene->m_Registry;

		auto view = srcSceneRegistry.view<UUIDComponent>();
		for (auto e : view)
		{
			UUID id = srcSceneRegistry.get<UUIDComponent>(e).ID;
			auto& tag = srcSceneRegistry.get<TagComponent>(e).Tag;
			Entity& newEntity= newScene->CreateEntityWithUUID(id, tag);

			enttMap[id] = (entt::entity)newEntity;
		}

		CopyComponent<TransformComponent>(desSceneRegistry,srcSceneRegistry,enttMap);
		CopyComponent<CircleSpriteComponent>(desSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponent<CameraComponent>(desSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponent<SpriteComponent>(desSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponent<Rigidbody2DComponent>(desSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponent<BoxCollider2DComponent>(desSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponent<CircleCollider2DComponent>(desSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponent<NativeScriptComponent>(desSceneRegistry, srcSceneRegistry, enttMap);

		return newScene;
	}

	Entity Scene::CreateEntityWithUUID(uint64_t id, const std::string& name)
	{
		Entity entity(m_Registry.create(), this);
		entity.AddComponent<UUIDComponent>(id);
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Untitled Entity" : name;

		return entity;
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		return CreateEntityWithUUID(UUID(), name);
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

	void Scene::DuplicateEntity(Entity entity)
	{
		Entity newEntity = CreateEntity(entity.GetComponent<TagComponent>().Tag);

		CopyComponentIfExists<TransformComponent>(newEntity,entity);
		CopyComponentIfExists<CircleSpriteComponent>(newEntity, entity);
		CopyComponentIfExists<CameraComponent>(newEntity, entity);
		CopyComponentIfExists<SpriteComponent>(newEntity, entity);
		CopyComponentIfExists<Rigidbody2DComponent>(newEntity, entity);
		CopyComponentIfExists<BoxCollider2DComponent>(newEntity, entity);
		CopyComponentIfExists<CircleCollider2DComponent>(newEntity, entity);
		CopyComponentIfExists<NativeScriptComponent>(newEntity, entity);
	}

	Entity Scene::GetPrimaryCameraEntity()
	{
		auto view =m_Registry.view<CameraComponent>();

		for (auto entity : view)
		{
			Entity e{ entity,this };

			if (e.GetComponent<CameraComponent>().Primary = true)
			{
                return e;
			}
		}
	}

	void Scene::OnUpdateEditor(TimeStep ts, EditorCamera& editorCamera)
	{
		Renderer2D::BeginScene(editorCamera);

		{
			auto group = m_Registry.view<TransformComponent, SpriteComponent>();
			for (auto [entity, transform, sprite] : group.each())
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteComponent>(entity);
				Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
				//Renderer2D::DrawRect(transform.GetTransform(), glm::vec4(1.0f), (int)entity);
			}
		}

		{
			auto group = m_Registry.view<TransformComponent, CircleSpriteComponent>();
			for (auto [entity, transform, sprite] : group.each())
			{
				auto [transform, sprite] = group.get<TransformComponent, CircleSpriteComponent>(entity);
				Renderer2D::DrawCircle(transform.GetTransform(), sprite.Color, sprite.Thickness, sprite.fade, (int)entity);
			}
		}
		

		Renderer2D::EndScene();
	}

	void Scene::OnUpdateRuntime(TimeStep ts)
	{
		//Script
		{
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, NativeScriptComponent& nsc)
				{
					if (!nsc.Instance)
					{
						nsc.Instance=nsc.InstanciateScript();
						nsc.Instance->m_Entity = Entity{entity,this};
						nsc.Instance->OnCreate();
					}
					
					nsc.Instance->OnUpdate(ts);
				});
		}

		//Physics
		{
			const uint32_t VelocityIterations = 6;
			const uint32_t  PositionIterations = 2;
			m_World->Step(ts,VelocityIterations, PositionIterations);

			auto view = m_Registry.view<Rigidbody2DComponent>();
			for (auto e : view)
			{
				Entity entity = {e,this};
				auto& transform = entity.GetComponent<TransformComponent>();
				auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

				b2Body* body = (b2Body*)rb2d.RuntimeBody;
				transform.Translation.x = body->GetPosition().x;
				transform.Translation.y = body->GetPosition().y;
				transform.Rotation.z = glm::degrees(body->GetAngle());
			}
		}


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
					cameratransform = &transform.GetTransform();
					break;
				}
			}
		}

		

		if (maincamera)
		{
			Renderer2D::BeginScene(maincamera->GetProjection(), *cameratransform);

			{
				auto group = m_Registry.view<TransformComponent, SpriteComponent>();
				for (auto [entity, transform, sprite] : group.each())
				{
					auto [transform, sprite] = group.get<TransformComponent, SpriteComponent>(entity);
					Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
				}
			}

			{
				auto group = m_Registry.view<TransformComponent, CircleSpriteComponent>();
				for (auto [entity, transform, sprite] : group.each())
				{
					auto [transform, sprite] = group.get<TransformComponent, CircleSpriteComponent>(entity);
					Renderer2D::DrawCircle(transform.GetTransform(), sprite.Color, sprite.Thickness, sprite.fade, (int)entity);
				}
			}

			Renderer2D::EndScene();
		}

		
	}

	void Scene::OnViewPortResize(uint32_t width, uint32_t height)
	{
		m_ViewPortwidth = width;
		m_ViewPortheight = height;

		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& Cameracomponent = view.get<CameraComponent>(entity);

			if (!Cameracomponent.FixedAspectRadio)
			{
				Cameracomponent.camera.SetViewPort(width, height);
				
			}
		}
	}

	void Scene::OnRuntimeStart()
	{
		m_World = new b2World({0.0f,-9.8f});

		auto& view = m_Registry.view<Rigidbody2DComponent>();
		for (auto e : view)
		{
			Entity entity = {e,this};
			auto& transform = entity.GetComponent<TransformComponent>();
			auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

			b2BodyDef bodydef;
			bodydef.type = RigidBodyTypeToBox2D(rb2d.Type);
			bodydef.position.Set(transform.Translation.x, transform.Translation.y);
			bodydef.angle = glm::radians(transform.Rotation.z);

			b2Body* body= m_World->CreateBody(&bodydef);
			rb2d.RuntimeBody = body;
			body->SetFixedRotation(rb2d.FixedRotation);

			if (entity.HasComponent<BoxCollider2DComponent>())
			{
				auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();
			
				b2PolygonShape boxshape;
				boxshape.SetAsBox(bc2d.size.x*transform.Scale.x, bc2d.size.y * transform.Scale.y);

				b2FixtureDef fixturedef;
				fixturedef.density = bc2d.Density;
				fixturedef.friction = bc2d.Friction;
				fixturedef.shape = &boxshape;
				fixturedef.restitution = bc2d.Restitution;
				fixturedef.restitutionThreshold = bc2d.RestitutionThreshold;
				body->CreateFixture(&fixturedef);

			}

			if (entity.HasComponent<CircleCollider2DComponent>())
			{
				auto& bc2d = entity.GetComponent<CircleCollider2DComponent>();

				b2CircleShape circleshape;
				circleshape.m_p.Set(bc2d.offset.x, bc2d.offset.y);
				circleshape.m_radius = bc2d.size*transform.Scale.x;

				b2FixtureDef fixturedef;
				fixturedef.density = bc2d.Density;
				fixturedef.friction = bc2d.Friction;
				fixturedef.shape = &circleshape;
				fixturedef.restitution = bc2d.Restitution;
				fixturedef.restitutionThreshold = bc2d.RestitutionThreshold;
				body->CreateFixture(&fixturedef);

			}
		}
	}

	void Scene::OnRuntimeEnd()
	{
		delete m_World;
		m_World = nullptr;
	}


	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		static_assert(false);
	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<SpriteComponent>(Entity entity, SpriteComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		component.camera.SetViewPort(m_ViewPortwidth, m_ViewPortheight );
	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<Rigidbody2DComponent>(Entity entity, Rigidbody2DComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<BoxCollider2DComponent>(Entity entity, BoxCollider2DComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<UUIDComponent>(Entity entity, UUIDComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<CircleSpriteComponent>(Entity entity, CircleSpriteComponent& component)
	{

	}


	template<>
	void Scene::OnComponentAdded<CircleCollider2DComponent>(Entity entity, CircleCollider2DComponent& component)
	{

	}

}

