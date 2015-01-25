/*
 * This file is licensed under the zlib/libpng license, included in this
 * distribution in the file COPYING.
 */

#ifndef LOGFORMAT_logformat_hpp
#define LOGFORMAT_logformat_hpp

#pragma once

#include <iosfwd>
#include <string>
#include <type_traits>

#ifndef NO_THREADING
#include <thread>
#include <mutex>
#endif

namespace logformat {

namespace {
// some sfinae tools
template<typename T>
using is_defined = std::true_type;

template<typename T>
auto has_mem_to_string(int) -> decltype(is_defined<decltype(std::declval<T&>().to_string())>());

template<typename T>
auto has_mem_to_string(long) -> std::false_type;

template<typename T>
auto has_to_string(int) -> decltype(is_defined<decltype(to_string(std::declval<T&>()))>());

template<typename T>
auto has_to_string(long) -> std::false_type;

// if param is an object of user class type, call its to_string member function
template<typename T,
         typename std::enable_if<decltype(has_mem_to_string<T>(0))::value>::type* = nullptr>
std::string to_string_helper(T p) {
	return p.to_string();
}

// if param has a free to_string() overload
template<typename T,
         typename std::enable_if<!decltype(has_mem_to_string<T>(0))::value &&
                                  decltype(has_to_string<T>(0))::value>::type* = nullptr>
std::string to_string_helper(T p) {
	return to_string(p);
}

// if param has a std::to_string() overload
template<typename T,
         typename std::enable_if<!decltype(has_mem_to_string<T>(0))::value &&
                                 !decltype(has_to_string<T>(0))::value>::type* = nullptr>
std::string to_string_helper(T p) {
	return std::to_string(p);
}

}


template<typename T>
std::string param_to_string(T p) {
	return to_string_helper(p);
}

// pass through strings
template<>
std::string param_to_string(const char* p) {
	return std::string(p);
}
template<>
std::string param_to_string(std::string&& p) {
	return std::move(p);
}
template<>
std::string param_to_string(const std::string& p) {
	return p;
}


template<typename ... Rest>
std::string format(const char* text, Rest... args);
template<typename ... Rest>
std::string format(const std::string& text, Rest... args);
template<typename T1, typename ... Rest>
std::string format(std::string&& text, const T1& p1, Rest... args);

std::string format(std::string&& text) {
	std::string::size_type n = 0;
	while((n = text.rfind("%%", n)) != std::string::npos) {
		text.erase(n, 1);
	}
	return std::move(text);
}

template<typename T1, typename ... Rest>
std::string format(std::string&& text, const T1& p1, Rest... args) {
	std::string::size_type n = 0;
	std::string::size_type p1_pos = std::string::npos;
	std::string::size_type p1_len = std::string::npos;
	int lowest_ref = 100;
	while((n = text.find('%', n)) != std::string::npos) {
		// first digit of reference
		int ref = text[n+1] - '0';
		if(ref > 0 && ref <= 9) {
			// second digit - should be enough for anyone
			int ref2 = text[n+2] - '0';
			if(ref2 >= 0 && ref2 <= 9) {
				ref = ref * 10 + ref2;
			}
			if(ref < lowest_ref) {
				lowest_ref = ref;
				p1_pos = n;
				p1_len = (ref > 9 ? 3 : 2);
			}
		}
		++n;
	}
	if(p1_pos != std::string::npos) {
		text.replace(p1_pos, p1_len, param_to_string(p1));
	}
	return format(std::move(text), args...);
}

template<typename ... Rest>
std::string format(const char* text, Rest... args) {
	std::string buffer(text);
	return format(std::move(buffer), args...);
}
template<typename ... Rest>
std::string format(const std::string& text, Rest... args) {
	std::string buffer(text);
	return format(std::move(buffer), args...);
}

struct log_target_t {
};

template<typename target>
struct log_target : public log_target_t {
#ifndef NO_THREADING
	std::mutex target_mutex;
#endif
	
};

class log {
#ifndef NO_THREADING
	
#endif

	log() {}

	void output(std::string&& text) {
		// for each output, print.
	}

public:
};

}

#endif //LOGFORMAT_logformat_hpp

// vim: syntax=cpp11:sts=4:ts=4:sw=4:noet
