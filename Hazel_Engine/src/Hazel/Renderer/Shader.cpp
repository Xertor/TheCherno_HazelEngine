#include "hzpch.h"
#include "Shader.h"

#include "Renderer.h"
#include "Hazel/Platform/OpenGL/OpenGLShader.h"

namespace Hazel
{
	Ref<Hazel::Shader> Shader::Create(const std::string& filepath)
	{
		switch(Renderer::GetAPI())
		{
			case RendererAPI::API::None: HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return std::make_shared<OpenGLShader>(filepath);
		}

		HZ_CORE_ASSERT(false, "Uknown RendererAPI!");
		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch(Renderer::GetAPI())
		{
			case RendererAPI::API::None: HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return std::make_shared<OpenGLShader>(vertexSrc, fragmentSrc);
		}

		HZ_CORE_ASSERT(false, "Uknown RendererAPI!");
		return nullptr;
	}

}