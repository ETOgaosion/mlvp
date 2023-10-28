#include "Reporter/reporter.h"

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <filesystem>

#include "Database/database.h"

using namespace std;
using namespace MVM::Reporter;
using namespace MVM::Transaction;
using namespace MVM::Database;

void VerilatorReporter::makeReport() {
    /* latest version of varilator documents are different from website, check here: */
    /* https://github.com/verilator/verilator/blob/master/docs/guide/exe_verilator_coverage.rst#verilator_coverage-example-usage */
    filesystem::create_directories(reportPath);
    string command = "verilator_coverage --annotate " + reportPath + " --write " + reportPath + "/total.bin --write-info " + reportPath + "/total.info" + " ";
    for (int i = 0; i < TransactionDatabase::getInstance().getTransactionSize(); i++) {
        command += logPath + "/Driver" + to_string(i) + "/coverage.dat ";
    }
    int status = system(command.c_str());
    if (status != 0) {
        throw std::runtime_error("[Error] Reporter > verilator coverage failed");
    }
}