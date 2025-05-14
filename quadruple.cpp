#include "quadruple.h"

QuadrupleGenerator::QuadrupleGenerator() : temp_counter(0) {}

std::string QuadrupleGenerator::new_temp() {
    return "t" + std::to_string(++temp_counter);
}