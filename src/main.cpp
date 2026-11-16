#define MTL_PRIVATE_IMPLEMENTATION
#define NS_PRIVATE_IMPLEMENTATION
#include "util.hpp"
#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <cstdio>
#include <print>

constexpr static auto UTF8 = NS::StringEncoding::UTF8StringEncoding;
template <typename T> using SC = ScopedRelease<T>;

constexpr char shader_source[] = {
#embed "../shaders/multiply_shader.metal" suffix(,)
    '\0'};

auto main() -> int {
  auto pool = SC{NS::AutoreleasePool::alloc()->init()};
  auto device = SC{MTL::CreateSystemDefaultDevice()};
  if (!device) {
    std::println(stderr, "No Metal device available");
    return 1;
  }
  std::println("Using Metal device: {}", device->name()->cString(UTF8));
  auto queue = SC{device->newCommandQueue()};

  NS::Error *error = nullptr;
  auto library = SC{device->newLibrary(
      NS::String::string(shader_source, UTF8), nullptr, &error)};
  if (!library) {
    std::println(stderr, "Shader compilation failed: {}",
                 error ? error->localizedDescription()->utf8String()
                       : "Unknown error");
    return 1;
  }
}
