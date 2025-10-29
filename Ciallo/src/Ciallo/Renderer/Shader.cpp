#include "clpch.h"
#include "Shader.h"
#include "Renderer.h"
#include "Platform/OpneGL/OpenGLShader.h"

namespace Ciallo
{
	//Shader
	Ref<Shader> Shader::Create(const std::string& name, const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::NONE: HZ_CORE_ASSERT(false, "NONE IS NOT SUPPORT!"); return nullptr;
		case RendererAPI::API::OpenGL: return std::make_shared<OpenGLShader>(name,filepath); break;
		}

		HZ_CORE_ASSERT(false, "Unknow API!!");
		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::NONE: HZ_CORE_ASSERT(false, "NONE IS NOT SUPPORT!"); return nullptr;
		case RendererAPI::API::OpenGL: return std::make_shared<OpenGLShader>(filepath);
		}

		HZ_CORE_ASSERT(false, "Unknow API!!");
		return nullptr;
	}


	void ShaderLibrary::Add(const std::string name, const Ref<Shader>& shader)
	{
		HZ_CORE_ASSERT(Exist(name), "Shader already exists!");
		m_Shaders[name] = shader;
	}

	//ShaderLibrary
	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		auto& name = shader->GetName();
		Add(name, shader);
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(name, shader);

		return shader;
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& name)
	{
		HZ_CORE_ASSERT(Exist(name), "Shader already exists!");
		return m_Shaders[name];
	}

	bool ShaderLibrary::Exist(const std::string& name)
	{
		return m_Shaders.find(name) == m_Shaders.end();
	}

}