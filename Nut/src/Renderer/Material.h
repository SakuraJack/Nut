#pragma once

#include "Shader.h"

namespace Nut {
	
	class Material
	{
	public:
		static std::shared_ptr<Material> Create(const std::shared_ptr<Shader>& shader, const std::string& name = "");
		static std::shared_ptr<Material> Copy(const std::shared_ptr<Material>& material, const std::string& name = "");
		virtual ~Material() = default;

		void Set(const std::string& name, int value);
		void Set(const std::string& name, float value);
		void Set(const std::string& name, const glm::vec2& value);
		void Set(const std::string& name, const glm::vec3& value);
		void Set(const std::string& name, const glm::vec4& value);
		void Set(const std::string& name, const glm::ivec2& value);
		void Set(const std::string& name, const glm::ivec3& value);
		void Set(const std::string& name, const glm::ivec4& value);
		void Set(const std::string& name, const glm::mat3& value);
		void Set(const std::string& name, const glm::mat4& value);

		// TODO: 添加纹理Set函数
		// void Set(const std::string& name, const std::shared_ptr<Texture2D>& texture);
		// void Set(const std::string& name, const std::shared_ptr<TextureCube>& texture);
		// void Set(const std::string& name, const std::shared_ptr<Image2D>& image);

		float GetFloat(const std::string& name) const;
		int GetInt(const std::string& name) const;
		glm::vec2& GetVec2(const std::string& name) const;
		glm::vec3& GetVec3(const std::string& name) const;
		glm::vec4& GetVec4(const std::string& name) const;
		glm::ivec2& GetIVec2(const std::string& name) const;
		glm::ivec3& GetIVec3(const std::string& name) const;
		glm::ivec4& GetIVec4(const std::string& name) const;
		glm::mat3& GetMat3(const std::string& name) const;
		glm::mat4& GetMat4(const std::string& name) const;

		// TODO: 添加纹理Get函数
		// std::shared_ptr<Texture2D> GetTexture2D(const std::string& name) const;
		// std::shared_ptr<TextureCube> GetTextureCube(const std::string& name) const;
		// std::shared_ptr<Image2D> GetImage2D(const std::string& name) const;

		std::shared_ptr<Shader> GetShader() const { return m_Shader; }	//  获取着色器

	private:
		std::shared_ptr<Shader> m_Shader;	//  着色器
	};

}