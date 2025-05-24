#pragma once

#include "Shader.h"
#include "Texture.h"

#include "Asset/ResourceManager.h"

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

		template<typename T>
		void Set(const std::string& name, const T& value)
		{
			auto decl = FindUniformInUniformBuffer(name);
			if (decl) {
				auto& buffer = m_UniformBuffer;
				buffer.Write((byte*)&value, decl->GetSize(), decl->GetOffset());
			}
			decl = FindUniformInStorageBuffer(name);
			if (decl) {
				auto& buffer = m_StorageBuffer;
				buffer.Write((byte*)&value, decl->GetSize(), decl->GetOffset());
			}
		}

		template<typename T>
		T& Get(const std::string& name)
		{
			auto decl = FindUniformInUniformBuffer(name);
			if (decl) {
				auto& buffer = m_UniformBuffer;
				return buffer.Read<T>(decl->GetOffset());
			}
			decl = FindUniformInStorageBuffer(name);
			if (decl) {
				auto& buffer = m_StorageBuffer;
				return buffer.Read<T>(decl->GetOffset());
			}
		}

		template<typename T>
		std::shared_ptr<T> GetResource(const std::string& name)
		{
			return m_ResourceManager.GetInput<T>(name);
		}

		float& GetFloat(const std::string& name);
		int32_t& GetInt(const std::string& name);
		uint32_t& GetUInt(const std::string& name);
		bool& GetBool(const std::string& name);
		glm::vec2& GetVec2(const std::string& name);
		glm::vec3& GetVec3(const std::string& name);
		glm::vec4& GetVec4(const std::string& name);
		glm::ivec2& GetIVec2(const std::string& name);
		glm::ivec3& GetIVec3(const std::string& name);
		glm::ivec4& GetIVec4(const std::string& name);
		glm::mat3& GetMat3(const std::string& name);
		glm::mat4& GetMat4(const std::string& name);

		static std::shared_ptr<Material> Create(const std::string& name = "Default");
		static std::shared_ptr<Material> Create(const std::shared_ptr<Shader>& shader, const std::string& name = "");
		static std::shared_ptr<Material> Copy(const std::shared_ptr<Material>& material, const std::string& name = "");

		std::shared_ptr<Shader> GetShader() const { return m_Shader; }	//  获取着色器

	private:
		void Init();
		void Allocate();

		const ShaderUniform* FindUniformInUniformBuffer(const std::string& name);
		const ShaderUniform* FindUniformInStorageBuffer(const std::string& name);
		const ShaderResourceDeclaration* FindResourceInUniformBuffer(const std::string& name);

	private:
		std::shared_ptr<Shader> m_Shader;	//  着色器
		std::string m_Name;
		
		ResourceManager m_ResourceManager;	//  资源管理器
		Buffer m_UniformBuffer;
		Buffer m_StorageBuffer;
	};

}