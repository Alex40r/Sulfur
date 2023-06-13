#pragma once

#include "Utils/Utils.hpp"

#include "RenderAttachment.hpp"

class RenderSubpass : public Object {
public:
	static Handle<RenderSubpass> Create(const Handle<LogicalDevice>& logical_device,
										const List<RenderAttachment::Descriptor>& input_attachments,
										const List<RenderAttachment::Descriptor>& color_attachments,
										const RenderAttachment::Descriptor& depth_stencil_attachment,
										const List<RenderAttachment::Descriptor>& resolve_attachments = {},
										const List<Handle<RenderAttachment>>& preserve_attachments = {}) {
		return new RenderSubpass(logical_device, input_attachments, color_attachments, depth_stencil_attachment, resolve_attachments, preserve_attachments);
	}

private:
	RenderSubpass(const Handle<LogicalDevice>& logical_device,
				  const List<RenderAttachment::Descriptor>& input_attachments,
				  const List<RenderAttachment::Descriptor>& color_attachments,
				  const RenderAttachment::Descriptor& depth_stencil_attachment,
				  const List<RenderAttachment::Descriptor>& resolve_attachments,
				  const List<Handle<RenderAttachment>>& preserve_attachments);

public:
	~RenderSubpass() override;

	/* ---- ---- ---- ---- */

	const List<RenderAttachment::Descriptor>& GetInputAttachments() const { return _InputAttachments; }
	const List<RenderAttachment::Descriptor>& GetColorAttachments() const { return _ColorAttachments; }
	const List<RenderAttachment::Descriptor>& GetResolveAttachments() const { return _ResolveAttachments; }

	const RenderAttachment::Descriptor& GetDepthStencilAttachment() const { return _DepthStencilAttachment; }

	const List<Handle<RenderAttachment>>& GetPreserveAttachments() const { return _PreserveAttachments; }

	/* ---- ---- ---- ---- */

	const ParentHandle<LogicalDevice>& GetLogicalDevice() const { return _LogicalDevice; }

private:
	ParentHandle<LogicalDevice> _LogicalDevice;
	List<ParentHandle<RenderAttachment>> _Attachments;

	/* ---- ---- ---- ---- */

	List<RenderAttachment::Descriptor> _InputAttachments;
	List<RenderAttachment::Descriptor> _ColorAttachments;
	List<RenderAttachment::Descriptor> _ResolveAttachments;

	RenderAttachment::Descriptor _DepthStencilAttachment;

	List<Handle<RenderAttachment>> _PreserveAttachments;
};