#pragma once
#include <list>
#include <vector>
#include <typeinfo>
#include <functional>
#include "ECSTypes.h"
#include "Components.h"

namespace crupt
{
	//T Needs to be a component
	template <typename T>
	class SignalHandler
	{
	public:
		//Delegate
		using Listener = std::function<void (T component)>;
		
		static SignalHandler<T>& GetInstance()
		{
			static SignalHandler<T> instance{};
			return instance;
		}

		SignalHandler();

		void Update(); 

		void AddListener(Listener listener);

		void Publish(T component);

	private:
		std::vector<Listener> m_Listeners;
	}; 

	template<typename T>
	inline SignalHandler<T>::SignalHandler()
	{
	}

	template<typename T>
	inline void SignalHandler<T>::Update()
	{
	}

	template<typename T>
	inline void SignalHandler<T>::AddListener(Listener listener)
	{
		//Add a new delegate to this specific handler
		m_Listeners.push_back(listener);
	}

	template<typename T>
	inline void SignalHandler<T>::Publish(T component)
	{
		//Call all the listeners and publish our component
		for(size_t i{}; i < m_Listeners.size(); ++i)
		{
			m_Listeners[i](component);
		}
	}
}
