#pragma once

#include "Utils/Utils.hpp"

#include "External/Vulkan.hpp"

class TextureView : public Object {
public:
	static Handle<TextureView> Create(const Handle<LogicalDevice>& logicalDevice,
									  const Handle<ITexture>& itexture,
									  VkImageViewType type,
									  VkFormat format,
									  VkImageAspectFlags aspect,
									  uint32 base_mip_level = 0,
									  uint32 mip_level_count = 1,
									  uint32 base_array_layer = 0,
									  uint32 array_layer_count = 1) {
		return new TextureView(logicalDevice, itexture, type, format, aspect, base_mip_level, mip_level_count, base_array_layer, array_layer_count);
	}

private:
	TextureView(const Handle<LogicalDevice>& logicalDevice,
				const Handle<ITexture>& itexture,
				VkImageViewType type,
				VkFormat format,
				VkImageAspectFlags aspect,
				uint32 base_mip_level,
				uint32 mip_level_count,
				uint32 base_array_layer,
				uint32 array_layer_count);

public:
	~TextureView() override;

	/* ---- ---- ---- ---- */

    VkImageView GetVKImageView() const { return _VKImageView; }

	/* ---- ---- ---- ---- */

	const ParentHandle<LogicalDevice>& GetLogicalDevice() const { return _LogicalDevice; }
	const ParentHandle<ITexture>& GetITexture() const { return _ITexture; }

private:
	ParentHandle<LogicalDevice> _LogicalDevice;
	ParentHandle<ITexture> _ITexture;

	/* ---- ---- ---- ---- */

	VkImageView _VKImageView;
};