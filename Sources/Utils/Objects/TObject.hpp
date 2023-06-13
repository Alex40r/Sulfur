#pragma once

#include "Definitions.hpp"

class TObject : public Fixed {
	template <class>
	friend class Handle;

protected:
	TObject() = default;

public:
	virtual ~TObject();

protected:
	void Destroy();

	static void NotifyCreation(TObject* ptr);
	static void NotifyDestruction(TObject* ptr);
	static void NotifyError();

	/* ---- ---- ---- ---- */
private:
	void DestroyHandles();

	template <class T>
	static void Destroy(const Handle<T>& target);

	/* ---- ---- ---- ---- */

	THandle* _HandlesHead = nullptr;
	bool _IsDestroyed = false;
};

template <class T>
inline void TObject::Destroy(const Handle<T>& target) {
	if (target.IsInvalid())
		return;

	TObject* object = target._Object;

	if (object->_IsDestroyed)
		return;

	object->_IsDestroyed = true;

	delete object;
}
