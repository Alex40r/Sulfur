#include "TextureView.hpp"

#include "Graphics/Devices/LogicalDevice.hpp"
#include "ITexture.hpp"

TextureView::TextureView(const Handle<LogicalDevice>& logicalDevice,
						 const Handle<ITexture>& itexture,
						 VkImageViewType type,
						 VkFormat format,
						 VkImageAspectFlags aspect,
						 uint32 base_mip_level,
						 uint32 mip_level_count,
						 uint32 base_array_layer,
						 uint32 array_layer_count)
	: _LogicalDevice(logicalDevice, this)
	, _ITexture(itexture, this) {
	NotifyCreation(this);

    if(_LogicalDevice.IsInvalid())
        throw std::runtime_error("TextureView::TextureView(): Logical device is null");

    if(_ITexture.IsInvalid())
        throw std::runtime_error("TextureView::TextureView(): Texture is null");

    VkImageViewCreateInfo vk_image_view_creation_info{};
    vk_image_view_creation_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    vk_image_view_creation_info.image = _ITexture->GetVKImage();
    vk_image_view_creation_info.viewType = type;
    vk_image_view_creation_info.format = format;
    vk_image_view_creation_info.subresourceRange.aspectMask = aspect;
    vk_image_view_creation_info.subresourceRange.baseMipLevel = base_mip_level;
    vk_image_view_creation_info.subresourceRange.levelCount = mip_level_count;
    vk_image_view_creation_info.subresourceRange.baseArrayLayer = base_array_layer;
    vk_image_view_creation_info.subresourceRange.layerCount = array_layer_count;

    VkResult vk_result = vkCreateImageView(_LogicalDevice->GetVKLogicalDevice(), &vk_image_view_creation_info, nullptr, &_VKImageView);
    if(vk_result != VK_SUCCESS)
        throw std::runtime_error("TextureView::TextureView(): Failed to create image view");
}

TextureView::~TextureView() {
	Destroy();
	NotifyDestruction(this);

    vkDestroyImageView(_LogicalDevice->GetVKLogicalDevice(), _VKImageView, nullptr);
}

/* ---- ---- ---- ---- */