#pragma once

#include <functional>

template<class T>
class Property
{
private:
	T value;
	std::function<void(const T&)> onSet;

public:
	Property();

	Property(const T& value, std::function<void(const T&)> onSet);

	T Get() const;

	void Set(const T& value);
};

template<class T>
inline Property<T>::Property() :
	value{}, onSet{ [](const T&) {} }
{
}

template<class T>
inline Property<T>::Property(const T& value, std::function<void(const T&)> onSet) :
	Property()
{
	this->value = value;
	this->onSet = onSet;
}

template<class T>
inline T Property<T>::Get() const
{
	return value;
}

template<class T>
inline void Property<T>::Set(const T& value)
{
	this->value = value;
	onSet(value);
}
