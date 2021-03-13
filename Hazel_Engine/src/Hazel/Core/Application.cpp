#include "hzpch.h"
#include "Application.h"

#include "Hazel/Core/Log.h"

#include "Hazel/Renderer/Renderer.h"

#include "Input.h"

#include <glfw/glfw3.h>

namespace Hazel 
{
	Application* Application::s_Instance = nullptr;

	Application::Application(const std::string& name)
		: m_Running(true)
	{
		HZ_PROFILE_FUNCTION();

		HZ_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = Window::Create(WindowProps(name));
		m_Window->SetEventCallback(HZ_BIND_EVENT_FN(Application::OnEvent));
		m_Window->SetVSync(true);

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		HZ_PROFILE_FUNCTION();

		Renderer::Shutdown();
	}

	void Application::PushLayer(Layer* layer)
	{
		HZ_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		HZ_PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::Close()
	{
		m_Running = false;
	}

	void Application::OnEvent(Event& e)
	{
		HZ_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(HZ_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(HZ_BIND_EVENT_FN(Application::OnWindowResize));

		//HZ_CORE_TRACE("{0}", e);

		for(auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if(e.Handled)
				break;
		}
	}

	void Application::Run()
	{
		HZ_PROFILE_FUNCTION();

		while(m_Running)
		{
			HZ_PROFILE_SCOPE("RunLoop");

			float time = (float)glfwGetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if(!m_Minimized)
			{
				{
					HZ_PROFILE_SCOPE("LayerStack OnUpdate");

					for(Layer* layer : m_LayerStack)
						layer->OnUpdate(timestep);
				}

				m_ImGuiLayer->Begin();
				{
					HZ_PROFILE_SCOPE("LayerStack OnImGuiRender");

					for(Layer* layer : m_LayerStack)
						layer->OnImGuiRender();
				}
				m_ImGuiLayer->End();
			}

			//auto[x, y] = Input::GetMousePosition();
			//HZ_CORE_TRACE("Mouse position X: {0}, Y: {1}", x, y);

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		HZ_PROFILE_FUNCTION();

		if(e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}
}