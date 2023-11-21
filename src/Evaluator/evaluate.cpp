#include "Evaluator/evaluate.h"

#include <utility>

using namespace std;
using namespace MVM::Evaluator;
using namespace MVM::Type;

void Evaluator::registerEval(const string &inSrc, const string &inDest, bool isResponse, function<bool(MVM::Type::SerialTestSingle, MVM::Type::SerialTestSingle)> inEvalFunc) {
    auto evalKey = make_tuple(inSrc, inDest, isResponse);
    userEvalSet[evalKey] = std::move(inEvalFunc);
}