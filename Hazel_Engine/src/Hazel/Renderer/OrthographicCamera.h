#pragma once

#include <glm/glm.hpp>

namespace Hazel
{
	class OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);

		const glm::vec3& GetPosition() const { return m_Position; }
		const float GetPositionX() const { return m_Position.x; }
		const float GetPositionY() const { return m_Position.y; }
		const float GetPositionZ() const { return m_Position.z; }

		void SetPosition(const glm::vec3& position) { m_Position = position;  RecalculateViewMatrix(); }
		void SetPositionX(float x) { m_Position.x = x;  RecalculateViewMatrix(); }
		void SetPositionY(float y) { m_Position.y = y;  RecalculateViewMatrix(); }
		void SetPositionZ(float z) { m_Position.z = z;  RecalculateViewMatrix(); }

		float GetRotation() const { return m_Rotation; }
		void SetRotation(float rotation) { m_Rotation = rotation;  RecalculateViewMatrix(); }

		const glm::mat4 GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4 GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4 GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
	private:
		void RecalculateViewMatrix();
	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		float m_Rotation = 0.0f;
	};
}