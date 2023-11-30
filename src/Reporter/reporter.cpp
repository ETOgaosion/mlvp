/**
 * @file reporter.cpp
 * @author Gao Sion (gaosion2001@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-24
 * 
 * @copyright Copyright (c) Gao Sion, BOSC Institute, 2023
 * 
 */

#include "Reporter/reporter.h"

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <filesystem>

#include "Database/database.h"

using namespace std;
using namespace MLVP::Reporter;
using namespace MLVP::Transaction;
using namespace MLVP::Database;

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
        throw runtime_error("[Error] Reporter > verilator coverage failed");
    }
}