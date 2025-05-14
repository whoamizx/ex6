#include "quadruple.h"

QuadrupleGenerator::QuadrupleGenerator() : temp_counter(0) {}

std::string QuadrupleGenerator::new_temp() {
    return "t" + std::to_string(++temp_counter);
}

void QuadrupleGenerator::add_quadruple(const Quadruple& quad) {
    quadruples.push_back(quad);
}

const std::vector<Quadruple>& QuadrupleGenerator::get_quadruples() const {
    return quadruples;
}

void QuadrupleGenerator::clear() {
    quadruples.clear();
    temp_counter = 0;
} 