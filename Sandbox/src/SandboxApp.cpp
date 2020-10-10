#include <Hazel.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Hazel::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f, 0.0f, 0.0f), m_SquarePosition(0.0f)
	{
		m_VertexArray.reset(Hazel::VertexArray::Create());

		float vertices[3 * 7] =
		{
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};
		
		vertexBuffer.reset(Hazel::VertexBuffer::Create(vertices, sizeof(vertices)));
		vertexBuffer->Bind();

		Hazel::BufferLayout layout =
		{
			{ Hazel::ShaderDataType::Float3, "a_Position" },
			{ Hazel::ShaderDataType::Float4, "a_Color" }
		};

		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<Hazel::IndexBuffer> indexBuffer;
		indexBuffer.reset(Hazel::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA.reset(Hazel::VertexArray::Create());

		float squareVertices[3 * 4] =
		{
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		std::shared_ptr<Hazel::VertexBuffer> squareVB;
		squareVB.reset(Hazel::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ Hazel::ShaderDataType::Float3, "a_Position" }
		});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Hazel::IndexBuffer> squareIB;
		squareIB.reset(Hazel::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSrc =
		R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrc =
		R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

		m_Shader.reset(Hazel::Shader::Create(vertexSrc, fragmentSrc));

		std::string flatColorShaderVertexSrc =
		R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string flatColorFragmentSrc =
		R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			uniform vec3 u_Color;

			void main()
			{
				color = vec4(u_Color, 1.0);
			}
		)";

		m_FlatColorShader.reset(Hazel::Shader::Create(flatColorShaderVertexSrc, flatColorFragmentSrc));
	}

	void OnUpdate(Hazel::Timestep ts) override
	{
		//HZ_INFO("Example::Update");
		//HZ_TRACE("Delta time : {0}s ({1}ms)", ts.GetSeconds(), ts.GetMilliseconds());
		if(Hazel::Input::IsKeyPressed(HZ_KEY_LEFT_SHIFT))
		{
			if(Hazel::Input::IsKeyPressed(HZ_KEY_LEFT_SHIFT) && (Hazel::Input::IsKeyPressed(HZ_KEY_A) || Hazel::Input::IsKeyPressed(HZ_KEY_LEFT)))
				m_CameraRotation -= m_CameraRotationSpeed * ts;
			if(Hazel::Input::IsKeyPressed(HZ_KEY_LEFT_SHIFT) && (Hazel::Input::IsKeyPressed(HZ_KEY_D) || Hazel::Input::IsKeyPressed(HZ_KEY_RIGHT)))
				m_CameraRotation += m_CameraRotationSpeed * ts;
		}
		if(Hazel::Input::IsKeyPressed(HZ_KEY_LEFT_CONTROL))
		{
			if(Hazel::Input::IsKeyPressed(HZ_KEY_LEFT_CONTROL) && (Hazel::Input::IsKeyPressed(HZ_KEY_A) || Hazel::Input::IsKeyPressed(HZ_KEY_LEFT)))
				m_SquarePosition.x -= m_SquareMoveSpeed * ts;
			if(Hazel::Input::IsKeyPressed(HZ_KEY_LEFT_CONTROL) && (Hazel::Input::IsKeyPressed(HZ_KEY_D) || Hazel::Input::IsKeyPressed(HZ_KEY_RIGHT)))
				m_SquarePosition.x += m_SquareMoveSpeed * ts;
			if(Hazel::Input::IsKeyPressed(HZ_KEY_LEFT_CONTROL) && Hazel::Input::IsKeyPressed(HZ_KEY_W) || Hazel::Input::IsKeyPressed(HZ_KEY_UP))
				m_SquarePosition.y += m_SquareMoveSpeed * ts;
			if(Hazel::Input::IsKeyPressed(HZ_KEY_LEFT_CONTROL) && Hazel::Input::IsKeyPressed(HZ_KEY_S) || Hazel::Input::IsKeyPressed(HZ_KEY_DOWN))
				m_SquarePosition.y -= m_SquareMoveSpeed * ts;
		}
		else
		{
			if(Hazel::Input::IsKeyPressed(HZ_KEY_A) || Hazel::Input::IsKeyPressed(HZ_KEY_LEFT))
				m_CameraPosition.x -= m_CameraMoveSpeed * ts;
			if(Hazel::Input::IsKeyPressed(HZ_KEY_D) || Hazel::Input::IsKeyPressed(HZ_KEY_RIGHT))
				m_CameraPosition.x += m_CameraMoveSpeed * ts;
			if(Hazel::Input::IsKeyPressed(HZ_KEY_W) || Hazel::Input::IsKeyPressed(HZ_KEY_UP))
				m_CameraPosition.y += m_CameraMoveSpeed * ts;
			if(Hazel::Input::IsKeyPressed(HZ_KEY_S) || Hazel::Input::IsKeyPressed(HZ_KEY_DOWN))
				m_CameraPosition.y -= m_CameraMoveSpeed * ts;
		}

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Hazel::RenderCommand::Clear();

		Hazel::Renderer::BeginScene(m_Camera);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
		
		std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);

		for(int y = 0; y < 20; y++)
		{
			for(int x = 0; x < 20; x++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Hazel::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
			}
		}

		//Hazel::Renderer::Submit(m_Shader, m_VertexArray);

		Hazel::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("SquareColor", glm::value_ptr(m_SquareColor));
		ImGui::ProgressBar(0.4f);
		ImGui::End();
	}

	void OnEvent(Hazel::Event& event) override
	{
		/*HZ_TRACE("{0}", event);*/
	}
private:
	std::shared_ptr<Hazel::VertexArray> m_VertexArray;
	std::shared_ptr<Hazel::VertexBuffer> vertexBuffer;
	std::shared_ptr<Hazel::VertexArray> m_SquareVA;
	std::shared_ptr<Hazel::Shader> m_Shader;
	std::shared_ptr<Hazel::Shader> m_FlatColorShader;

	Hazel::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 1.0f;
	float m_SquareMoveSpeed = 1.0f;
	
	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 100.0f;

	glm::vec3 m_SquarePosition;
	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};

class Sandbox : public Hazel::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox() {}
};

Hazel::Application* Hazel::CreateApplication()
{
	return new Sandbox();
}