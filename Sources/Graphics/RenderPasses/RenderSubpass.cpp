#include "RenderSubpass.hpp"

RenderSubpass::RenderSubpass(const Handle<LogicalDevice>& logical_device,
							 const List<RenderAttachment::Descriptor>& input_attachments,
							 const List<RenderAttachment::Descriptor>& color_attachments,
							 const RenderAttachment::Descriptor& depth_stencil_attachment,
							 const List<RenderAttachment::Descriptor>& resolve_attachments,
							 const List<Handle<RenderAttachment>>& preserve_attachments)
	: _LogicalDevice(logical_device, this)
	, _InputAttachments(input_attachments)
	, _ColorAttachments(color_attachments)
	, _ResolveAttachments(resolve_attachments)
	, _DepthStencilAttachment(depth_stencil_attachment)
	, _PreserveAttachments(preserve_attachments) {
	NotifyCreation(this);

	if (_LogicalDevice.IsInvalid())
		throw std::runtime_error("RenderSubpass::RenderSubpass(): Logical device is null");

    if(_ResolveAttachments.Length() != 0 && _ResolveAttachments.Length() != _ColorAttachments.Length())
        throw std::runtime_error("RenderSubpass::RenderSubpass(): Different number of color and resolve attachments");

	_Attachments.Resize(_InputAttachments.Length() +
						_ColorAttachments.Length() +
						(_DepthStencilAttachment.IsValid() ? 1 : 0) +
						_ResolveAttachments.Length() +
						_PreserveAttachments.Length());

	uint32 attachment_id = 0;

	for (uint32 i = 0; i < _InputAttachments.Length(); i++)
		if (_InputAttachments[i].IsValid())
			_Attachments[attachment_id++].Set(_InputAttachments[i].Attachment, this);
		else
			throw std::runtime_error("RenderSubpass::RenderSubpass(): Input attachment is null");

	for (uint32 i = 0; i < _ColorAttachments.Length(); i++)
		if (_ColorAttachments[i].IsValid())
			_Attachments[attachment_id++].Set(_ColorAttachments[i].Attachment, this);
		else
			throw std::runtime_error("RenderSubpass::RenderSubpass(): Color attachment is null");

	if (_DepthStencilAttachment.IsValid())
		_Attachments[attachment_id++].Set(_DepthStencilAttachment.Attachment, this);

	for (uint32 i = 0; i < _ResolveAttachments.Length(); i++)
		if (_ResolveAttachments[i].IsValid())
			_Attachments[attachment_id++].Set(_ResolveAttachments[i].Attachment, this);
		else
			throw std::runtime_error("RenderSubpass::RenderSubpass(): Resolve attachment is null");

	for (uint32 i = 0; i < _PreserveAttachments.Length(); i++)
		if (_PreserveAttachments[i].IsValid())
			_Attachments[attachment_id++].Set(_PreserveAttachments[i], this);
		else
			throw std::runtime_error("RenderSubpass::RenderSubpass(): Preserve attachment is null");
}

RenderSubpass::~RenderSubpass() {
	Destroy();
	NotifyDestruction(this);
}

/* ---- ---- ---- ---- */