#pragma once

#include <algorithm>
#include <array>
#include <assert.h>
#include <chrono>
#include <deque>
#include <filesystem>
#include <fstream>
#include <future>
#include <iostream>
#include <limits>
#include <map>
#include <math.h>
#include <memory>
#include <mutex>
#include <numbers>
#include <random>
#include <sstream>
#include <stack>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "json.hpp"

#define PROFILING 0

#include "Profiling/Instrumentor.hpp"
#include "Profiling/Timer.hpp"

// DEBUG MACROS
#ifdef DEBUG

#endif

inline const int kCORE_COUNT = std::thread::hardware_concurrency();

#ifdef _WIN32

#define NOMINMAX
#include <direct.h>
#include <windows.h>

inline const std::string BINARY_DIRECTORY(std::filesystem::current_path().string());
inline const std::string BINARY_DIRECTORY_TEST(std::string(_getcwd(NULL, 0)) + '/');

#else

#include <filesystem>
#include <unistd.h>

inline const std::string BINARY_DIRECTORY(std::string((char *)std::filesystem::current_path().c_str()) + "/");
inline const std::string BINARY_DIRECTORY_TEST(std::string(get_current_dir_name()) + "/");

#endif

#define kEpsilon 0.000001f

template <typename T>
inline T random(T min = 0.0, T max = 1.0)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(min, max);
    return (T)dis(gen);
}

// map a given value to a range
template <typename T>
inline constexpr T map_to_range(T value, T min, T max, T new_min, T new_max)
{
    return (T)(((value - min) / (max - min)) * (new_max - new_min) + new_min);
}

void debug_print()
{
    std::cout << std::endl;
}

template <typename First, typename... Strings>
[[nodiscard]] void debug_print([[maybe_unused]] First arg, [[maybe_unused]] const Strings &...rest)
{

#ifdef DEBUG

    std::cout << arg << " ";
    debug_print(rest...);

#endif
}

template <typename Base, typename T>
inline constexpr int instanceof (const T *)
{
    return std::is_base_of<Base, T>::value;
}
