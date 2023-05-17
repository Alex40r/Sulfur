#pragma once

#include "Definitions.hpp"

#include <initializer_list>

#define LIST_CHECK_BOUNDS

template <typename T>
class TList {
public:
	TList();
	TList(uint32 length);
	TList(const TList<T>& list);
	TList(const T* data, uint32 size);
	TList(const std::initializer_list<TList<T>> lists);
	TList(const std::initializer_list<T> init);
	~TList();

	T& operator[](uint32 index);
	const T& operator[](uint32 index) const;

	T& At(uint32 index);
	const T& At(uint32 index) const;

	T& First();
	const T& First() const;

	T& Last();
	const T& Last() const;

	TList<T>& operator=(const TList<T>& list);
	TList<T>& operator=(const std::initializer_list<T>& list);

	uint32 IndexOf(const T& value) const;

	void RemoveAtEnd();
	void Clear();

	uint32 Emplace();
	uint32 Add(const T& value);

	uint32 Push(const T& value);
	T Pop();

	uint32 GetLength() const;
	bool IsEmpty() const;
	uint32 GetCapacity() const;

	void SetCapacity(uint32 new_capacity);
	void EnsureCapacity(uint32 target_capacity);
	void TrimCapacity();
	void Resize(uint32 new_length);

	T* GetPointer();
	const T* GetPointer() const;

protected:
	T* Memory;
	uint32 Length;
	uint32 Capacity;
};

/* ---- ---- ---- ---- */

template <typename T>
inline TList<T>::TList()
	: Memory(nullptr)
	, Length(0)
	, Capacity(0) {
}

template <typename T>
inline TList<T>::TList(uint32 length) {
	Length = length;
	Capacity = Length;
	Memory = nullptr;

	if (Capacity != 0) {
		Memory = (T*)malloc(sizeof(T) * Capacity);
		if (Memory == nullptr)
			throw std::runtime_error("Out of memory");

		for (uint32 i = 0; i < Length; i++)
			new (&Memory[i]) T();
	}
}

template <typename T>
inline TList<T>::TList(const TList<T>& list) {
	Length = list.Length;
	Capacity = Length;
	Memory = nullptr;

	if (Capacity != 0) {
		Memory = (T*)malloc(sizeof(T) * Capacity);
		if (Memory == nullptr)
			throw std::runtime_error("Out of memory");

		for (uint32 i = 0; i < Length; i++)
			new (&Memory[i]) T(list.Memory[i]);
	}
}

template <typename T>
inline TList<T>::TList(const T* data, uint32 size) {
	Length = size;
	Capacity = Length;
	Memory = nullptr;

	if (Capacity != 0) {
		Memory = (T*)malloc(sizeof(T) * Capacity);
		if (Memory == nullptr)
			throw std::runtime_error("Out of memory");

		for (uint32 i = 0; i < Length; i++)
			new (&Memory[i]) T(data[i]);
	}
}

template <typename T>
inline TList<T>::TList(const std::initializer_list<TList<T>> lists) {
	Length = 0;
	Memory = nullptr;

	const TList<T>* l = lists.begin();

	for (uint32 i = 0; i < lists.size(); i++)
		Length += l[i].GetLength();

	Capacity = Length;

	if (Capacity != 0) {
		Memory = (T*)malloc(sizeof(T) * Capacity);
		if (Memory == nullptr)
			throw std::runtime_error("Out of memory");

		uint32 offset = 0;

		for (uint32 list_id = 0; list_id < lists.size(); list_id++)
			for (uint32 i = 0; i < l[list_id].GetLength(); i++) {
				new (&Memory[offset]) T(l[list_id][i]);
				offset++;
			}
	}
}

template <typename T>
inline TList<T>::TList(const std::initializer_list<T> init) {
	Length = (uint32)init.size();
	Capacity = Length;
	Memory = nullptr;

	const T* data = init.begin();

	if (Capacity != 0) {
		Memory = (T*)malloc(sizeof(T) * Capacity);
		if (Memory == nullptr)
			throw std::runtime_error("Out of memory");

		for (uint32 i = 0; i < Length; i++)
			new (&Memory[i]) T(data[i]);
	}
}

template <typename T>
inline TList<T>::~TList() {
	Clear();
	free(Memory);
}

template <typename T>
inline T& TList<T>::operator[](uint32 index) {
#ifdef LIST_CHECK_BOUNDS
	if (index > Length)
		throw std::runtime_error("Index is out of bounds");
#endif

	return Memory[index];
}

template <typename T>
inline const T& TList<T>::operator[](uint32 index) const {
#ifdef LIST_CHECK_BOUNDS
	if (index > Length)
		throw std::runtime_error("Index is out of bounds");
#endif

	return Memory[index];
}

template <typename T>
inline T& TList<T>::At(uint32 index) {
#ifdef LIST_CHECK_BOUNDS
	if (index > Length)
		throw std::runtime_error("Index is out of bounds");
#endif

	return Memory[index];
}

template <typename T>
inline const T& TList<T>::At(uint32 index) const {
#ifdef LIST_CHECK_BOUNDS
	if (index > Length)
		throw std::runtime_error("Index is out of bounds");
#endif

	return Memory[index];
}

template <typename T>
inline T& TList<T>::First() {
#ifdef LIST_CHECK_BOUNDS
	if (Length == 0)
		throw std::runtime_error("List is empty");
#endif

	return Memory[0];
}

template <typename T>
inline const T& TList<T>::First() const {
#ifdef LIST_CHECK_BOUNDS
	if (Length == 0)
		throw std::runtime_error("List is empty");
#endif

	return Memory[0];
}

template <typename T>
inline T& TList<T>::Last() {
#ifdef LIST_CHECK_BOUNDS
	if (Length == 0)
		throw std::runtime_error("List is empty");
#endif

	return Memory[Length - 1];
}

template <typename T>
inline const T& TList<T>::Last() const {
#ifdef LIST_CHECK_BOUNDS
	if (Length == 0)
		throw std::runtime_error("List is empty");
#endif

	return Memory[Length - 1];
}

template <typename T>
inline TList<T>& TList<T>::operator=(const TList<T>& list) {
	Clear();
	free(Memory);

	Length = list.Length;
	Capacity = Length;
	Memory = nullptr;

	if (Capacity != 0) {
		Memory = (T*)malloc(sizeof(T) * Capacity);
		if (Memory == nullptr)
			throw std::runtime_error("Out of memory");

		for (uint32 i = 0; i < Length; i++)
			new (&Memory[i]) T(list.Memory[i]);
	}

	return *this;
}

template <typename T>
inline TList<T>& TList<T>::operator=(const std::initializer_list<T>& list) {
	Clear();
	free(Memory);

	Length = (uint32)list.size();
	Capacity = Length;
	Memory = nullptr;

	const T* data = list.begin();

	if (Capacity != 0) {
		Memory = (T*)malloc(sizeof(T) * Capacity);
		if (Memory == nullptr)
			throw std::runtime_error("Out of memory");

		for (uint32 i = 0; i < Length; i++)
			new (&Memory[i]) T(data[i]);
	}

	return *this;
}

template <typename T>
inline uint32 TList<T>::IndexOf(const T& value) const {
	for (uint32 i = 0; i < Length; i++)
		if (Memory[i] == value)
			return i;

	return INVALID_ID;
}


template <typename T>
inline void TList<T>::RemoveAtEnd() {
#ifdef LIST_CHECK_BOUNDS
	if (Length == 0)
		throw std::runtime_error("List is empty");
#endif

	Length--;
	Memory[Length].~T();
}

template <typename T>
inline void TList<T>::Clear() {
	while (Length > 0)
		RemoveAtEnd();
}

template <typename T>
inline uint32 TList<T>::Emplace() {
	EnsureCapacity(Length + 1);
	Length++;
	new (&Memory[Length - 1]) T();
	return Length - 1;
}

template <typename T>
inline uint32 TList<T>::Add(const T& value) {
	EnsureCapacity(Length + 1);
	Length++;
	new (&Memory[Length - 1]) T(value);
	return Length - 1;
}

template <typename T>
inline uint32 TList<T>::Push(const T& value) {
	return Add(value);
}

template <typename T>
inline T TList<T>::Pop() {
#ifdef LIST_CHECK_BOUNDS
	if (Length == 0)
		throw std::runtime_error("List is empty");
#endif

	Length--;
	T copy = T(Memory[Length]);
	Memory[Length].~T();
	return copy;
}

template <typename T>
inline uint32 TList<T>::GetLength() const {
	return Length;
}

template <typename T>
inline bool TList<T>::IsEmpty() const {
	return Length == 0;
}

template <typename T>
inline uint32 TList<T>::GetCapacity() const {
	return Capacity;
}

template <typename T>
inline void TList<T>::SetCapacity(uint32 new_capacity) {
	while (Length > new_capacity)
		RemoveAtEnd();

	// TODO: make sure to prevent mem leaks
	T* new_memory = nullptr;
	uint32 new_length = Length;

	if (new_capacity != 0) {
		new_memory = (T*)malloc(sizeof(T) * new_capacity);
		if (new_memory == nullptr)
			throw std::runtime_error("Out of memory");
		for (uint32 i = 0; i < Length; i++)
			new (&new_memory[i]) T(Memory[i]);
	}

	Clear();
	free(Memory);

	Capacity = new_capacity;
	Memory = new_memory;
	Length = new_length;
}

template <typename T>
inline void TList<T>::EnsureCapacity(uint32 target_capacity) {
	if (target_capacity <= Capacity)
		return;

	if (target_capacity > (Capacity << 1))
		SetCapacity(target_capacity);
	else
		SetCapacity(Capacity << 1);
}

template <typename T>
inline void TList<T>::TrimCapacity() {
	SetCapacity(Length);
}

template <typename T>
inline void TList<T>::Resize(uint32 new_length) {
	EnsureCapacity(new_length);

	while (Length > new_length)
		RemoveAtEnd();

	while (Length < new_length)
		Emplace();
}

template <typename T>
inline T* TList<T>::GetPointer() {
	return Memory;
}

template <typename T>
inline const T* TList<T>::GetPointer() const {
	return Memory;
}

/* ---- ---- ---- ---- */
/* Generic list */

template <typename T>
class List : public TList<T> {
public:
	using TList<T>::TList;
};
