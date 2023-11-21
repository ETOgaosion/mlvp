#pragma once

#include <memory>
#include <string>
#include <utility>

#include "Transaction/transaction.h"

namespace MVM::Reporter {
class Reporter {
protected:
    std::string logPath;
    std::string reportPath;

public:
    Reporter() = delete;
    
    Reporter(std::string inLogPath, std::string inReportPath) : logPath(std::move(inLogPath)), reportPath(std::move(inReportPath)) {}

    virtual void makeReport() = 0;
};

class VerilatorReporter : public Reporter {
private:

public:
    VerilatorReporter() = delete;
    
    VerilatorReporter(std::string inLogPath, std::string inReportPath) : Reporter(std::move(inLogPath), std::move(inReportPath)) {}

    void makeReport() override;

};


} // namespace MVM