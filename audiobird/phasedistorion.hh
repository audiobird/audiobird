#pragma once

#include "luts/sine.hh"
#include "phaser.hh"
#include <algorithm>
#include <cstdint>

namespace Audiobird::Oscillator {

template <typename T, uint32_t sample_rate_hz,
          uint8_t phase_step_table_size_bits>
class PhaseDistortion {
  static_assert(std::is_same_v<T, int16_t> || std::is_same_v<T, float>);
  using Phaser = Phaser<sample_rate_hz, phase_step_table_size_bits>;

  std::array<Phaser, 2> phaser;
  int16_t phase_distortion;
};

template <uint32_t sample_rate_hz, uint8_t phase_step_table_size_bits>
class Interface {
  using T = Phaser<sample_rate_hz, phase_step_table_size_bits>;

  std::array<T, 2> phaser;
  int8_t pd;
  int8_t midi_note;
  int8_t pbendmsb = 0;
  int8_t pbendsemitones = 1;

public:
  void Trig() {
    phaser[0].Set(midi_note, pbendmsb << 7, pbendsemitones);
    phaser[1].Set(midi_note + (pd >> 1), ((pd & 1) << 5) << 7, 1);
    phaser[0].Trig();
    phaser[1].Trig();
  }
  void UnTrig() {
    for (auto &p : phaser)
      p.UnTrig();
  }
  int16_t Update() {
    const auto prev = phaser[0].Read();
    const auto p = phaser[0].Update();
    uint16_t lut_phase;
    if (prev > p) {
      // phase wrap
      uint32_t overshoot = p;
      overshoot *= phaser[1].Update();
      phaser[1].SetPhase(overshoot >> 16);
      lut_phase = phaser[1].Read();
    } else {
      lut_phase = phaser[1].Update();
    }

    int32_t sample = Wavetable::Sin(lut_phase);

    sample *= 65535 - p;

    return sample >> 16;
  }
  int8_t SetMidiNote(int8_t m) { return midi_note = m <= 0 ? 0 : m; }
  int8_t SetPitchBendMsb(int8_t msb) {
    pbendmsb = std::clamp<int8_t>(msb, -63, 63);
    phaser[0].Set(midi_note, pbendmsb << 7, pbendsemitones);
    phaser[1].Set(midi_note + (pd >> 1), ((pd & 1) << 5) << 7, 1);
    return pbendmsb;
  }
  int8_t SetPitchBendSemitones(int8_t pbs) {
    pbendsemitones = std::clamp<int8_t>(pbs, 0, 12 * 4);
    phaser[0].Set(midi_note, pbendmsb << 7, pbendsemitones);
    phaser[1].Set(midi_note + (pd >> 1), ((pd & 1) << 5) << 7, 1);
    return pbendsemitones;
  }

  int8_t SetDistortion(int8_t i) {
    pd = std::clamp<int8_t>(i, 0, 127);
    phaser[1].Set(midi_note + (pd >> 1), ((pd & 1) << 5) << 7, 1);
    return pd;
  }
};

} // namespace Audiobird::Oscillator
