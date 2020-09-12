#include <Hazel.h>

class ExampleLayer : public Hazel::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate() override
	{
		//HZ_INFO("Example::Update");

		if(Hazel::Input::IsMouseButtonPressed(HZ_MOUSE_BUTTON_1))
			HZ_INFO("Mouse Button 1 is pressed!");

	}

	void OnEvent(Hazel::Event& event) override
	{
		HZ_TRACE("{0}", event);
	}
};

class Sandbox : public Hazel::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new Hazel::ImGuiLayer());
	}

	~Sandbox() {}
};

Hazel::Application* Hazel::CreateApplication()
{
	return new Sandbox();
}