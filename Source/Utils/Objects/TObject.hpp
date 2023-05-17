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

extern int TOTAL;

#ifdef _DEBUG

#include <iostream>
#include <string>

#endif

template <class T>
inline void TObject::NotifyCreation(T* ptr) {
#ifdef _DEBUG
	TOTAL++;

	std::string classname(typeid(typename std::remove_pointer<T>::type).name());
	classname = classname.substr(std::string("class ").length());

	std::size_t hash = std::hash<std::string>{}(classname);

	uint32 r = (hash & 0xFF) | 0x7F;
	uint32 g = (hash >> 8) & 0xFF;
	uint32 b = (hash >> 16) & 0xFF;

	std::cout << "[\033[38;2;0;255;0mCREATION\033[m] \033[38;2;" << std::to_string(r) << ";" << std::to_string(g) << ";" << std::to_string(b) << "m" << classname << "\033[m"
			  << " - " << std::hex << ptr << "\033[38;2;48;48;48m | " << std::to_string(TOTAL) << "\033[m\n";

#endif
}

template <class T>
inline void TObject::NotifyDestruction(T* ptr) {
#ifdef _DEBUG
	bool d = std::is_same<T, TObject>::value;
	if (!d)
		TOTAL--;

	std::string classname(typeid(*ptr).name());
	classname = classname.substr(std::string("class ").length());

	std::size_t hash = std::hash<std::string>{}(classname);

	uint32 r = (hash & 0xFF) | 0x7F;
	uint32 g = (hash >> 8) & 0xFF;
	uint32 b = (hash >> 16) & 0xFF;

	std::cout << (d ? "[\033[38;2;96;16;0mPARENT" : "[\033[38;2;255;0;0mDESTRUCTION") << "\033[m] \033[38;2;" << std::to_string(r) << ";" << std::to_string(g) << ";" << std::to_string(b) << "m" << (d ? "\033[38;2;32;32;32m" : "") << classname
			  << (d ? "" : "\033[m")
			  << " - " << std::hex << ptr << "\033[38;2;48;48;48m | " << std::to_string(TOTAL) << "\033[m\n";

#endif
}
