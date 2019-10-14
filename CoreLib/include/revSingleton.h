#ifndef __REVSINGLETON_H__
#define __REVSINGLETON_H__

template<typename T>
class revSingleton
{
public:
	static T& Get()
	{
		if (!instance){
			instance = new T();
		}
		return *instance;
	}

	static void DestroyInstance()
	{
		if (instance){
			delete instance;
		}
		instance = nullptr;
	}

protected:
	static T* instance;
};

template<typename T>
T* revSingleton<T>::instance = nullptr;

#endif
