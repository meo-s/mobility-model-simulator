#pragma once
#ifndef _OPTREF_HH
#define _OPTREF_HH

#include <optional>
#include <utility>

template <typename T>
using OptRef = std::optional<std::reference_wrapper<T>>;

template <typename T>
T& getValue(OptRef<T> const& opt) {
  return opt.value().get();
}

#endif
