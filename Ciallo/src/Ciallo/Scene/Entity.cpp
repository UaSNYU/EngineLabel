#include "clpch.h"
#include "Entity.h"

namespace Ciallo
{



	Entity::Entity(entt::entity handle, Scene* scene)
		:m_entityHandler(handle),m_scene(scene)
	{
	}

}