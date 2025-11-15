#pragma once

#include "entt.hpp"
#include "Ciallo/Core/TimeStep.h"

namespace Ciallo
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());

		void OnViewPortResize(uint32_t height,uint32_t width);

		void OnUpdate(TimeStep ts);
	private:
		friend class Entity;

		uint32_t m_ViewPortheight;
		uint32_t m_ViewPortwidth;
		entt::registry m_Registry;
	};
}
