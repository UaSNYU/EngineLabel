#pragma once

#include "entt.hpp"
#include "Scene.h"

namespace Ciallo
{
	class Entity
	{
	public:

		Entity() = default;
		Entity(entt::entity handle,Scene* scene);
		Entity(const Entity& other) = default;


		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			HZ_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");
			std::cout << "Adding component: " << typeid(T).name() << std::endl;
			std::cout << "Entity handle: " << (int)m_entityHandler << std::endl;
			T& comp = m_scene->m_Registry.emplace<T>(m_entityHandler, std::forward<Args>(args)...);
			std::cout << "Added successfully.\n";
			return comp;
		}

		template<typename T>
		T& GetComponent()
		{
			HZ_CORE_ASSERT(HasComponent<T>(), "Component dose not Exist!!!");
			
			return m_scene->m_Registry.get<T>(m_entityHandler);
		}

		template<typename T>
		bool HasComponent()
		{
			return m_scene->m_Registry.has<T>(m_entityHandler);
		}

		template<typename T>
		void DeleteComponent()
		{
			HZ_CORE_ASSERT(HasComponent<T>(), "Component dose not Exist!!!");

			m_scene->m_Registry.remove<T>(m_entityHandler);
		}
		
		operator bool()const { return (unsigned int)m_entityHandler != entt::null; }
	
	private:
		entt::entity m_entityHandler{entt::null};
		Scene* m_scene=nullptr;

	};

}