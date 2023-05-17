#include "TObject.hpp"

#include "THandle.hpp"
#include "TLink.hpp"

TObject::~TObject() {
	DestroyHandles();
	DestroyChildren();
}

void TObject::DestroyChildren() {
	for (uint32 i = 0; Children != nullptr; i++)
		if (Children->Matches(this) && i < 0xFFFFFF)
			Destroy(Children->GetChild());
		else
			return;
}

void TObject::DestroyHandles() {
	for (uint32 i = 0; Handles != nullptr; i++)
		if (Handles->Matches(this) && i < 0xFFFFFF)
			Handles->Destroy();
		else
			return;
}

void TObject::Destroy(TObject* object) {
	NotifyDestruction(object);

	delete object;
}
