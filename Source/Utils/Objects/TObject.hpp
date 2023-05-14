#pragma once

#include "Definitions.hpp"

#include <iostream>
#include <string>

class Utils::TObject : public Fixed {
	friend class Utils;

protected:
	TObject() = default;
	virtual ~TObject();

	void DestroyChildren();

	template <class T>
	static void NotifyCreation(T* ptr);
	template <class T>
	static void NotifyDestruction(T* ptr);

private:
	void DestroyHandles();

	static void Destroy(TObject* object);

	Utils::THandle* Handles = nullptr;
	Utils::TLink* Children = nullptr;
};

/* ---- ---- ---- ---- */

template <class T>
inline void Utils::TObject::NotifyCreation(T* ptr) {
#ifdef _DEBUG
	std::string classname(typeid(typename std::remove_pointer<T>::type).name());
	classname = classname.substr(std::string("class ").length());

	std::size_t hash = std::hash<std::string>{}(classname);

	uint32 r = (hash & 0xFF);
	uint32 g = (hash >> 8) & 0xFF;
	uint32 b = (hash >> 16) & 0xFF;

	std::cout << "[\033[38;2;0;255;0mCREATION\033[m] \033[38;2;" << std::to_string(r) << ";" << std::to_string(g) << ";" << std::to_string(b) << "m" << classname << "\033[m"
			  << " - " << std::hex << ptr << std::endl;
#endif
}

template <class T>
inline void Utils::TObject::NotifyDestruction(T* ptr) {
#ifdef _DEBUG
	std::string classname(typeid(typename std::remove_pointer<T>::type).name());
	classname = classname.substr(std::string("class ").length());

	std::size_t hash = std::hash<std::string>{}(classname);

	uint32 r = (hash & 0xFF);
	uint32 g = (hash >> 8) & 0xFF;
	uint32 b = (hash >> 16) & 0xFF;

	std::cout << "[\033[38;2;255;0;0mDESTRUCTION\033[m] \033[38;2;" << std::to_string(r) << ";" << std::to_string(g) << ";" << std::to_string(b) << "m" << classname << "\033[m"
			  << " - " << std::hex << ptr << std::endl;
#endif
}
