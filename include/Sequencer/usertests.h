#pragma once

#include <vector>
#include <memory>

#include "Sequencer/testcases.h"

namespace MVM {
namespace Sequencer {
const static SerialTestsSet userTest = {
    {{1, 0, 0, 0, 0}, {1, 0, 0, 0, 0}, {0, 1, 1, 0, 1}, {0, 1, 1, 0, 1}, {0, 1, 0, 1, 0}, {0, 1, 0, 1, 0}},
    {{1, 0, 0, 0, 0}, {1, 0, 0, 0, 0}, {0, 2, 1, 0, 2}, {0, 2, 1, 0, 2}, {0, 2, 0, 1, 0}, {0, 2, 0, 1, 0}},
    {{1, 0, 0, 0, 0}, {1, 0, 0, 0, 0}, {0, 3, 1, 0, 3}, {0, 3, 1, 0, 3}, {0, 3, 0, 1, 0}, {0, 3, 0, 1, 0}},
    {{1, 0, 0, 0, 0}, {1, 0, 0, 0, 0}, {0, 4, 1, 0, 4}, {0, 4, 1, 0, 4}, {0, 4, 0, 1, 0}, {0, 4, 0, 1, 0}},
};


} // namespace Sequencer
    
} // namespace MVM