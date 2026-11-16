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

  auto function = SC{library->newFunction(NS::String::string("transform", UTF8))};
  auto pipeline = SC{device->newComputePipelineState(function, &error)};

  if (!pipeline) {
    std::println(stderr, "Pipeline creation failed: {}",
                 error ? error->localizedDescription()->utf8String()
                       : "Unknown error");
    return 1;
  }

  constexpr auto buffer_count = 1024uz * 1024uz;
  auto buffer = SC{device->newBuffer(buffer_count * sizeof(float),
                            MTL::ResourceStorageModeShared)};
  { 
    const auto data = static_cast<float *>(buffer->contents());
    for (auto i = 0uz; i < buffer_count; ++i) {
      data[i] = static_cast<float>(i);
    }
  }

  // These objects are autoreleased; the pool manages their lifetimes.
  auto command_buffer = queue->commandBuffer();
  auto encoder = command_buffer->computeCommandEncoder();

  encoder->setComputePipelineState(pipeline);
  encoder->setBuffer(buffer, 0, 0);
  
  auto thread_group_size = pipeline->maxTotalThreadsPerThreadgroup();
  
  encoder->dispatchThreadgroups(
      MTL::Size(buffer_count / thread_group_size, 1, 1),
      MTL::Size(thread_group_size, 1, 1));

  encoder->endEncoding();

  command_buffer->commit();

  command_buffer->waitUntilCompleted();

  const auto data = static_cast<float *>(buffer->contents());

  for (auto i = 0uz; i < buffer_count; ++i) {
    if (data[i] != static_cast<float>(i) * 2.0f + 1.0f) {
      std::println(stderr, "Error at index {}: expected {}, got {}", i,
                   static_cast<float>(i) * 2.0f, data[i]);
      return 1;
    }
  }

  std::println("All values are correct!");




}
