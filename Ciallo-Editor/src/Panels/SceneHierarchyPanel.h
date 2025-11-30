#pragma once

#include "Ciallo/Core/Core.h"
#include "Ciallo/Scene/Scene.h"
#include "Ciallo/Scene/Entity.h"

namespace Ciallo { 
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& scene);

		void SetContext(const Ref<Scene>& scene);

		void SetSelectedEntity(Entity entity) { SelectedEntity = entity; }

		Entity GetSelectedEntity() { return SelectedEntity; }

		void OnImGuiRender();
	private:
		void DrawEntityNode(Entity entity);
		void DrawComponent(Entity entity);

	private:
		Ref<Scene> m_Context;
		Entity SelectedEntity;
	};
}