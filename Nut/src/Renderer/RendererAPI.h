#pragma once
#include "glm/glm.hpp"

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
		static void Init();
		static void Shutdown();
		static void BeginFrame();
		static void EndFrame();
		static void Clear();
		static void Clear(float r, float g, float b, float a);
		static void Resize(uint32_t width, uint32_t height);

		// 改变渲染器状态
		static void SetViewport(int x, int y, int width, int height); // 设置视口
		static void SetClearColor(const glm::vec4& color); // 设置清除颜色
		static void SetClearDepth(float depth); // 设置清除深度
		static void SetClearStencil(int stencil); // 设置清除模板
		static void SetPolygonMode(PolygonMode mode); // 设置多边形模式
		static void SetLineWidth(float width); // 设置线宽
		static void SetPointSize(float size); // 设置点大小
		static void SetScissorEnabled(bool enable); // 是否启用剪裁器
		static void SetScissor(int x, int y, int width, int height); // 设置剪裁器
		static void SetBlendEnabled(bool enable); // 是否启用混合
		static void SetBlendFunc(int srcFactor, int dstFactor); // 设置混合函数
		static void SetDepthTestEnabled(bool enable); // 是否启用深度测试
		static void SetDepthFunc(DepthFunc func); // 设置深度函数
		static void SetStencilTestEnabled(bool enable); // 是否启用模板测试
		static void SetStencilFunc(StencilFunc func, int ref, int mask); // 设置模板函数
		static void SetStencilOp(int fail, int zfail, int zpass); // 设置模板操作
		static void SetFrontFace(FrontFace face); // 设置面朝向
		static void SetCullFace(bool enable); // 设置剔除面
		static void SetCullFaceMode(CullFaceMode mode); // 设置剔除面模式

		// 绘制图元
		static void DrawArrays(PrimitiveType mode, int first, int count); // 绘制数组
		static void DrawElements(PrimitiveType mode, int count, int type, const void* indices); // 绘制元素
		static void DrawElementsInstanced(PrimitiveType mode, int count, int type, const void* indices, int instanceCount); // 绘制实例化元素
		static void DrawArraysInstanced(PrimitiveType mode, int first, int count, int instanceCount); // 绘制实例化数组
		static void DrawArraysIndirect(PrimitiveType mode, const void* indirect); // 绘制间接数组
		static void DrawElementsIndirect(PrimitiveType mode, int count, int type, const void* indices, const void* indirect); // 绘制间接元素
		static void DrawArraysInstancedBaseInstance(PrimitiveType mode, int first, int count, int instanceCount, int baseInstance); // 绘制实例化数组基础实例

	protected:
		static RendererAPIStatus s_RendererAPIStatus; // 渲染器状态

	public:
		static RendererAPIStatus GetRendererAPIStatus() { return s_RendererAPIStatus; } // 获取渲染器状态
	};
}