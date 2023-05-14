#pragma once

#include <stdexcept>
#include <type_traits>

#define interface class

using uint8 = unsigned char;
using uint16 = unsigned short;
using uint32 = unsigned int;
using uint64 = unsigned long long;

using int8 = signed char;
using int16 = signed short;
using int32 = signed int;
using int64 = signed long long;

constexpr uint32 INVALID_ID = -1;

/* ---- ---- ---- ---- */

class GraphicsContext;

class Buffer;
class BufferView;
interface IBuffer;

class CommandBuffer;
class CommandBufferPool;

class CommandQueue;
class CommandQueueFamily;

class DescriptorLayout;
class DescriptorPool;
class DescriptorSet;

class Instance;
class LogicalDevice;
class PhysicalDevice;

class Memory;
class MemoryHeap;
class MemoryType;

class ComputePipeline;
class GraphicsPipeline;
interface IPipeline;
class PipelineLayout;
class Shader;

class FrameBuffer;
class RenderAttachment;
class RenderDependency;
class RenderPass;
class RenderSubpass;

class Fence;
class Semaphore;

interface ITexture;
class Sampler;
class Texture;
class Texture1D;
class Texture2D;
class Texture3D;
class TextureView;

class Monitor;
class Swapchain;
class SwapchainTexture;
class Window;
class WindowContext;
class WindowSurface;

/* ---- ---- ---- ---- */

template <typename T>
struct is_complete_v {
	template <typename U>
	static auto test(U*) -> std::integral_constant<bool, sizeof(U) == sizeof(U)>;
	static auto test(...) -> std::false_type;
	using type = decltype(test((T*)0));
};

template <typename T>
struct is_complete : is_complete_v<T>::type {};

/* ---- ---- ---- ---- */

class Uncopyable {
protected:
	Uncopyable() = default;
	~Uncopyable() = default;

private:
	Uncopyable(const Uncopyable&) = delete;
	Uncopyable& operator=(const Uncopyable&) = delete;
};

class Unmovable {
protected:
	Unmovable() = default;
	~Unmovable() = default;

private:
	Unmovable(Unmovable&&) = delete;
	Unmovable& operator=(Unmovable&&) = delete;
};

class Fixed : public Uncopyable, public Unmovable {
protected:
	Fixed() = default;
	~Fixed() = default;
};

class Static : public Uncopyable, public Unmovable {
private:
	Static() = delete;
};

/* ---- ---- ---- ---- */

class Utils : Static {
public:
	template <class...>
	class Object;

	template <class>
	class Handle;

private:
	class THandle;
	class TLink;
	class TObject;

	template <class>
	class Parent;

	template <class>
	class ParentType;

	template <class>
	class Link;

public:
	template <class P, class O>
	static const typename ParentType<P>::Type GetParent(O* parent);
	template <class P, class O>
	static const typename ParentType<P>::Type GetParent(const Handle<O>& parent);
};

template <class... P>
using Object = Utils::Object<P...>;

template <class T>
using Handle = Utils::Handle<T>;
