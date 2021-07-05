#pragma once

#include "entt.hpp"

#include "Hazel/Core/Timestep.h"

namespace Hazel
{
	class Entity;

	class Scene
	{
	public:
		Scene();

		Entity CreateEntity(const std::string& name = std::string());

		// TEMP
		entt::registry& Reg() { return m_Registry; }

		void OnUpdate(Timestep ts);
		
		~Scene();
	private:
		entt::registry m_Registry;

		friend class Entity;
	};
}