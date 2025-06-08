#pragma once

#include <glm/glm.hpp>
#include <map>

#include "Image.h"

namespace Nut {

	class Framebuffer;

	enum class FramebufferBlendMode
	{
		None = 0,
		OneZero,
		SrcAlphaOneMinusSrcAlpha,
		Additive,
		Zero_SrcColor
	};

	enum class AttachmentType
	{
		Clear = 0,
		Load
	};

	struct FramebufferTextureSpecification
	{
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(ImageFormat format) : Format(format) {}

		ImageFormat Format;
		bool Blend = true;
		FramebufferBlendMode BlendMode = FramebufferBlendMode::SrcAlphaOneMinusSrcAlpha;
		AttachmentType Attachment = AttachmentType::Load;
	};

	struct FramebufferAttachmentSpecification
	{
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(const std::initializer_list<FramebufferTextureSpecification>& attachments)
			: Attachments(attachments) {}

		std::vector<FramebufferTextureSpecification> Attachments;
	};

	// Stolen form Hazel
	struct FramebufferSpecification
	{
		float Scale = 1.0f;
		uint32_t Width = 0;
		uint32_t Height = 0;
		glm::vec4 ClearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
		float DepthClearValue = 0.0f;
		bool ClearColorOnLoad = true;
		bool ClearDepthOnLoad = true;

		FramebufferAttachmentSpecification Attachments;
		uint32_t Samples = 1; // multi-sampling

		bool NoResize = false;

		bool Blend = true;

		FramebufferBlendMode BlendMode = FramebufferBlendMode::SrcAlphaOneMinusSrcAlpha;

		// SwapChainTarget = screen buffer (i.e. no framebuffer)
		bool SwapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		Framebuffer(const FramebufferSpecification& spec);
		virtual ~Framebuffer();
		virtual void Bind();
		virtual void Unbind();

		virtual void Resize(uint32_t width, uint32_t height, bool forceRecreate = false);
		virtual void AddResizeCallback(const std::function<void(std::shared_ptr<Framebuffer>)>& func);

		virtual void BindTexture(uint32_t attachmentIndex = 0, uint32_t slot = 0) const;

		virtual uint32_t GetWidth() const { return m_Width; }
		virtual uint32_t GetHeight() const { return m_Height; }

		virtual RenderID GetFramebufferID() const { return m_FramebufferID; }

		virtual std::shared_ptr<Image2D> GetImage(uint32_t attachmentIndex = 0) { return m_AttachmentImages[attachmentIndex]; }
		virtual size_t GetColorAttachmentCount() const { return m_AttachmentImages.size(); }
		virtual bool HasDepthAttachment() const { return m_DepthAttachmentImage != nullptr; }
		virtual std::shared_ptr<Image2D> GetDepthImage() const { return m_DepthAttachmentImage;}

		virtual const FramebufferSpecification& GetSpecification() const { return m_Specification; }

		void Invalidate();
		void Release();

		static std::shared_ptr<Framebuffer> Create(const FramebufferSpecification& spec);

	private:
		FramebufferSpecification m_Specification;
		RenderID m_FramebufferID = 0;
		uint32_t m_Width = 0, m_Height = 0;

		std::vector<std::shared_ptr<Image2D>> m_AttachmentImages;
		std::shared_ptr<Image2D> m_DepthAttachmentImage;

		std::vector<std::function<void(std::shared_ptr<Framebuffer>)>> m_ResizeCallbacks;
	};
}