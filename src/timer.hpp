#pragma once

#include <iostream>
#include <chrono>
#include <typeinfo>
#include <ratio>

#include "logger.hpp"

template<class Function, class... Args>
void print_execution_time(Function func, Args&&... args)
{
	auto start = std::chrono::steady_clock::now();

	func(std::forward<Args>(args)...);

	auto end = std::chrono::steady_clock::now();

	std::chrono::duration<double,std::milli> duration = end-start;

	std::cout << "Execution Time [ " << typeid(func).name() << " ]: " 
		<< duration.count() << std::endl;
}

class Timer
{
	public:
		Timer()
		{
			reset();
		}

		std::pair<double,double> read()
		{
			auto now = std::chrono::steady_clock::now();
			std::chrono::duration<double,std::milli> duration = now-_last;
			std::chrono::duration<double,std::milli> total_duration = now-_start;
			_last = now;
			return {duration.count(),total_duration.count()};
		}

		void log(const std::string &title)
		{
			auto times = read();
			logging::d() << "TIMER " << title << ": " << times.first <<" (" << times.second << ")" << std::endl;
		}

		void print(const std::string &title)
		{
			auto times = read();
			std::cout << "TIMER " << title << ": " << times.first <<" (" << times.second << ")" << std::endl;
		}

		double reset()
		{
			_last = std::chrono::steady_clock::now();
			std::chrono::duration<double,std::milli> duration = _last-_start;
			_start = _last;
			return duration.count();
		}

	private:
		std::chrono::steady_clock::time_point _start;
		std::chrono::steady_clock::time_point _last;
};
