#pragma once
#include <concepts>
#include <memory>
#include <type_traits>

template <typename T>
concept auto_releasable = std::is_pointer_v<T> and requires(T item) {
  { item->release() } -> std::same_as<void>;
};
struct release_deleter {
  template <typename U>
  void operator()(U *p) const noexcept { p->release(); }
};

template <auto_releasable T>
struct ScopedRelease
    : std::unique_ptr<std::remove_pointer_t<T>, release_deleter> {
  using base = std::unique_ptr<std::remove_pointer_t<T>, release_deleter>;
  using base::base;
  operator T() const noexcept { return this->get(); }
};

template <auto_releasable T> ScopedRelease(T) -> ScopedRelease<T>;
