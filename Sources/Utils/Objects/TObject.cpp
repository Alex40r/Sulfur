#include "TObject.hpp"

#include "THandle.hpp"

#include <cassert>
#include <iostream>

TObject::~TObject() {
	Destroy();
}

void TObject::Destroy() {
	DestroyHandles();
}

void TObject::NotifyCreation(TObject* ptr) {
	std::string type = typeid(*ptr).name();

	const std::size_t hash = std::hash<std::string>{}(type);

	const uint32 r = (hash >> 0) & 0xFF | 0x40;
	const uint32 g = (hash >> 8) & 0xFF | 0x40;
	const uint32 b = (hash >> 16) & 0xFF | 0x40;

	std::cout << "[\033[38;2;0;255;0m"
			  << "CREATION"
			  << "\033[m]"
			  << std::dec << " \033[38;2;" << r << ";" << g << ";" << b << "m"
			  << type
			  << "\033[38;2;48;48;48m"
			  << " | " << std::hex << (0xFFF & std::hash<TObject*>{}(ptr))
			  << "\033[m"
			  << std::endl;
}

void TObject::NotifyDestruction(TObject* ptr) {
	std::string type = typeid(*ptr).name();

	const std::size_t hash = std::hash<std::string>{}(type);

	const uint32 r = (hash >> 0) & 0xFF | 0x40;
	const uint32 g = (hash >> 8) & 0xFF | 0x40;
	const uint32 b = (hash >> 16) & 0xFF | 0x40;

	std::cout << "[\033[38;2;255;0;0m"
			  << "DESTRUCTION"
			  << "\033[m]"
			  << std::dec << " \033[38;2;" << r << ";" << g << ";" << b << "m"
			  << type
			  << "\033[38;2;48;48;48m"
			  << " | " << std::hex << (0xFFF & std::hash<TObject*>{}(ptr))
			  << "\033[m"
			  << std::endl;
}

void TObject::NotifyError() {
	try {
		std::cout << "Error reported !\n";
	} catch (const std::runtime_error&) {
	}
	assert(0);
}

void TObject::DestroyHandles() {
	for (uint32 runs = 0; _HandlesHead != nullptr; runs++) {
		THandle* ptr = _HandlesHead;
		ptr->Trigger();

		if (ptr == _HandlesHead)
			ptr->Destroy();

		if (runs > 0xFFFFF)
			return;
	}
}
