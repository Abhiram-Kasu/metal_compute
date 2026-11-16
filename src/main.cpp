#define MTL_PRIVATE_IMPLEMENTATION
#define NS_PRIVATE_IMPLEMENTATION
#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <print>

constexpr static auto UTF8 = NS::StringEncoding::UTF8StringEncoding;
auto main() -> int {
  auto *pool = NS::AutoreleasePool::alloc();
  auto *hello_world_str = NS::String::string("Hello World", UTF8);

  std::println("{}", hello_world_str->cString(UTF8));
}
