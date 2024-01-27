#pragma once

#include "phaser.hh"
#include <algorithm>
#include <cstdint>
#include <type_traits>

namespace Audiobird::Oscillator {

template <typename T, uint32_t sample_rate_hz,
          uint8_t phase_step_table_size_bits>
class Square : public Phaser<sample_rate_hz, phase_step_table_size_bits> {
  static_assert(std::is_same_v<T, int16_t> || std::is_same_v<T, float>);
  using mtPhaser = Phaser<sample_rate_hz, phase_step_table_size_bits>;
  static_assert(std::is_same_v<typename mtPhaser::type, uint16_t>);

  mtPhaser::type pulsewidth;

public:
  T update() {
    const auto p = mtPhaser::update();
    const auto high = p >= (32768 + pulsewidth);
    if constexpr (std::is_same_v<T, float>) {
      return high ? 1.f : -1.f;
    } else {
      return high ? 32767 : -32767;
    }
  }
  int8_t setPulseWidth(int8_t pw) {
    pulsewidth = pw << 8;
    return pw;
  }
  float setPulseWidth(float pw) {
    const auto o = std::clamp(pw, -1.f, 1.f);
    pulsewidth = pw * 32767;
    return o;
  }
};

} // namespace Audiobird::Oscillator
