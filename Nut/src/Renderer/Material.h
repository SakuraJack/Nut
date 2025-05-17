#pragma once

#include "Shader.h"
#include "Texture.h"

namespace Nut {

	class Material
	{
	public:
		Material();
		Material(const std::shared_ptr<Shader>& shader, const std::string& name = "Default");
		virtual ~Material() = default;

		void Invalidate();

		void Bind();
		void Unbind();

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
		void Set(const std::string& name, const std::shared_ptr<Texture2D>& texture);
		void Set(const std::string& name, const std::shared_ptr<TextureCube>& texture);
		void Set(const std::string& name, const std::shared_ptr<Image2D>& image);

		static std::shared_ptr<Material> Create(const std::string& name = "Default");
		static std::shared_ptr<Material> Create(const std::shared_ptr<Shader>& shader, const std::string& name = "");
		static std::shared_ptr<Material> Copy(const std::shared_ptr<Material>& material, const std::string& name = "");

		std::shared_ptr<Shader> GetShader() const { return m_Shader; }	//  获取着色器

	private:
		std::shared_ptr<Shader> m_Shader;	//  着色器
		std::string m_Name;
		std::unordered_map<std::string, std::shared_ptr<Texture>> m_Textures;	//  纹理列表
		std::unordered_map<std::string, uint32_t> m_TextureSlots;	//  纹理槽列表
	};

}