#include "Evaluator/evaluate.h"

using namespace std;
using namespace MVM::Evaluator;
using namespace MVM::Type;

void Evaluator::registerEval(std::string inSrc, std::string inDest, bool isResponse, std::function<bool(MVM::Type::SerialTestSingle, MVM::Type::SerialTestSingle)> inEvalFunc) {
    auto evalKey = make_tuple(inSrc, inDest, isResponse);
    userEvalSet[evalKey] = inEvalFunc;
}