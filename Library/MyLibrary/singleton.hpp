#ifndef __singleton_H
#define __singleton_H

template <class T>
class Singleton {
public:
	static T& getInstance(void) {
		static T _instance;
		return _instance;
	}

protected:
	Singleton() = default;
	~Singleton() = default;

private:
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
	Singleton(Singleton&&) = delete;
	Singleton& operator=(Singleton&&) = delete;
};

#endif