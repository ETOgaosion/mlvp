/**
 * @file evaluate.h
 * @author Gao Sion (gaosion2001@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once

#include <utility>
#include <vector>
#include <string>
#include <functional>
#include <tuple>
#include <map>

#include "Library/types.h"

namespace MLVP::Evaluator {
class Evaluator {
private:
    std::map<std::tuple<std::string, std::string, bool>, std::function<bool(MLVP::Type::PortsData, MLVP::Type::PortsData)>> userEvalSet;
    
    Evaluator() : userEvalSet({}) {}

public:
    ~Evaluator() = default;

    static Evaluator &getInstance() {
        static Evaluator instance;
        return instance;
    }

    void registerEval(const std::string &inSrc, const std::string &inDest, bool isResponse, std::function<bool(MLVP::Type::PortsData, MLVP::Type::PortsData)> inEvalFunc);

    bool hasValidUserEval(const std::string &inSrc, const std::string &inDest, bool isResponse) {
        return userEvalSet.contains(std::make_tuple(inSrc, inDest, isResponse));
    }

    std::function<bool(MLVP::Type::PortsData, MLVP::Type::PortsData)> getEval(const std::string &inSrc, const std::string &inDest, bool isResponse) {
        return userEvalSet[std::make_tuple(inSrc, inDest, isResponse)];
    }

    bool eval(const std::string &inSrc, const std::string &inDest, bool isResponse, MLVP::Type::PortsData dutSignal, MLVP::Type::PortsData refSignal) {
        return userEvalSet[std::make_tuple(inSrc, inDest, isResponse)](std::move(dutSignal), std::move(refSignal));
    }
};

} // namespace MLVP::Evaluator
