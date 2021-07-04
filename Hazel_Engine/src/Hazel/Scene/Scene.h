#pragma once

#include "entt.hpp"

#include "Hazel/Core/Timestep.h"

using Entity = entt::entity;

namespace Hazel
{
	class Scene
	{
	public:
		Scene();

		Entity CreateEntity();

		// TEMP
		entt::registry& Reg() { return m_Registry; }

		void OnUpdate(Timestep ts);
		
		~Scene();
	private:
		entt::registry m_Registry;
	};
}