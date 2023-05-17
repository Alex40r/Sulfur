#pragma once

#include "Definitions.hpp"

class TObject : public Fixed {
	template <class T>
	friend class Handle;
	template <class P>
	friend class Link;

public:
	virtual ~TObject();

protected:
	TObject() = default;

	template <class T>
	static void NotifyCreation(T* ptr);
	template <class T>
	static void NotifyDestruction(T* ptr);

	void DestroyChildren();

private:
	void DestroyHandles();

	static void Destroy(TObject* object);

	THandle* Handles = nullptr;
	TLink* Children = nullptr;
};

#ifdef _DEBUG

#include <iostream>
#include <string>

#endif

template <class T>
inline void TObject::NotifyCreation(T* ptr) {
#ifdef _DEBUG
	std::string classname(typeid(typename std::remove_pointer<T>::type).name());
	classname = classname.substr(std::string("class ").length());

	std::size_t hash = std::hash<std::string>{}(classname);

	uint32 r = (hash & 0xFF) | 0x7F;
	uint32 g = (hash >> 8) & 0xFF;
	uint32 b = (hash >> 16) & 0xFF;

	std::cout << "[\033[38;2;0;255;0mCREATION\033[m] \033[38;2;" << std::to_string(r) << ";" << std::to_string(g) << ";" << std::to_string(b) << "m" << classname << "\033[m"
			  << " - " << std::hex << ptr << std::endl;
#endif
}

template <class T>
inline void TObject::NotifyDestruction(T* ptr) {
#ifdef _DEBUG
	std::string classname(typeid(typename std::remove_pointer<T>::type).name());
	classname = classname.substr(std::string("class ").length());

	std::string classname2(typeid(*ptr).name());
	classname2 = classname2.substr(std::string("class ").length());

	std::size_t hash = std::hash<std::string>{}(classname);

	uint32 r = (hash & 0xFF) | 0x7F;
	uint32 g = (hash >> 8) & 0xFF;
	uint32 b = (hash >> 16) & 0xFF;

	std::cout << (classname == "TObject" ?
		"[\033[38;2;255;96;0mDESTR START" : "[\033[38;2;255;0;0mDESTRUCTION") << "\033[m] \033[38;2;" << std::to_string(r) << ";" << std::to_string(g) << ";" << std::to_string(b) << "m" << (classname == "TObject" ? "\033[38;2;32;32;32m" : "") << classname2
			  << "\033[m"
			  << " - " << std::hex << ptr << std::endl;
#endif
}
