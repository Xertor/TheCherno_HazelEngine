#include "hzpch.h"
#include "Renderer/Texture.h"

#include "Renderer.h"
#include "Hazel/Platform/OpenGL/OpenGLTexture.h"

namespace Hazel
{
	Hazel::Ref<Hazel::Texture2D> Texture2D::Create(const std::string& path)
	{
		switch(Renderer::GetAPI())
		{
			case RendererAPI::API::None: HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return std::make_shared<OpenGLTexture2D>(path);
		}

		HZ_CORE_ASSERT(false, "Uknown RendererAPI!");
		return nullptr;
	}
}