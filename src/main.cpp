#define MTL_PRIVATE_IMPLEMENTATION
#define NS_PRIVATE_IMPLEMENTATION
#include "util.hpp"
#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <print>

constexpr static auto UTF8 = NS::StringEncoding::UTF8StringEncoding;
template <typename T> using SC = ScopedRelease<T>;
auto main() -> int {

  auto hello_world_str = SC{NS::String::string("Hello World", UTF8)};

  std::println("{}", hello_world_str->cString(UTF8));
}
