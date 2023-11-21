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

    void registerEval(const std::string &inSrc, const std::string &inDest, bool isResponse, std::function<bool(MVM::Type::SerialTestSingle, MVM::Type::SerialTestSingle)> inEvalFunc);

    bool hasValidUserEval(const std::string &inSrc, const std::string &inDest, bool isResponse) {
        return userEvalSet.contains(std::make_tuple(inSrc, inDest, isResponse));
    }

    std::function<bool(MVM::Type::SerialTestSingle, MVM::Type::SerialTestSingle)> getEval(const std::string &inSrc, const std::string &inDest, bool isResponse) {
        return userEvalSet[std::make_tuple(inSrc, inDest, isResponse)];
    }

    bool eval(const std::string &inSrc, const std::string &inDest, bool isResponse, MVM::Type::SerialTestSingle dutSignal, MVM::Type::SerialTestSingle refSignal) {
        return userEvalSet[std::make_tuple(inSrc, inDest, isResponse)](std::move(dutSignal), std::move(refSignal));
    }
};

} // namespace MVM::Evaluator
