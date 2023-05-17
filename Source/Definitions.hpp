#pragma once

#include <stdexcept>
#include <type_traits>

using uint8 = unsigned char;
using uint16 = unsigned short;
using uint32 = unsigned int;
using uint64 = unsigned long long;

using int8 = signed char;
using int16 = signed short;
using int32 = signed int;
using int64 = signed long long;

constexpr uint32 INVALID_ID = 0xFFFFFFFF;

#define interface class

/* ---- ---- ---- ---- */

interface IGraphicsContext;
class GraphicsContext;

interface IWindowContext;
class WindowContext;
interface IMonitor;
class Monitor;
interface IWindow;
class Window;

interface IInstance;
class Instance;
interface IPhysicalDevice;
class PhysicalDevice;
interface ILogicalDevice;
class LogicalDevice;

interface ICommandQueue;
class CommandQueue;
interface ICommandQueueFamily;
class CommandQueueFamily;

interface IMemoryHeap;
class MemoryHeap;
interface IMemoryType;
class MemoryType;

interface IRenderAttachment;
class RenderAttachment;
interface IRenderSubpass;
class RenderSubpass;
interface IRenderDependency;
class RenderDependency;
interface IRenderPass;
class RenderPass;

/* ---- ---- ---- ---- */

class TObject;
class THandle;
class TLink;
class Object;

template <class T>
class Handle;
template <class P>
class Parent;
template <class P>
class Link;

/* ---- ---- ---- ---- */

class Unmovable {
protected:
	Unmovable() = default;
	~Unmovable() = default;

public:
	Unmovable(Unmovable&&) = delete;
	Unmovable& operator=(Unmovable&&) = delete;
};

class Uncopyable {
protected:
	Uncopyable() = default;
	~Uncopyable() = default;

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
