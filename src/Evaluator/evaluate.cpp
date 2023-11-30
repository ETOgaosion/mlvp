/**
 * @file evaluate.cpp
 * @author Gao Sion (gaosion2001@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-24
 * 
 * @copyright Copyright (c) Gao Sion, BOSC Institute, 2023
 * 
 */

#include "Evaluator/evaluate.h"

#include <utility>

using namespace std;
using namespace MLVP::Evaluator;
using namespace MLVP::Type;

void Evaluator::registerEval(const string &inSrc, const string &inDest, bool isResponse, function<bool(MLVP::Type::PortsData, MLVP::Type::PortsData)> inEvalFunc) {
    auto evalKey = make_tuple(inSrc, inDest, isResponse);
    userEvalSet.emplace(evalKey, inEvalFunc);
}