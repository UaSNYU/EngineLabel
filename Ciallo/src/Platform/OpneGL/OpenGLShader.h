#pragma once

#include "Ciallo/Renderer/Shader.h"
#include "glm/glm.hpp"
#include <string>


namespace Ciallo
{
	struct ShaderSource
	{
		std::string VertexSource;
		std::string FragmentSource;
	};


	class OpenGLShader:public Shader
	{
	private:
		std::string ReadFile(const std::string& FilePath);
		std::unordered_map<unsigned int,std::string> PreProcess(const std::string& source);
		void Compile(const std::unordered_map<unsigned int, std::string> shadersources);

	public:
		OpenGLShader(const std::string& Filepath);
		OpenGLShader(const std::string& name, const std::string& Filepath);
		~OpenGLShader();

		virtual std::string GetName()override { return m_name; }

		void UploadUniform4fv(const std::string& name, const glm::mat4& matrix) const;
		void UploadUniform4f(const std::string& name, const glm::vec4& values)const;
		void UploadUniform3f(const std::string& name, const glm::vec3& values)const;
		void UploadUniform2f(const std::string& name, const glm::vec2& values)const;
		void UploadUniformfloat(const std::string& name, const float values)const;
		void UploadUniformInt(const std::string& name, const int values)const;

		virtual void Bind() override;
		virtual void UnBind()override;
	private:
		uint32_t m_RendererID;
		std::string m_name;
	};
}