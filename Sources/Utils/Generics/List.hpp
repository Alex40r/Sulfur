#pragma once

#include "Definitions.hpp"

#define LIST_CHECK_BOUNDS

template <typename T>
class TList {
public:
	TList() = default;
	TList(uint32 length);
	TList(const T* data, uint32 length);
	TList(const std::initializer_list<T> init);
	TList(const TList<T>& other);
	TList(TList<T>&& other);
	~TList();

	TList<T>& operator=(const TList<T>& other);
	TList<T>& operator=(TList<T>&& other);
	TList<T>& operator=(const std::initializer_list<T>& init);

	bool operator==(const TList<T>& other) const;
	bool operator!=(const TList<T>& other) const;

	T& operator[](uint32 index);
	const T& operator[](uint32 index) const;

	T& At(uint32 index);
	const T& At(uint32 index) const;

	T& First();
	const T& First() const;

	T& Last();
	const T& Last() const;

	uint32 FindIndex(const T& value) const;
	bool Equals(const TList<T>& other) const;

	uint32 Length() const;
	uint32 Capacity() const;
	bool Empty() const;

	T* Data();
	const T* Data() const;

	uint32 Reserve();
	uint32 Add(const T& value);
	uint32 Push(const T& value);
	void Insert(const T& value, uint32 index);

	uint32 RemoveAtEnd();
	T Pop();
	void RemoveAt(uint32 index);

	void Clear();
	void TrimExcessCapacity();
	void SetCapacity(uint32 new_capacity);
	void EnsureCapacity(uint32 target_capacity);
	void Resize(uint32 length);

	/* ---- ---- ---- ---- */
private:
	T* _Memory = nullptr;

	uint32 _Length = 0;
	uint32 _Capacity = 0;
};

/* ---- ---- ---- ---- */

template <typename T>
inline TList<T>::TList(uint32 length) {
	_Length = length;
	_Capacity = length;
	_Memory = nullptr;

	if (_Capacity == 0)
		return;

	_Memory = static_cast<T*>(malloc(sizeof(T) * _Capacity));

	if (_Memory == nullptr)
		throw std::runtime_error("Out of memory");

	for (uint32 i = 0; i < _Length; i++)
		new (&_Memory[i]) T();
}

template <typename T>
inline TList<T>::TList(const T* data, uint32 length) {
	_Length = length;
	_Capacity = length;
	_Memory = nullptr;

	if (_Capacity == 0)
		return;

	_Memory = static_cast<T*>(malloc(sizeof(T) * _Capacity));

	if (_Memory == nullptr)
		throw std::runtime_error("Out of memory");

	for (uint32 i = 0; i < _Length; i++)
		new (&_Memory[i]) T(data[i]);
}

template <typename T>
inline TList<T>::TList(const std::initializer_list<T> init)
	: TList(init.begin(), static_cast<uint32>(init.size())) {
}

template <typename T>
inline TList<T>::TList(const TList<T>& other) {
	_Length = other._Length;
	_Capacity = other._Length;
	_Memory = nullptr;

	if (_Capacity == 0)
		return;

	_Memory = static_cast<T*>(malloc(sizeof(T) * _Capacity));

	if (_Memory == nullptr)
		throw std::runtime_error("Out of memory");

	for (uint32 i = 0; i < _Length; i++)
		new (&_Memory[i]) T(other._Memory[i]);
}

template <typename T>
inline TList<T>::TList(TList<T>&& list) {
	_Length = list._Length;
	_Capacity = list._Length;
	_Memory = list._Memory;

	list._Length = 0;
	list._Capacity = 0;
	list._Memory = nullptr;
}

template <typename T>
inline TList<T>::~TList() {
	Clear();
	free(_Memory);
}

template <typename T>
inline TList<T>& TList<T>::operator=(const TList<T>& other) {
	Clear();
	free(_Memory);

	_Length = other._Length;
	_Capacity = other._Length;
	_Memory = nullptr;

	if (_Capacity == 0)
		return *this;

	_Memory = static_cast<T*>(malloc(sizeof(T) * _Capacity));

	if (_Memory == nullptr)
		throw std::runtime_error("Out of memory");

	for (uint32 i = 0; i < _Length; i++)
		new (&_Memory[i]) T(other._Memory[i]);

	return *this;
}

template <typename T>
inline TList<T>& TList<T>::operator=(TList<T>&& other) {
	Clear();
	free(_Memory);

	_Length = other._Length;
	_Capacity = other._Length;
	_Memory = other._Memory;

	other._Length = 0;
	other._Capacity = 0;
	other._Memory = nullptr;

	return *this;
}

template <typename T>
inline TList<T>& TList<T>::operator=(const std::initializer_list<T>& init) {
	Clear();
	free(_Memory);

	_Length = init.size();
	_Capacity = init.size();
	_Memory = nullptr;

	if (_Capacity == 0)
		return *this;

	_Memory = static_cast<T*>(malloc(sizeof(T) * _Capacity));

	if (_Memory == nullptr)
		throw std::runtime_error("Out of memory");

	T* data = static_cast<T*>(init.begin());

	for (uint32 i = 0; i < _Length; i++)
		new (&_Memory[i]) T(data[i]);

	return *this;
}

template <typename T>
inline bool TList<T>::operator==(const TList<T>& other) const {
	return Equals(other);
}

template <typename T>
inline bool TList<T>::operator!=(const TList<T>& other) const {
	return !Equals(other);
}

template <typename T>
inline T& TList<T>::operator[](uint32 index) {
#ifdef LIST_CHECK_BOUNDS
	if (index >= _Length)
		throw std::runtime_error("Index out of bounds");
#endif

	return _Memory[index];
}

template <typename T>
inline const T& TList<T>::operator[](uint32 index) const {
#ifdef LIST_CHECK_BOUNDS
	if (index >= _Length)
		throw std::runtime_error("Index out of bounds");
#endif

	return _Memory[index];
}

template <typename T>
inline T& TList<T>::At(uint32 index) {
#ifdef LIST_CHECK_BOUNDS
	if (index >= _Length)
		throw std::runtime_error("Index out of bounds");
#endif

	return _Memory[index];
}

template <typename T>
inline const T& TList<T>::At(uint32 index) const {
#ifdef LIST_CHECK_BOUNDS
	if (index >= _Length)
		throw std::runtime_error("Index out of bounds");
#endif

	return _Memory[index];
}

template <typename T>
inline T& TList<T>::First() {
#ifdef LIST_CHECK_BOUNDS
	if (_Length == 0)
		throw std::runtime_error("Index out of bounds");
#endif

	return _Memory[0];
}

template <typename T>
inline const T& TList<T>::First() const {
#ifdef LIST_CHECK_BOUNDS
	if (_Length == 0)
		throw std::runtime_error("Index out of bounds");
#endif

	return _Memory[0];
}

template <typename T>
inline T& TList<T>::Last() {
#ifdef LIST_CHECK_BOUNDS
	if (_Length == 0)
		throw std::runtime_error("Index out of bounds");
#endif

	return _Memory[_Length - 1];
}

template <typename T>
inline const T& TList<T>::Last() const {
#ifdef LIST_CHECK_BOUNDS
	if (_Length == 0)
		throw std::runtime_error("Index out of bounds");
#endif

	return _Memory[_Length - 1];
}

template <typename T>
inline uint32 TList<T>::FindIndex(const T& value) const {
	for (uint32 i = 0; i < _Length; i++)
		if (value == _Memory[i])
			return i;

	return INVALID_ID;
}

template <typename T>
inline bool TList<T>::Equals(const TList<T>& other) const {
	if (_Length != other._Length)
		return false;

	for (uint32 i = 0; i < _Length; i++)
		if (_Memory[i] != other._Memory[i])
			return false;

	return true;
}

template <typename T>
inline uint32 TList<T>::Length() const {
	return _Length;
}

template <typename T>
inline uint32 TList<T>::Capacity() const {
	return _Capacity;
}

template <typename T>
inline bool TList<T>::Empty() const {
	return _Length == 0;
}

template <typename T>
inline T* TList<T>::Data() {
	return _Memory;
}

template <typename T>
inline const T* TList<T>::Data() const {
	return _Memory;
}

template <typename T>
inline uint32 TList<T>::Reserve() {
	EnsureCapacity(_Length + 1);

	_Length++;
	new (&_Memory[_Length - 1]) T();

	return _Length - 1;
}

template <typename T>
inline uint32 TList<T>::Add(const T& value) {
	EnsureCapacity(_Length + 1);

	_Length++;
	new (&_Memory[_Length - 1]) T(value);

	return _Length - 1;
}

template <typename T>
inline uint32 TList<T>::Push(const T& value) {
	return Add(value);
}

template <typename T>
inline uint32 TList<T>::RemoveAtEnd() {
#ifdef LIST_CHECK_BOUNDS
	if (_Length == 0)
		throw std::runtime_error("Index out of bounds");
#endif

	_Length--;
	_Memory[_Length].~T();

	return _Length;
}

template <typename T>
inline T TList<T>::Pop() {
#ifdef LIST_CHECK_BOUNDS
	if (_Length == 0)
		throw std::runtime_error("Index out of bounds");
#endif

	_Length--;
	T value = _Memory[_Length];
	_Memory[_Length].~T();

	return value;
}

template <typename T>
inline void TList<T>::Clear() {
	for (uint32 i = 0; i < _Length; i++)
		_Memory[i].~T();

	_Length = 0;
}

template <typename T>
inline void TList<T>::TrimExcessCapacity() {
	SetCapacity(_Length);
}

template <typename T>
inline void TList<T>::SetCapacity(uint32 new_capacity) {
	if (new_capacity == _Capacity)
		return;

	uint32 new_length = _Length < new_capacity ? _Length : new_capacity;

	T* new_memory = nullptr;
	if (new_capacity != 0) {
		new_memory = static_cast<T*>(malloc(sizeof(T) * new_capacity));
		if (new_memory == nullptr)
			throw std::runtime_error("Out of memory");

		for (uint32 i = 0; i < new_length; i++)
			new (&new_memory[i]) T(std::move(_Memory[i]));
	}

	Clear();
	free(_Memory);

	_Capacity = new_capacity;
	_Memory = new_memory;
	_Length = new_length;
}

template <typename T>
inline void TList<T>::EnsureCapacity(uint32 target_capacity) {
	if (target_capacity <= _Capacity)
		return;

	uint32 new_capacity = _Capacity << 1;
	if (target_capacity > new_capacity)
		new_capacity = target_capacity;

	SetCapacity(new_capacity);
}

template <typename T>
inline void TList<T>::Resize(uint32 length) {
	EnsureCapacity(length);

	if (length > _Length)
		for (uint32 i = _Length; i < length; i++)
			new (&_Memory[i]) T();
	else
		for (uint32 i = length; i < _Length; i++)
			_Memory[i].~T();

	_Length = length;
}

/* ---- ---- ---- ---- */
/* Generic list */

template <typename T>
class List : public TList<T> {
public:
	using TList<T>::TList;
};
