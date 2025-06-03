#include "ntpch.h"
#include "RendererAPI.h"
#include "Nut/Tool/DebugTools.h"
#include "Renderer.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace Nut {
	RendererAPIStatus RendererAPI::s_RendererAPIStatus;
}

void Nut::RendererAPI::Init()
{
	NUT_CORE_INFO_TAG("Renderer", "初始化渲染器");
	auto spec = RendererAPISpecification();
	spec.RendererName = (const char*)glGetString(GL_RENDERER);
	spec.RendererVersion = (const char*)glGetString(GL_VERSION);
	spec.RendererVendor = (const char*)glGetString(GL_VENDOR);
	glGetIntegerv(GL_MAX_SAMPLES, &spec.MaxSampleCount);
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &spec.MaxTextureUnit);

	NUT_CORE_INFO_TAG("Renderer", "设备名称: {0}", spec.RendererName);
	NUT_CORE_INFO_TAG("Renderer", "渲染接口: OpenGL");
	NUT_CORE_INFO_TAG("Renderer", "版本: {0}", spec.RendererVersion);
	NUT_CORE_INFO_TAG("Renderer", "厂商: {0}", spec.RendererVendor);
	NUT_CORE_INFO_TAG("Renderer", "最大采样数: {0}", spec.MaxSampleCount);
	NUT_CORE_INFO_TAG("Renderer", "最大纹理单元: {0}", spec.MaxTextureUnit);

	OpenGLDebugTools::EnableOpenGLDebugMessage();
	NUT_CORE_INFO_TAG("Renderer", "启用OpenGL调试... 调试模式:同步");

	glClearColor(s_RendererAPIStatus.s_ClearColor.r, s_RendererAPIStatus.s_ClearColor.g, s_RendererAPIStatus.s_ClearColor.b, s_RendererAPIStatus.s_ClearColor.a);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	NUT_CORE_INFO_TAG("Renderer", "启用混合... 混合函数:源Alpha, 1-源Alpha");

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	NUT_CORE_INFO_TAG("Renderer", "启用深度测试... 深度函数:小于等于");

	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 0xFF, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	NUT_CORE_INFO_TAG("Renderer", "启用模板测试... 模板函数:总是通过, 模板操作:保持, 保持, 替换");

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	NUT_CORE_INFO_TAG("Renderer", "启用剔除面... 剔除面:背面, 正面朝向:逆时针");

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	NUT_CORE_INFO_TAG("Renderer", "启用多重采样... 启用无缝立方体贴图");


	GLenum err = glGetError();
	while (err != GL_NO_ERROR) {
		NUT_CORE_ERROR_TAG("Renderer", "OpenGL错误: {0}", err);
		err = glGetError();
	}
}

void Nut::RendererAPI::Shutdown()
{

}

void Nut::RendererAPI::BeginFrame()
{
	Clear();
}

void Nut::RendererAPI::EndFrame()
{
}

void Nut::RendererAPI::Clear()
{
	Renderer::Submit([]() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		});
}

void Nut::RendererAPI::Clear(float r, float g, float b, float a)
{
	if (r == s_RendererAPIStatus.s_ClearColor.x &&
		g == s_RendererAPIStatus.s_ClearColor.y &&
		b == s_RendererAPIStatus.s_ClearColor.z &&
		a == s_RendererAPIStatus.s_ClearColor.w)		return;

	Renderer::Submit([r, g, b, a]() {
		s_RendererAPIStatus.s_ClearColor = glm::vec4(r, g, b, a);
		glClearColor(r, g, b, a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		NUT_CORE_INFO_TAG("Renderer", "清除颜色缓冲区: {0}, {1}, {2}, {3}", r, g, b, a);
		});
}

void Nut::RendererAPI::Resize(uint32_t width, uint32_t height)
{
	if (width == 0 || height == 0)
		return;
	if (width == s_RendererAPIStatus.s_Viewport.z && height == s_RendererAPIStatus.s_Viewport.w)
		return;

	Renderer::Submit([width, height]() {
		s_RendererAPIStatus.s_Viewport.z = width;
		s_RendererAPIStatus.s_Viewport.w = height;
		glViewport(0, 0, width, height);
		NUT_CORE_INFO_TAG("Renderer", "调整视口大小: {0}, {1}", width, height);
		});
}

void Nut::RendererAPI::SetViewport(int x, int y, int width, int height)
{
	if (x == s_RendererAPIStatus.s_Viewport.x &&
		y == s_RendererAPIStatus.s_Viewport.y &&
		width == s_RendererAPIStatus.s_Viewport.z &&
		height == s_RendererAPIStatus.s_Viewport.w)
		return;

	Renderer::Submit([x, y, width, height]() {
		s_RendererAPIStatus.s_Viewport = glm::vec4(x, y, width, height);
		glViewport(x, y, width, height);
		NUT_CORE_INFO_TAG("Renderer", "设置视口: {0}, {1}, {2}, {3}", x, y, width, height);
		});
}

void Nut::RendererAPI::SetClearColor(const glm::vec4& color)
{
	if (color == s_RendererAPIStatus.s_ClearColor)
		return;

	Renderer::Submit([color]() {
		s_RendererAPIStatus.s_ClearColor = color;
		glClearColor(color.r, color.g, color.b, color.a);
		NUT_CORE_INFO_TAG("Renderer", "设置清除颜色: {0}, {1}, {2}, {3}", color.r, color.g, color.b, color.a);
		});
}

void Nut::RendererAPI::SetClearDepth(float depth)
{
	if (depth == s_RendererAPIStatus.s_ClearDepth)
		return;

	Renderer::Submit([depth]() {
		s_RendererAPIStatus.s_ClearDepth = depth;
		glClearDepth(depth);
		NUT_CORE_INFO_TAG("Renderer", "设置清除深度: {0}", depth);
		});
}

void Nut::RendererAPI::SetClearStencil(int stencil)
{
	if (stencil == s_RendererAPIStatus.s_ClearStencil)
		return;

	Renderer::Submit([stencil]() {
		s_RendererAPIStatus.s_ClearStencil = stencil;
		glClearStencil(stencil);
		NUT_CORE_INFO_TAG("Renderer", "设置清除模板: {0}", stencil);
		});
}

void Nut::RendererAPI::SetPolygonMode(PolygonMode mode)
{
	if (mode == s_RendererAPIStatus.s_PolygonMode)
		return;

	Renderer::Submit([mode]() {
		s_RendererAPIStatus.s_PolygonMode = mode;
		switch (mode)
		{
		case Nut::PolygonMode::None:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
		case Nut::PolygonMode::Fill:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
		case Nut::PolygonMode::Line:
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;
		case Nut::PolygonMode::Point:
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
			break;
		default:
			break;
		}
		NUT_CORE_INFO_TAG("Renderer", "设置多边形模式: {0}", (int)mode);
		});
}

void Nut::RendererAPI::SetLineWidth(float width)
{
	if (width == s_RendererAPIStatus.s_LineWidth)
		return;

	Renderer::Submit([width]() {
		s_RendererAPIStatus.s_LineWidth = width;
		glLineWidth(width);
		NUT_CORE_INFO_TAG("Renderer", "设置线宽: {0}", width);
		});
}

void Nut::RendererAPI::SetPointSize(float size)
{
	if (size == s_RendererAPIStatus.s_PointSize)
		return;

	Renderer::Submit([size]() {
		s_RendererAPIStatus.s_PointSize = size;
		glPointSize(size);
		NUT_CORE_INFO_TAG("Renderer", "设置点大小: {0}", size);
		});
}

void Nut::RendererAPI::SetScissorEnabled(bool enable)
{
	if (enable == s_RendererAPIStatus.s_ScissorEnabled)
		return;

	Renderer::Submit([enable]() {
		s_RendererAPIStatus.s_ScissorEnabled = enable;
		if (enable)
			glEnable(GL_SCISSOR_TEST);
		else
			glDisable(GL_SCISSOR_TEST);
		NUT_CORE_INFO_TAG("Renderer", "启用剪裁器: {0}", enable);
		});
}

void Nut::RendererAPI::SetScissor(int x, int y, int width, int height)
{
	if (x == s_RendererAPIStatus.s_Scissor.x &&
		y == s_RendererAPIStatus.s_Scissor.y &&
		width == s_RendererAPIStatus.s_Scissor.z &&
		height == s_RendererAPIStatus.s_Scissor.w)
		return;

	Renderer::Submit([x, y, width, height]() {
		s_RendererAPIStatus.s_Scissor = glm::vec4(x, y, width, height);
		glScissor(x, y, width, height);
		NUT_CORE_INFO_TAG("Renderer", "设置剪裁器: {0}, {1}, {2}, {3}", x, y, width, height);
		});
}

void Nut::RendererAPI::SetBlendEnabled(bool enable)
{
	if (enable == s_RendererAPIStatus.s_BlendEnabled)
		return;

	Renderer::Submit([enable]() {
		s_RendererAPIStatus.s_BlendEnabled = enable;
		if (enable)
			glEnable(GL_BLEND);
		else
			glDisable(GL_BLEND);
		NUT_CORE_INFO_TAG("Renderer", "启用混合: {0}", enable);
		});
}

void Nut::RendererAPI::SetBlendFunc(int srcFactor, int dstFactor)
{
	/*if (srcFactor == s_SrcFactor && dstFactor == s_DstFactor)
		return;
	s_SrcFactor = srcFactor;*/

	Renderer::Submit([srcFactor, dstFactor]() {
		glBlendFunc(srcFactor, dstFactor);
		NUT_CORE_INFO_TAG("Renderer", "设置混合函数: {0}, {1}", srcFactor, dstFactor);
		});
}

void Nut::RendererAPI::SetDepthTestEnabled(bool enable)
{
	if (enable == s_RendererAPIStatus.s_DepthTestEnabled)
		return;

	Renderer::Submit([enable]() {
		s_RendererAPIStatus.s_DepthTestEnabled = enable;
		if (enable)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);
		NUT_CORE_INFO_TAG("Renderer", "启用深度测试: {0}", enable);
		});
}

void Nut::RendererAPI::SetDepthFunc(DepthFunc func)
{
	if (func == s_RendererAPIStatus.s_DepthFunc)
		return;

	Renderer::Submit([func]() {
		s_RendererAPIStatus.s_DepthFunc = func;
		switch (func)
		{
		case Nut::DepthFunc::None:						glDepthFunc(GL_ALWAYS); break;
		case Nut::DepthFunc::Never:						glDepthFunc(GL_NEVER); break;
		case Nut::DepthFunc::Less:						glDepthFunc(GL_LESS); break;
		case Nut::DepthFunc::Equal:						glDepthFunc(GL_EQUAL); break;
		case Nut::DepthFunc::LessEqual:					glDepthFunc(GL_LEQUAL); break;
		case Nut::DepthFunc::Greater:					glDepthFunc(GL_GREATER); break;
		case Nut::DepthFunc::NotEqual:					glDepthFunc(GL_NOTEQUAL); break;
		case Nut::DepthFunc::GreaterEqual:				glDepthFunc(GL_GEQUAL); break;
		case Nut::DepthFunc::Always:					glDepthFunc(GL_ALWAYS); break;
		default:
			break;
		}
		NUT_CORE_INFO_TAG("Renderer", "设置深度函数: {0}", (int)func);
		});
}

void Nut::RendererAPI::SetStencilTestEnabled(bool enable)
{
	if (enable == s_RendererAPIStatus.s_StencilTestEnabled)
		return;

	Renderer::Submit([enable]() {
		s_RendererAPIStatus.s_StencilTestEnabled = enable;
		if (enable)
			glEnable(GL_STENCIL_TEST);
		else
			glDisable(GL_STENCIL_TEST);
		NUT_CORE_INFO_TAG("Renderer", "启用模板测试: {0}", enable);
		});
}

void Nut::RendererAPI::SetStencilFunc(StencilFunc func, int ref, int mask)
{
	/*if (func == s_StencilFunc && ref == s_StencilRef && mask == s_StencilMask)
		return;
	s_StencilFunc = func;*/
	Renderer::Submit([func, ref, mask]() {
		switch (func)
		{
		case Nut::StencilFunc::None:					glStencilFunc(GL_ALWAYS, ref, mask);  break;
		case Nut::StencilFunc::Never:					glStencilFunc(GL_NEVER, ref, mask);  break;
		case Nut::StencilFunc::Less:					glStencilFunc(GL_LESS, ref, mask);  break;
		case Nut::StencilFunc::Equal:					glStencilFunc(GL_EQUAL, ref, mask);  break;
		case Nut::StencilFunc::LessEqual:				glStencilFunc(GL_LEQUAL, ref, mask);  break;
		case Nut::StencilFunc::Greater:					glStencilFunc(GL_GREATER, ref, mask);  break;
		case Nut::StencilFunc::NotEqual:				glStencilFunc(GL_NOTEQUAL, ref, mask);  break;
		case Nut::StencilFunc::GreaterEqual: 			glStencilFunc(GL_GEQUAL, ref, mask);  break;
		case Nut::StencilFunc::Always: 					glStencilFunc(GL_ALWAYS, ref, mask);  break;
		default:
			break;
		}
		NUT_CORE_INFO_TAG("Renderer", "设置模板函数: {0}, {1}, {2}", (int)func, ref, mask);
		});
}

void Nut::RendererAPI::SetStencilOp(int fail, int zfail, int zpass)
{
	Renderer::Submit([fail, zfail, zpass]() {
		glStencilOp(fail, zfail, zpass);
		NUT_CORE_INFO_TAG("Renderer", "设置模板操作: {0}, {1}, {2}", fail, zfail, zpass);
		});
}

void Nut::RendererAPI::SetFrontFace(FrontFace face)
{
	if (face == s_RendererAPIStatus.s_FrontFace)
		return;

	Renderer::Submit([face]() {
		s_RendererAPIStatus.s_FrontFace = face;
		switch (face)
		{
		case Nut::FrontFace::None:						glFrontFace(GL_CCW); break;
		case Nut::FrontFace::CW:						glFrontFace(GL_CW); break;
		case Nut::FrontFace::CCW:						glFrontFace(GL_CCW); break;
		default:
			break;
		}
		NUT_CORE_INFO_TAG("Renderer", "设置正面朝向: {0}", (int)face);
		});
}

void Nut::RendererAPI::SetCullFace(bool enable)
{
	if (enable == s_RendererAPIStatus.s_CullFaceEnabled)
		return;

	Renderer::Submit([enable]() {
		s_RendererAPIStatus.s_CullFaceEnabled = enable;
		if (enable)
			glEnable(GL_CULL_FACE);
		else
			glDisable(GL_CULL_FACE);
		NUT_CORE_INFO_TAG("Renderer", "启用剔除面: {0}", enable);
		});
}

void Nut::RendererAPI::SetCullFaceMode(CullFaceMode mode)
{
	if (mode == s_RendererAPIStatus.s_CullFaceMode)
		return;

	Renderer::Submit([mode]() {
		s_RendererAPIStatus.s_CullFaceMode = mode;
		switch (mode)
		{
		case Nut::CullFaceMode::None:					glCullFace(GL_NONE); break;
		case Nut::CullFaceMode::Front:					glCullFace(GL_FRONT); break;
		case Nut::CullFaceMode::Back:					glCullFace(GL_BACK); break;
		case Nut::CullFaceMode::FrontAndBack:			glCullFace(GL_FRONT_AND_BACK); break;
		default:
			break;
		}
		NUT_CORE_INFO_TAG("Renderer", "设置剔除面模式: {0}", (int)mode);
		});
}

void Nut::RendererAPI::RenderStaticMesh(std::shared_ptr<Pipeline> pipeline, std::shared_ptr<StaticMesh> mesh, uint32_t submeshIndex, std::shared_ptr<MaterialTable> materialTable)
{
	Renderer::Submit([pipeline, mesh, submeshIndex, materialTable]() {
		std::shared_ptr<MeshSource> meshSource = mesh->GetMeshSource();
		glBindVertexArray(meshSource->GetVertexArray()->GetID());

		const auto& submeshes = meshSource->GetSubmeshes();
		const SubMesh& submesh = submeshes[submeshIndex];
		std::shared_ptr<MaterialTable> meshMaterialTable = mesh->GetMaterials();
		uint32_t materialCount = meshMaterialTable->GetMaterialCount();
		AssetHandle materialHandle = materialTable->HasMaterial(submesh.MaterialIndex ? materialTable->GetMaterial(submesh.MaterialIndex) : meshMaterialTable->GetMaterial(submesh.MaterialIndex));
		// TODO: 交给每个Project的AssetManager来处理
		//std::shared_ptr<MaterialAsset> materialAsset = AssetManager::GetAsset<MaterialAsset>(materialHandle);
		// TODO: Fix this
		/*std::shared_ptr<MaterialAsset> materialAsset = mesh->m_MaterialAssets[submeshIndex];
		std::shared_ptr<Material> material = materialAsset->GetMaterial();*/
		std::shared_ptr<Material> material = mesh->GetMeshSource()->GetMaterials()[submeshIndex];
		glUseProgram(material->GetShader()->GetShaderID());

		for (auto& [name, buffer] : material->GetUniformBuffers()) {
			RenderID id = material->GetShader()->GetUniformBuffers()[name].BufferID;
			glNamedBufferSubData(id, 0, (uint32_t)buffer.Size, buffer.Data);
		}
		for (auto& [name, buffer] : material->GetStorageBuffers()) {
			RenderID id = material->GetShader()->GetStorageBuffers()[name].BufferID;
			glNamedBufferSubData(id, 0, (uint32_t)buffer.Size, buffer.Data);
		}
		glDrawElements(GL_TRIANGLES, submesh.IndexCount, GL_UNSIGNED_INT, 0);
		});
}
