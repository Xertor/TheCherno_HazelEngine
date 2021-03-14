#pragma once

#include "Hazel/Renderer/Framebuffer.h"

namespace Hazel
{
	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FrameBufferSpecification& spec);
		virtual ~OpenGLFramebuffer();

		void Invalidate();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void Resize(uint32_t width, uint32_t height) override;

		virtual uint32_t GetColorAttachmentRendererID() const { return m_ColorAttachement; }

		virtual const FrameBufferSpecification& GetSpecification() const override { return m_Specification; }
	private:
		uint32_t m_RendererID = 0;
		uint32_t m_ColorAttachement = 0, m_DepthAttachment = 0;
		FrameBufferSpecification m_Specification;
	};
}