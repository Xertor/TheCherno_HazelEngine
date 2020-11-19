#include "hzpch.h"
#include "OrthographicCameraController.h"

#include "Hazel/Core/Input.h"
#include "Hazel/Core/KeyCodes.h"

namespace Hazel
{
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_Rotation(rotation)
	{
	}

	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		//HZ_INFO("Example::Update");
		//HZ_TRACE("Delta time : {0}s ({1}ms)", ts.GetSeconds(), ts.GetMilliseconds());
		if(m_Rotation)
		{
			if(Input::IsKeyPressed(HZ_KEY_LEFT_SHIFT))
			{
				if(Input::IsKeyPressed(HZ_KEY_LEFT_SHIFT) && (Input::IsKeyPressed(HZ_KEY_A) || Input::IsKeyPressed(HZ_KEY_LEFT)))
					m_CameraRotation -= m_CameraRotationSpeed * ts;
				if(Input::IsKeyPressed(HZ_KEY_LEFT_SHIFT) && (Input::IsKeyPressed(HZ_KEY_D) || Input::IsKeyPressed(HZ_KEY_RIGHT)))
					m_CameraRotation += m_CameraRotationSpeed * ts;
			}
			else
			{
				if(Input::IsKeyPressed(HZ_KEY_Q))
					m_CameraRotation -= m_CameraRotationSpeed * ts;
				if(Input::IsKeyPressed(HZ_KEY_E))
					m_CameraRotation += m_CameraRotationSpeed * ts;
			}
			m_Camera.SetRotation(m_CameraRotation);
		}

		if(Input::IsKeyPressed(HZ_KEY_A) || Input::IsKeyPressed(HZ_KEY_LEFT))
			m_CameraPosition.x -= m_CameraTranslationSpeed * ts;
		if(Input::IsKeyPressed(HZ_KEY_D) || Input::IsKeyPressed(HZ_KEY_RIGHT))
			m_CameraPosition.x += m_CameraTranslationSpeed * ts;
		if(Input::IsKeyPressed(HZ_KEY_W) || Input::IsKeyPressed(HZ_KEY_UP))
			m_CameraPosition.y += m_CameraTranslationSpeed * ts;
		if(Input::IsKeyPressed(HZ_KEY_S) || Input::IsKeyPressed(HZ_KEY_DOWN))
			m_CameraPosition.y -= m_CameraTranslationSpeed * ts;

		m_Camera.SetPosition(m_CameraPosition);
		m_CameraTranslationSpeed = m_ZoomLevel;
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(HZ_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(HZ_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}
}