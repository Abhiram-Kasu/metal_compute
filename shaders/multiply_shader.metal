#include <metal_stdlib>
using namespace metal;

kernel void transform(
    device float* data [[buffer(0)]],
    uint id [[thread_position_in_grid]]
) {
    data[id] = data[id] * 2.0f + 1.0f;
}
