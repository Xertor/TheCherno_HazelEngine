#include "hzpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Hazel
{

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_windowHandle(windowHandle)
	{
		HZ_CORE_ASSERT(m_windowHandle, "Failed to initialize window handle!");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_windowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		HZ_CORE_ASSERT(status, "Failed to initialize Glad!");
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_windowHandle);
	}
}