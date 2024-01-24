#include "doctest.h"
#include "lib/luts/sine.hh"

TEST_CASE("Sine Test") {
  using namespace Audiobird::Luts;
  CHECK(Sine<8, int16_t>::table.size() == 256);
  // TODO: Check more stuff?
}
