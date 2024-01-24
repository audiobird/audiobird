#pragma once

#include "luts/phase_step_table.hh"
#include <cstdint>

namespace Audiobird::Oscillator {

template <uint32_t sample_rate_hz, uint8_t phase_step_table_size_bits>
class Phaser {
  using Pst =
      Luts::PhaseStep::Table<sample_rate_hz, phase_step_table_size_bits>;

public:
  using type = Luts::PhaseStep::type;
  type update() { return accumulator += inc_step; }
  type read() { return accumulator; }
  void setPhase(type p) { accumulator = p; }
  void trig() { accumulator = 0; }
  void unTrig() { inc_step = 0; }
  void reset() { accumulator = inc_step = 0; }
  void set(int8_t midi_note, int16_t pbend, int8_t pbendsemitones) {
    inc_step = Pst::fromMidiNote(midi_note, pbend, pbendsemitones);
  }
  void set(float voltage) { inc_step = Pst::fromVoltage(voltage); }

private:
  type accumulator;
  type inc_step;
};

} // namespace Audiobird::Oscillator
