#ifndef QUADRUPLE_H
#define QUADRUPLE_H

#include <string>

struct Quadruple {
    std::string op;
    std::string arg1;
    std::string arg2;
    std::string result;

    Quadruple(const std::string& op, const std::string& arg1, const std::string& arg2, const std::string& result)
        : op(op), arg1(arg1), arg2(arg2), result(result) {}
};

class QuadrupleGenerator {
public:
    QuadrupleGenerator();
    std::string new_temp();
    void clear();

private:
    int temp_counter;
};

#endif // QUADRUPLE_H 