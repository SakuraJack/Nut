#pragma once
#include "glm/glm.hpp"

namespace Nut {
	enum class PrimitiveType
	{
		// ����ͼԪ����
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
		// ���������
		None = 0,
		Point,
		Line,
		Fill
	};

	enum class BlendFunc
	{
		// ��Ϻ���
	};

	enum class DepthFunc
	{
		// ��Ⱥ���
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
		// ģ�庯��
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
		// �޳���ģʽ
		None = 0,
		Front,
		Back,
		FrontAndBack
	};

	enum class FrontFace
	{
		// �泯��
		None = 0,
		CW, // ˳ʱ��
		CCW // ��ʱ��
	};


	struct RendererAPISpecification
	{
		// ��ȾAPI���
		std::string RendererName; // ��Ⱦ������
		std::string RendererVersion; // ��Ⱦ���汾
		std::string RendererVendor; // ��Ⱦ������
		int MaxSampleCount = 0; // ��������
		int MaxTextureUnit = 0; // �������Ԫ
	};

	struct RendererAPIStatus
	{
		bool s_ScissorEnabled = false; // �������Ƿ�����
		bool s_DepthTestEnabled = false; // ��Ȳ����Ƿ�����
		bool s_StencilTestEnabled = false; // ģ������Ƿ�����
		bool s_CullFaceEnabled = true; // �޳����Ƿ�����
		bool s_BlendEnabled = false; // ����Ƿ�����
		int s_LineWidth = 1.0; // �߿�
		int s_PointSize = 1.0; // ���С
		PolygonMode s_PolygonMode = PolygonMode::Fill; // �����ģʽ
		PrimitiveType s_PrimitiveType = PrimitiveType::Triangles; // ����ͼԪ����
		// BlendFunc s_BlendFunc; // ��Ϻ���
		DepthFunc s_DepthFunc = DepthFunc::Always; // ��Ⱥ���
		StencilFunc s_StencilFunc = StencilFunc::Always; // ģ�庯��
		CullFaceMode s_CullFaceMode = CullFaceMode::Back; // �޳���ģʽ
		FrontFace s_FrontFace = FrontFace::CCW; // �泯��
		glm::vec4 s_ClearColor = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f); // �����ɫ
		glm::vec4 s_Viewport = glm::vec4(0, 0, 0, 0); // �ӿ�
		glm::vec4 s_Scissor = glm::vec4(0, 0, 0, 0); // ������
		float s_ClearDepth = 0.0; // ������
		int s_ClearStencil = 0; // ���ģ��
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

		// �ı���Ⱦ��״̬
		static void SetViewport(int x, int y, int width, int height); // �����ӿ�
		static void SetClearColor(const glm::vec4& color); // ���������ɫ
		static void SetClearDepth(float depth); // ����������
		static void SetClearStencil(int stencil); // �������ģ��
		static void SetPolygonMode(PolygonMode mode); // ���ö����ģʽ
		static void SetLineWidth(float width); // �����߿�
		static void SetPointSize(float size); // ���õ��С
		static void SetScissorEnabled(bool enable); // �Ƿ����ü�����
		static void SetScissor(int x, int y, int width, int height); // ���ü�����
		static void SetBlendEnabled(bool enable); // �Ƿ����û��
		static void SetBlendFunc(int srcFactor, int dstFactor); // ���û�Ϻ���
		static void SetDepthTestEnabled(bool enable); // �Ƿ�������Ȳ���
		static void SetDepthFunc(DepthFunc func); // ������Ⱥ���
		static void SetStencilTestEnabled(bool enable); // �Ƿ�����ģ�����
		static void SetStencilFunc(StencilFunc func, int ref, int mask); // ����ģ�庯��
		static void SetStencilOp(int fail, int zfail, int zpass); // ����ģ�����
		static void SetFrontFace(FrontFace face); // �����泯��
		static void SetCullFace(bool enable); // �����޳���
		static void SetCullFaceMode(CullFaceMode mode); // �����޳���ģʽ

		// ����ͼԪ
		static void DrawArrays(PrimitiveType mode, int first, int count); // ��������
		static void DrawElements(PrimitiveType mode, int count, int type, const void* indices); // ����Ԫ��
		static void DrawElementsInstanced(PrimitiveType mode, int count, int type, const void* indices, int instanceCount); // ����ʵ����Ԫ��
		static void DrawArraysInstanced(PrimitiveType mode, int first, int count, int instanceCount); // ����ʵ��������
		static void DrawArraysIndirect(PrimitiveType mode, const void* indirect); // ���Ƽ������
		static void DrawElementsIndirect(PrimitiveType mode, int count, int type, const void* indices, const void* indirect); // ���Ƽ��Ԫ��
		static void DrawArraysInstancedBaseInstance(PrimitiveType mode, int first, int count, int instanceCount, int baseInstance); // ����ʵ�����������ʵ��

	protected:
		static RendererAPIStatus s_RendererAPIStatus; // ��Ⱦ��״̬

	public:
		static RendererAPIStatus GetRendererAPIStatus() { return s_RendererAPIStatus; } // ��ȡ��Ⱦ��״̬
	};
}