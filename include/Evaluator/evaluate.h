#pragma once

#include <utility>
#include <vector>
#include <string>
#include <functional>
#include <tuple>
#include <map>

#include "Library/types.h"

namespace MVM::Evaluator {
class Evaluator {
private:
    std::map<std::tuple<std::string, std::string, bool>, std::function<bool(MVM::Type::SerialTestSingle, MVM::Type::SerialTestSingle)>> userEvalSet;
    
    Evaluator() : userEvalSet({}) {}

public:
    ~Evaluator() = default;

    static Evaluator &getInstance() {
        static Evaluator instance;
        return instance;
    }

    void registerEval(std::string inSrc, std::string inDest, bool isResponse, std::function<bool(MVM::Type::SerialTestSingle, MVM::Type::SerialTestSingle)> inEvalFunc);

    bool hasValidUserEval(std::string inSrc, std::string inDest, bool isResponse) {
        return userEvalSet.contains(std::make_tuple(inSrc, inDest, isResponse));
    }

    std::function<bool(MVM::Type::SerialTestSingle, MVM::Type::SerialTestSingle)> getEval(std::string inSrc, std::string inDest, bool isResponse) {
        return userEvalSet[std::make_tuple(inSrc, inDest, isResponse)];
    }

    bool eval(std::string inSrc, std::string inDest, bool isResponse, MVM::Type::SerialTestSingle inSignal, MVM::Type::SerialTestSingle outSignal) {
        return userEvalSet[std::make_tuple(inSrc, inDest, isResponse)](inSignal, outSignal);
    }
};

} // namespace MVM::Evaluator
