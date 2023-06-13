#pragma once

#include <stdexcept>
#include <string>
#include <cstdint>

using int8 = int8_t;
using int16 = int16_t;
using int32 = int32_t;
using int64 = int64_t;

using uint8 = uint8_t;
using uint16 = uint16_t;
using uint32 = uint32_t;
using uint64 = uint64_t;

constexpr char ENGINE_NAME[] = "Sulfur";
#define SULFUR_MAKE_API_VERSION(variant, major, minor, patch) \
	((((uint32_t)(variant)) << 29U) | (((uint32_t)(major)) << 22U) | (((uint32_t)(minor)) << 12U) | ((uint32_t)(patch)))
constexpr uint32 ENGINE_VERSION = SULFUR_MAKE_API_VERSION(0, 1, 0, 0);

constexpr uint32 INVALID_ID = 0xFFFFFFFFU;
constexpr uint64 INVALID_ID_64 = 0xFFFFFFFFFFFFFFFFULL;

#define interface class

/* ---- ---- ---- ---- */

class TObject;
class Object;
class THandle;
template <class T>
class Handle;

/* ---- ---- ---- ---- */

class Unmovable {
protected:
	Unmovable() = default;
	~Unmovable() = default;

	Unmovable(const Unmovable&) = default;
	Unmovable& operator=(const Unmovable&) = default;

	/* ---- ---- ---- ---- */
public:
	Unmovable(Unmovable&&) = delete;
	Unmovable& operator=(Unmovable&&) = delete;
};

class Uncopyable {
protected:
	Uncopyable() = default;
	~Uncopyable() = default;

	/* ---- ---- ---- ---- */
public:
	Uncopyable(const Uncopyable&) = delete;
	Uncopyable& operator=(const Uncopyable&) = delete;
};

class Static : public Uncopyable, public Unmovable {
protected:
	Static() = delete;
	~Static() = delete;
};

class Fixed : public Uncopyable, public Unmovable {
protected:
	Fixed() = default;
	~Fixed() = default;
};

/* ---- ---- ---- ---- */

class GraphicsContext;

class WindowContext;
class Window;
class Monitor;
class WindowSurface;
class Swapchain;

class Instance;
class PhysicalDevice;
class LogicalDevice;

class CommandQueue;
class CommandQueueFamily;

class CommandBuffer;
class CommandPool;

class Memory;
class MemoryHeap;
class MemoryType;

class RenderAttachment;
class RenderPass;
class Framebuffer;
class RenderSubpass;
class RenderDependency;

class DescriptorPool;
class DescriptorSet;
class DescriptorLayout;

class Semaphore;
class Fence;

interface IResource;

interface IBuffer;
class Buffer;

interface ITexture;
class TextureView;

class SwapchainTexture;

interface IPipeline;
class PipelineLayout;
class GraphicsPipeline;
class Shader;


enum AllocationType {
	PREFER_LARGEST,
	PREFER_DEVICE_LOCAL,
	PREFER_HOST_VISIBLE,
	PREFER_HOST_VISIBLE_COHERENT,
	PREFER_HOST_VISIBLE_CACHED,
};
