#pragma once
#include <concepts>

// 任何自称是 ISimulator 的类型 T，必须拥有以下成员函数。
// 这种检查发生在编译期，零运行时开销 (Zero Runtime Overhead)。

template<typename T>
concept ISimulator = requires(T t)
{
    { t.initialization() } -> std::same_as<void>;
    { t.run() } -> std::same_as<void>;
    { t.printReport() } -> std::same_as<void>;
};