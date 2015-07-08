namespace Library
{
	template<class T>
	Hashmap<std::string, Factory<T>*> Factory<T>::mFactoryMap;

	template<class T>
	Factory<T>* Factory<T>::Find(const std::string& className)
	{
		if (className.length() == 0)
		{
			throw std::exception("Cannot find class with name of zero length!");
		}
		auto it = mFactoryMap.Find(className);
		if (it == mFactoryMap.end())
		{
			return nullptr;
		}
		else
		{
			return (*it).second;
		}
	}

	template<class T>
	T* Factory<T>::Create(const std::string& className)
	{
		if (className.length() == 0)
		{
			throw std::exception("Cannot create class with name of zero length!");
		}
		auto* it = Find(className);
		if (it)
		{
			return it->Create();
		}
		else
		{
			return nullptr;
		}
	}

	template<class T>
	typename Hashmap<std::string, Factory<T>*>::Iterator Factory<T>::begin()
	{
		return mFactoryMap.begin();
	}

	template<class T>
	typename Hashmap<std::string, Factory<T>*>::Iterator Factory<T>::end()
	{
		return mFactoryMap.end();
	}

	template<class T>
	void Factory<T>::Add(Factory<T>& concreteFactory)
	{
		mFactoryMap.Insert(concreteFactory.ClassName(), &concreteFactory);
	}

	template<class T>
	void Factory<T>::Remove(Factory<T>& concreteFactory)
	{
		mFactoryMap.Remove(concreteFactory.ClassName());
	}
}
