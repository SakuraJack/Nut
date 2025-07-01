#pragma once
#include "glm/glm.hpp"

#include "Pipeline.h"
#include "Mesh.h"
#include "Nut/Asset/MaterialAsset.h"

namespace Nut {
	enum class PrimitiveType
	{
		// 绘制图元类型
		None = 0,
		Triangles,
		TriangleStrip,
		TriangleFan,
		Lines,
		LineStrip,
		LineLoop,
		Point
	};

	enum class PolygonMode
	{
		// 多边形类型
		None = 0,
		Point,
		Line,
		Fill
	};

	enum class BlendFunc
	{
		// 混合函数
	};

	enum class DepthFunc
	{
		// 深度函数
		None = 0,
		Never,
		Less,
		Equal,
		LessEqual,
		Greater,
		NotEqual,
		GreaterEqual,
		Always
	};

	enum class StencilFunc
	{
		// 模板函数
		None = 0,
		Never,
		Less,
		Equal,
		LessEqual,
		Greater,
		NotEqual,
		GreaterEqual,
		Always
	};

	enum class CullFaceMode
	{
		// 剔除面模式
		None = 0,
		Front,
		Back,
		FrontAndBack
	};

	enum class FrontFace
	{
		// 面朝向
		None = 0,
		CW, // 顺时针
		CCW // 逆时针
	};


	struct RendererAPISpecification
	{
		// 渲染API规格
		std::string RendererName; // 渲染器名称
		std::string RendererVersion; // 渲染器版本
		std::string RendererVendor; // 渲染器厂商
		int MaxSampleCount = 0; // 最大采样数
		int MaxTextureUnit = 0; // 最大纹理单元
	};

	struct RendererAPIStatus
	{
		bool s_ScissorEnabled = false; // 剪裁器是否启用
		bool s_DepthTestEnabled = false; // 深度测试是否启用
		bool s_StencilTestEnabled = false; // 模板测试是否启用
		bool s_CullFaceEnabled = true; // 剔除面是否启用
		bool s_BlendEnabled = false; // 混合是否启用
		int s_LineWidth = 1.0; // 线宽
		int s_PointSize = 1.0; // 点大小
		PolygonMode s_PolygonMode = PolygonMode::Fill; // 多边形模式
		PrimitiveType s_PrimitiveType = PrimitiveType::Triangles; // 绘制图元类型
		// BlendFunc s_BlendFunc; // 混合函数
		DepthFunc s_DepthFunc = DepthFunc::Always; // 深度函数
		StencilFunc s_StencilFunc = StencilFunc::Always; // 模板函数
		CullFaceMode s_CullFaceMode = CullFaceMode::Back; // 剔除面模式
		FrontFace s_FrontFace = FrontFace::CCW; // 面朝向
		glm::vec4 s_ClearColor = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f); // 清除颜色
		glm::vec4 s_Viewport = glm::vec4(0, 0, 0, 0); // 视口
		glm::vec4 s_Scissor = glm::vec4(0, 0, 0, 0); // 剪裁器
		float s_ClearDepth = 0.0; // 清除深度
		int s_ClearStencil = 0; // 清除模板
	};

	class RendererAPI
	{
	public:
		void Init();
		void Shutdown();
		void BeginFrame();
		void EndFrame();
		void Clear();
		void Clear(float r, float g, float b, float a);
		void Resize(uint32_t width, uint32_t height);

		// 改变渲染器状态
		void SetViewport(int x, int y, int width, int height); // 设置视口
		void SetClearColor(const glm::vec4& color); // 设置清除颜色
		void SetClearDepth(float depth); // 设置清除深度
		void SetClearStencil(int stencil); // 设置清除模板
		void SetPolygonMode(PolygonMode mode); // 设置多边形模式
		void SetLineWidth(float width); // 设置线宽
		void SetPointSize(float size); // 设置点大小
		void SetScissorEnabled(bool enable); // 是否启用剪裁器
		void SetScissor(int x, int y, int width, int height); // 设置剪裁器
		void SetBlendEnabled(bool enable); // 是否启用混合
		void SetBlendFunc(int srcFactor, int dstFactor); // 设置混合函数
		void SetDepthTestEnabled(bool enable); // 是否启用深度测试
		void SetDepthFunc(DepthFunc func); // 设置深度函数
		void SetStencilTestEnabled(bool enable); // 是否启用模板测试
		void SetStencilFunc(StencilFunc func, int ref, int mask); // 设置模板函数
		void SetStencilOp(int fail, int zfail, int zpass); // 设置模板操作
		void SetFrontFace(FrontFace face); // 设置面朝向
		void SetCullFace(bool enable); // 设置剔除面
		void SetCullFaceMode(CullFaceMode mode); // 设置剔除面模式

		// 绘制图元
		void RenderStaticMesh(std::shared_ptr<Pipeline> pipeline, std::shared_ptr<StaticMesh> mesh, std::shared_ptr<MeshSource> meshSource, uint32_t submeshIndex, std::shared_ptr<MaterialTable> materialTable, std::shared_ptr<VertexBuffer> transformBuffer, uint32_t transformOffset, uint32_t instanceCount);
		void RenderSubmeshInstanced(std::shared_ptr<Pipeline> pipeline, std::shared_ptr<Mesh> mesh, std::shared_ptr<MeshSource> meshSource, uint32_t submeshIndex, std::shared_ptr<MaterialTable> materialTable, std::shared_ptr<VertexBuffer> transformBuffer, uint32_t transformOffset, uint32_t boneTransformsOffset, uint32_t instanceCount);
		void RenderMeshWithMaterial(std::shared_ptr<Pipeline> pipeline, std::shared_ptr<Mesh> mesh, std::shared_ptr<MeshSource> meshSource, uint32_t submeshIndex, std::shared_ptr<VertexBuffer> transformBuffer, uint32_t transformOffset, uint32_t instanceCount, std::shared_ptr<Material> material, Buffer additionalUniforms = Buffer());
		void RenderStaticMeshWithMaterial(std::shared_ptr<Pipeline> pipeline, std::shared_ptr<StaticMesh> mesh, std::shared_ptr<MeshSource> meshSource, uint32_t submeshIndex, std::shared_ptr<VertexBuffer> transformBuffer, uint32_t transformOffset, uint32_t instanceCount, std::shared_ptr<Material> material, Buffer additionalUniforms = Buffer());
		void RenderQuad(std::shared_ptr<Pipeline> pipeline, std::shared_ptr<Material> material, const glm::mat4& transform);
		void SubmitFullscreenQuad(std::shared_ptr<Pipeline> pipeline, std::shared_ptr<Material> material);
		void SubmitFullscreenQuadWithOverrides(std::shared_ptr<Pipeline> pipeline, std::shared_ptr<Material> material, Buffer vertexShaderOverrides, Buffer fragmentShaderOverrides);
		// void LightCulling(std::shared_ptr<ComputePass> computePass, std::shared_ptr<Material> material, const glm::uvec3& workGroups);
		void RenderGeometry(std::shared_ptr<Pipeline> pipeline, std::shared_ptr<Material> material, std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer, const glm::mat4& transform, uint32_t indexCount = 0);
		void SubmitQuad(std::shared_ptr<Material> material, const glm::mat4& transform = glm::mat4(1.0f));

	protected:
		static RendererAPIStatus s_RendererAPIStatus; // 渲染器状态

	public:
		static RendererAPIStatus GetRendererAPIStatus() { return s_RendererAPIStatus; } // 获取渲染器状态
	};
}