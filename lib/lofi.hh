#pragma once

#include <algorithm>
#include <cstdint>

namespace Audiobird::Lofi {

template <typename T> class SampleRateReducer {
  T prev_out;
  int8_t srr = 0;
  int8_t cnt;

public:
  void trig() { cnt = 0; }
  T update(T sample) {
    cnt += 1;
    if (cnt > srr) {
      cnt = 0;
      prev_out = sample;
    }
    return prev_out;
  }
  int8_t set(int8_t redux) {
    srr = redux < 0 ? 0 : redux;
    return srr;
  }
  float set(float redux) {
    const auto o = std::clamp(redux, 0.f, 1.f);
    srr = o * 127;
    return o;
  }
};
} // namespace Audiobird::Lofi
