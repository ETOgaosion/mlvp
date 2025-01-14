/**
 * @file driver.cpp
 * @author Gao Sion (gaosion2001@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-24
 * 
 * @copyright Copyright (c) Gao Sion, BOSC Institute, 2023
 * 
 */

#include "Drivers/driver.h"

#include <future>
#include <iostream>

using namespace std;
using namespace MLVP::Driver;
using namespace MLVP::Type;
using namespace MLVP::Library;
using namespace MLVP::Transaction;

int Driver::driving(const std::shared_ptr<std::string> &errorMsgRaw) {
    bool dutResult, refResult;
    if (USE_THREADS) {
        for (int i = 0; i < transactions.size(); i++) {
            if (!dutDriver->checkTransactionFinish()) {
                throw runtime_error("Error: transaction is not finished yet");
            }
            auto sendDutFuture = std::async(std::launch::async, [this] {
                sendTransaction(false);
            });
            auto sendRefFuture = std::async(std::launch::async, [this] {
                sendTransaction(true);
            });
            sendDutFuture.wait();
            sendRefFuture.wait();
            incTransPtr();
            auto dutFuture = std::async(std::launch::async, [this, i] {
                return dutDriver->drivingStep(i == transactions.size() - 1);
            });
            auto refFuture = std::async(std::launch::async, [this, i] {
                return refDriver->drivingStep(i == transactions.size() - 1);
            });
            dutFuture.wait();
            refFuture.wait();
            if (!transactions[i]->compareRefDut(MLVP::Transaction::Transaction::CompareOption::ALL)) {
                throw runtime_error("Error: transaction compare failed");
            }
            dutDriver->resetChannels();
            refDriver->resetChannels();
            std::cout << "driver times: " << driveTime++ << std::endl;
        }
    }
    else {
        for (int i = 0; i < transactions.size(); i++) {
            if (!dutDriver->checkTransactionFinish()) {
                throw runtime_error("Error: transaction is not finished yet");
            }
            sendTransaction(false);
            sendTransaction(true);
            incTransPtr();
            dutResult = dutDriver->drivingStep(i == transactions.size() - 1);
            refResult = refDriver->drivingStep(i == transactions.size() - 1);
            if (!transactions[i]->compareRefDut(MLVP::Transaction::Transaction::CompareOption::ALL)) {
                throw runtime_error("Error: transaction compare failed");
            }
            dutDriver->resetChannels();
            refDriver->resetChannels();
            std::cout << "driver times: " << driveTime++ << std::endl;
        }
    }
    return 0;
}