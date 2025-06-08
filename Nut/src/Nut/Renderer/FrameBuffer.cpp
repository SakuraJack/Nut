#include "ntpch.h"
#include "FrameBuffer.h"
#include "Nut/Core/Application.h"
#include "Renderer.h"
#include "Nut/Utils/ImageUtils.h"

namespace Nut {

	Framebuffer::Framebuffer(const FramebufferSpecification& spec)
		: m_Specification(spec)
	{
		if (spec.Width == 0) {
			m_Width = Application::Get().GetWindow()->GetWidth();
			m_Height = Application::Get().GetWindow()->GetHeight();
		}
		else {
			m_Width = (uint32_t)(spec.Width * m_Specification.Scale);
			m_Height = (uint32_t)(spec.Height * m_Specification.Scale);
		}

		uint32_t attachmentIndex = 0;
		for (auto& attachmentSpec : m_Specification.Attachments.Attachments) {
			if (Utils::IsDepthFormat(attachmentSpec.Format)) {
				ImageSpecification spec;
				spec.Format = attachmentSpec.Format;
				spec.Usage = ImageUsage::Attachment;
				spec.Width = (uint32_t)(m_Width * m_Specification.Scale);
				spec.Height = (uint32_t)(m_Height * m_Specification.Scale);
				m_DepthAttachmentImage = Image2D::Create(spec);
			}
			else {
				ImageSpecification spec;
				spec.Format = attachmentSpec.Format;
				spec.Usage = ImageUsage::Attachment;
				spec.Width = (uint32_t)(m_Width * m_Specification.Scale);
				spec.Height = (uint32_t)(m_Height * m_Specification.Scale);
				m_AttachmentImages.emplace_back(Image2D::Create(spec));
			}
		}

		Resize(m_Width, m_Height, true);
	}

	Framebuffer::~Framebuffer()
	{
		Release();
	}

	void Framebuffer::Bind()
	{
		Renderer::Submit([this]() {
			glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferID);
			});
	}

	void Framebuffer::Unbind()
	{
		Renderer::Submit([this]() {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			});
	}

	void Framebuffer::Resize(uint32_t width, uint32_t height, bool forceRecreate)
	{
		if (!forceRecreate && (m_Width == width && m_Height == height)) return;

		Renderer::Submit([this, width, height]() mutable {
			m_Width = (uint32_t)(width * m_Specification.Scale);
			m_Height = (uint32_t)(height * m_Specification.Scale);
			if (!m_Specification.SwapChainTarget) {
				if (m_FramebufferID) {
					glDeleteFramebuffers(1, &m_FramebufferID);
				}

				uint32_t attachmentIndex = 0;
				for (std::shared_ptr<Image2D> image : m_AttachmentImages) {
					image->Release();
					glDeleteTextures(1, &image->GetImageID());
					attachmentIndex++;
				}

				if (m_DepthAttachmentImage) {
					m_DepthAttachmentImage->Release();
					glDeleteTextures(1, &m_DepthAttachmentImage->GetImageID());
				}

				glCreateFramebuffers(1, &m_FramebufferID);
				glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferID);

				std::vector<GLuint> colorAttachments(m_AttachmentImages.size());
				glCreateTextures(GL_TEXTURE_2D, (GLsizei)m_AttachmentImages.size(), colorAttachments.data());

				for (size_t i = 0; i < m_AttachmentImages.size(); i++) {
					auto& image = m_AttachmentImages[i];
					glBindTexture(GL_TEXTURE_2D, colorAttachments[i]);
					glTextureStorage2D(colorAttachments[i], image->GetSpecification().Mips, ImageUtils::ImageFormatToGLInternalFormat(image->GetSpecification().Format),
						image->GetSpecification().Width, image->GetSpecification().Height);
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + (GLenum)i, GL_TEXTURE_2D, colorAttachments[i], 0);
				}

				if (m_DepthAttachmentImage) {
					glCreateRenderbuffers(1, &m_DepthAttachmentImage->GetImageID());
					glRenderbufferStorage(GL_RENDERBUFFER, ImageUtils::ImageFormatToGLInternalFormat(m_DepthAttachmentImage->GetSpecification().Format),
						m_DepthAttachmentImage->GetSpecification().Width, m_DepthAttachmentImage->GetSpecification().Height);
					glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthAttachmentImage->GetImageID());
				}

			}
			});
	}

	void Framebuffer::AddResizeCallback(const std::function<void(std::shared_ptr<Framebuffer>)>& func)
	{
		m_ResizeCallbacks.push_back(func);
	}

	void Framebuffer::BindTexture(uint32_t attachmentIndex, uint32_t slot) const
	{

	}

	void Framebuffer::Invalidate()
	{
		Renderer::Submit([this]() {

			});
	}

	void Framebuffer::Release()
	{
		if (m_FramebufferID) {
			RenderID framebufferID = m_FramebufferID;
			Renderer::Submit([framebufferID]() {
				glDeleteFramebuffers(1, &framebufferID);
				});

			uint32_t attachmentIndex = 0;
			for (std::shared_ptr<Image2D> image : m_AttachmentImages) {
				image->Release();
				Renderer::Submit([image]() {
					glDeleteTextures(1, &image->GetImageID());
					});
				attachmentIndex++;
			}

			if (m_DepthAttachmentImage) {
				m_DepthAttachmentImage->Release();
				Renderer::Submit([depthImage = m_DepthAttachmentImage]() {
					glDeleteTextures(1, &depthImage->GetImageID());
					});
			}
		}
	}
	std::shared_ptr<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		return std::make_shared<Framebuffer>(spec);
	}
}