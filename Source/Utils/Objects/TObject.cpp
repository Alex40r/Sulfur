#include "TObject.hpp"

#include "THandle.hpp"
#include "TLink.hpp"

Utils::TObject::~TObject() {
	DestroyHandles();
	DestroyChildren();
}

void Utils::TObject::DestroyHandles() {
	for (uint32 i = 0; Handles != nullptr; i++)
		if (Handles->Object == this && i < 0xFFFFFF)
			Handles->Destroy();
		else
			return;
}

void Utils::TObject::Destroy(TObject* object) {
	delete object;
}

void Utils::TObject::DestroyChildren() {
	for (uint32 i = 0; Children != nullptr; i++)
		delete Children->ChildObject;
}
