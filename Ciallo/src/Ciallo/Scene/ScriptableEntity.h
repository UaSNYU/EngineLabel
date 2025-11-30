#pragma once

#include "Scene.h"
#include "Entity.h"

namespace Ciallo
{
	class ScriptEntity
	{
	public:
		virtual ~ScriptEntity() {}
		

		template<typename T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}

	protected:
    public:
		virtual void OnCreate() {}

        virtual void OnUpdate(TimeStep timestep){}

		virtual void OnDestroy() {}

	private:
		Entity m_Entity;
		friend class Scene;
	};

}