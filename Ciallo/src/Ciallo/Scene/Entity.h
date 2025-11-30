#pragma once

#include "entt.hpp"
#include "Scene.h"
#include "Components.h"

namespace Ciallo
{
	class Entity
	{
	public:

		Entity() = default;
		Entity(entt::entity handle,Scene* scene);
		Entity(const Entity& other) = default;

		uint64_t GetID() { return GetComponent<UUIDComponent>().ID; };

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			HZ_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");
			T& comp = m_scene->m_Registry.emplace<T>(m_entityHandler, std::forward<Args>(args)...);
			m_scene->OnComponentAdded<T>(*this,comp);
			return comp;
		}

		template<typename T, typename... Args>
		T& AddOrReplaceComponent(Args&&... args)
		{
			T& comp = m_scene->m_Registry.emplace_or_replace<T>(m_entityHandler, std::forward<Args>(args)...);
			m_scene->OnComponentAdded<T>(*this, comp);
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
			return m_scene->m_Registry.any_of<T>(m_entityHandler);
		}

		template<typename T>
		void DeleteComponent()
		{
			HZ_CORE_ASSERT(HasComponent<T>(), "Component dose not Exist!!!");

			m_scene->m_Registry.remove<T>(m_entityHandler);
		}
		
		operator bool()const { return (unsigned int)m_entityHandler != entt::null; }
		operator uint32_t() const { return (unsigned int)m_entityHandler; }
		bool operator==(const Entity& other) const { return m_entityHandler == other.m_entityHandler&&m_scene==other.m_scene; }
		bool operator!=(const Entity& other) const { return m_entityHandler != other.m_entityHandler || m_scene != other.m_scene; }
		operator entt::entity() const { return m_entityHandler; }
	private:
		entt::entity m_entityHandler{entt::null};
		Scene* m_scene=nullptr;

	};

}