#pragma once

#include <memory>
#include <string>

#include "Transaction/transaction.h"

namespace MVM {
namespace Reporter {
class Reporter {
protected:
    std::string logPath;
    std::string reportPath;

public:
    Reporter() = delete;
    
    Reporter(std::string inLogPath, std::string inReportPath) : logPath(inLogPath), reportPath(inReportPath) {}

    virtual void makeReport() = 0;
};

class VerilatorReporter : public Reporter {
private:

public:
    VerilatorReporter() = delete;
    
    VerilatorReporter(std::string inLogPath, std::string inReportPath) : Reporter(inLogPath, inReportPath) {}

    void makeReport() override;

};


} // namespace Reporter



} // namespace MVM