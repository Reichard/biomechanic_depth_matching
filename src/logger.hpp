#pragma once

#include <string>
#include <iostream>

#ifndef LOG_DEBUG
#define LOG_DEBUG true
#endif

namespace logging {

	static const bool log_debug = LOG_DEBUG;

	class NullBuffer : public std::streambuf {
		public:
			int overflow(int c) { return c; }
	};

	class NullStream : public std::ostream {
		public:
			NullStream() : std::ostream(&_null_buffer) {}
		private:
			NullBuffer _null_buffer;
	};

	inline std::ostream &debug() 
	{
		static NullStream null_stream;
		return log_debug ? std::cout : null_stream; 
	}

	inline std::ostream &d() {
		return debug();
	}
}
