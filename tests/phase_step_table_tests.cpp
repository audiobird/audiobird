#include "doctest.h"
#include "lib/luts/phase_step_table.hh"

TEST_CASE("Phase Step Table Test") {
  using namespace Audiobird::Luts;
  using Tab = PhaseStep::Table<48000, 14>;
  for (auto i = 0u; i < 128; ++i) {
    CHECK(Tab::fromMidiNote(i) == Tab::fromMidiNote(i, 0, 1));
  }
  // check pitch bend
  for (auto i = 32u; i < 64u + 32u; ++i) {
    CHECK(Tab::fromMidiNote(i) == Tab::fromMidiNote(i + 1, -8192, 1));
  }
  // TODO: Check math
}
