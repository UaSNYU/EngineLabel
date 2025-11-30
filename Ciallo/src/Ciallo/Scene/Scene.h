#pragma once

#include "entt.hpp"
#include "Ciallo/Core/TimeStep.h"
#include "Ciallo/Scene/EditorCamera.h"

class b2World;

namespace Ciallo
{
	class Entity;

	class SceneHierarchyPanel;

	class Scene
	{
	public:
		Scene();
		~Scene();

		static Ref<Scene> Copy(Ref <Scene> other);

		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateEntityWithUUID(uint64_t id,const std::string& name = std::string());
		void DestroyEntity(Entity entity);
		void ClearEntity() { m_Registry.clear(); }
		void DuplicateEntity(Entity entity);

		Entity GetPrimaryCameraEntity();

		void OnViewPortResize(uint32_t width,uint32_t height);

		void OnRuntimeStart();
		void OnRuntimeEnd();

		void OnUpdateEditor(TimeStep ts, EditorCamera& editorCamera);
		void OnUpdateRuntime(TimeStep ts);

		template<typename... Component>
		auto GetComponentEntity()
		{
			return m_Registry.view<Component...>();
		}

	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

	private:
		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;

		b2World* m_World;
		uint32_t m_ViewPortheight;
		uint32_t m_ViewPortwidth;
		entt::registry m_Registry;
	};
}
