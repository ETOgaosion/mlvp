/**
 * @file main.cpp
 * @author Gao Sion (gaosion2001@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-24
 * 
 * @copyright Copyright (c) Gao Sion, BOSC Institute, 2023
 * 
 */

/**
 * @mainpage MLVP Documentation
 *
 * <b>MLVP</b> is a  <b>M</b>ulti-<b>L</b>anguage <b>V</b>erification <b>P</b>latform, which is a framework for multi-language verification, coding in C++
 *
 * @section intro_sec Introduction
 *
 * Documentation is in the [official documnetation website](https://mlvp-doc.readthedocs.io/zh-cn/latest/)
 *
 * @section arch_sec Architecture
 *
 * This is our architecture
 *
 * @image html MLVP_BareDut.png
 *
 * Our Test result waveform:
 *
 * @image html MLVP_Waveform.png
 * 
 */

#include "Library/types.h"
#include "Transaction/transaction.h"
#include "mlvp.h"
#include "MCVPack/BareDut/NutshellCache/Vnutshellcache.h"

#include <iostream>
#include <fstream>
#include <cassert>
#include <memory>
#include <optional>
#include <random>
#include <stdexcept>
#include <string>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>
#include <chrono>

/**
 * @brief SimpleBusCmd
 * 
 */
enum class SimpleBusCmd : Data {
    //! req
    read        = 0b0000,
    write       = 0b0001,
    readBurst   = 0b0010,
    writeBurst  = 0b0011,
    writeLast   = 0b0111,
    prob        = 0b1000,
    prefetch    = 0b0100,

    //! resp
    readLast    = 0b0110,
    writeResp   = 0b0101,
    probHit     = 0b1100,
    probeMiss   = 0b1000
};

// ========================= DutUnitDriver =========================
class DutCacheDriver : public DutUnitDriver {
private:
    const shared_ptr<Vnutshellcache> top;
    int drivingTimes = 0;
    int transactionNumber = 0;
    bool firstTime = true;
    chrono::time_point<std::chrono::high_resolution_clock> startTime;
    ofstream logFile;

public:
    DutCacheDriver(int inResetCycles, int inDriverID, const string& inLogPath) : DutUnitDriver(inResetCycles, "cache", inDriverID, inLogPath), top(make_shared<Vnutshellcache>(contextp.get(), "top")) {
        contextp->debug(0);
        //! [notice] > randReset(other value) can cause error, because our first posedge is delayed 1 cycle
        contextp->randReset(0);
        top->clock = 1;
        top->trace(tfp.get(), 99);
        //! this should be called after trace
        tfp->open((logPath + "/cache.vcd").c_str());
        logFile.open("debug/log.txt", ios::trunc);
    }
    ~DutCacheDriver() override {
        logFile.close();
    }

    void reset() override {
        contextp->timeInc(1);
        top->clock = !top->clock;
        top->reset = true;
        //! evaluate model
        top->eval();
        //! generate trace
        tfp->dump(contextp->time());

        //! a cycle is down and up, so eval twice
        contextp->timeInc(1);
        top->clock = !top->clock;
        top->eval();
        tfp->dump(contextp->time());
    }

    /**
     * @brief just execute one cycle
     */
    bool drivingStep(bool isLast) override {
        drivingTimes++;
        if (firstTime) {
            firstTime = false;
            startTime = std::chrono::high_resolution_clock::now();
        }
        //! Sync Global Timer
        if (USE_SYNC) {
            GlobalUserTimer::getInstance().incTime();
        }

        if(!contextp->gotFinish()) {
            bool hasData = false;

            //! negative edge
            contextp->timeInc(1);
            top->clock = !top->clock;
            top->eval();
            //! generate trace
            tfp->dump(contextp->time());

            //! Write your state machine here, <b>Notice that we will detect when your transaction totally done to launch new one, you MUST figure out what is transaction in your case clearly</b>
            auto transactionStatus = transaction->getTotalStatus(false);
            auto memRes = channels[make_tuple("cache", "memory", false)]->getData(std::vector<std::string>({"mem_resp_valid", "mem_resp_bits_cmd", "mem_resp_bits_rdata"}), hasData);

            //! assign input signals
            //! c_if
            top->reset = transaction->getInSignal("reset");
            top->io_flush = transaction->getInSignal("io_flush");

            //! in_if
            if ((transactionStatus == TransactionStatus::NEW && top->io_in_req_ready)) {
                top->io_in_req_valid = true;
            }
            else {
                top->io_in_req_valid = false;
            }

            top->io_in_req_bits_addr = transaction->getInSignal("io_in_req_bits_addr");
            top->io_in_req_bits_size = transaction->getInSignal("io_in_req_bits_size");
            top->io_in_req_bits_cmd = transaction->getInSignal("io_in_req_bits_cmd");
            top->io_in_req_bits_wmask = transaction->getInSignal("io_in_req_bits_wmask");
            top->io_in_req_bits_wdata = transaction->getInSignal("io_in_req_bits_wdata");
            top->io_in_req_bits_user = transaction->getInSignal("io_in_req_bits_user");
            top->io_in_resp_ready = 1;

            //! mem_if
            top->io_out_mem_req_ready = true;
            top->io_out_mem_resp_valid = memRes["mem_resp_valid"];
            top->io_out_mem_resp_bits_cmd = memRes["mem_resp_bits_cmd"];
            bool burstLast = true;
            top->io_out_mem_resp_bits_rdata = memRes["mem_resp_bits_rdata"];

            //! coh_if
            top->io_out_coh_req_valid = 0;
            top->io_out_coh_req_bits_addr = 0;
            top->io_out_coh_req_bits_size = 0;
            top->io_out_coh_req_bits_cmd = 0;
            top->io_out_coh_req_bits_wmask = 0;
            top->io_out_coh_req_bits_wdata = 0;
            top->io_out_coh_resp_ready = 0;

            //! mmio_if
            top->io_mmio_req_ready = true;
            auto mmioRes = channels[make_tuple("cache", "mmio", false)]->getData(std::vector<std::string>({"mmio_resp_valid", "mmio_resp_bits_cmd", "mmio_resp_bits_rdata"}), hasData);
            top->io_mmio_resp_valid = mmioRes["mmio_resp_valid"];
            top->io_mmio_resp_bits_cmd = mmioRes["mmio_resp_bits_cmd"];
            top->io_mmio_resp_bits_rdata = mmioRes["mmio_resp_bits_rdata"];

            if (SHOW_TEXT_WAVE) {
                logFile << "Driving Times: " << drivingTimes
                        << " Status: " << (Data)transactionStatus
                        << " in_req_ready: " << (Data)top->io_in_req_ready
                        << " in_req_valid: " << (Data)top->io_in_req_valid
                        << " in_req_bits_addr: " << (Data)top->io_in_req_bits_addr
                        << " in_resp_valid: " << (Data)top->io_in_resp_valid
                        << " in_resp_bits_rdata: " << (Data)top->io_in_resp_bits_rdata
                        << " out_mem_req_valid: " << (Data)top->io_out_mem_req_valid
                        << " out_mem_resp_valid: " << (Data)top->io_out_mem_resp_valid
                        << " out_mem_resp_bits_cmd: " << (Data)top->io_out_mem_resp_bits_cmd
                        << " out_mem_resp_bits_rdata: " << (Data)top->io_out_mem_resp_bits_rdata;
            }

            //! a cycle is down and up, so eval twice
            contextp->timeInc(1);
            top->clock = !top->clock;
            top->eval();
            tfp->dump(contextp->time());

            if (SHOW_TEXT_WAVE) {
                logFile << " in_resp_valid: " << (Data)top->io_in_resp_valid
                        << " in_resp_bits_rdata: " << (Data)top->io_in_resp_bits_rdata
                        << " out_mem_req_valid: " << (Data)top->io_out_mem_req_valid
                        << " out_mem_resp_valid: " << (Data)top->io_out_mem_resp_valid
                        << " out_mem_resp_bits_cmd: " << (Data)top->io_out_mem_resp_bits_cmd
                        << " out_mem_resp_bits_rdata: " << (Data)top->io_out_mem_resp_bits_rdata << endl;
            }

            if (transaction->getInSignal("reset")) {
                transaction->transactionItems.setDone(false);
                transaction->setTransactionDone(false);
                return true;
            }

            //! assign output signals
            if (top->io_in_resp_ready && top->io_in_resp_valid) {
                // cout << "finish a request" << endl;
                transaction->setOutSignal("io_empty", top->io_empty, false);
                transaction->setOutSignal("io_in_resp_valid", top->io_in_resp_valid, false);
                transaction->setOutSignal("io_in_resp_bits_cmd", top->io_in_resp_bits_cmd, false);
                transaction->setOutSignal("io_in_resp_bits_rdata", top->io_in_resp_bits_rdata, false);
                transaction->setOutSignal("io_in_resp_bits_user", top->io_in_resp_bits_user, false);
                transaction->transactionItems.setDone(false);
            }

            //! mem_if
            if (top->io_out_mem_req_ready && top->io_out_mem_req_valid) {
                channels[make_tuple("cache", "memory", false)]->setData({
                    {"mem_req_valid", top->io_out_mem_req_valid},
                    {"mem_req_bits_addr", top->io_out_mem_req_bits_addr},
                    {"mem_req_bits_size", top->io_out_mem_req_bits_size},
                    {"mem_req_bits_cmd", top->io_out_mem_req_bits_cmd},
                    {"mem_req_bits_wmask", top->io_out_mem_req_bits_wmask},
                    {"mem_req_bits_wdata", top->io_out_mem_req_bits_wdata}
                }, true, false, true);
            }

            //! mmio_if
            if (top->io_mmio_req_ready && top->io_mmio_req_valid) {
                channels[make_tuple("cache", "mmio", false)]->setData({
                    {"mmio_req_valid", top->io_mmio_req_valid},
                    {"mmio_req_bits_addr", top->io_mmio_req_bits_addr},
                    {"mmio_req_bits_size", top->io_mmio_req_bits_size},
                    {"mmio_req_bits_cmd", top->io_mmio_req_bits_cmd},
                    {"mmio_req_bits_wmask", top->io_mmio_req_bits_wmask},
                    {"mmio_req_bits_wdata", top->io_mmio_req_bits_wdata}
                }, true, false, true);
            }

            channels[make_tuple("cache", "cache", false)]->setData({{"victimWaymask", top->victimWaymask}}, false, false, true);

            if ((transactionStatus == TransactionStatus::INVALID && top->io_in_req_ready)) {
                transaction->setTransactionNew(false);
            }
            else if (transactionStatus == TransactionStatus::NEW && top->io_in_req_ready && top->io_in_req_valid) {
                transaction->setTransactionHandling(false);
            }
            if (transactionStatus == TransactionStatus::HANDLING && ((top->io_out_mem_resp_bits_cmd == 6 && top->io_out_mem_resp_valid) || (!top->io_out_mem_resp_valid && top->io_in_resp_valid))) {
                transaction->setTransactionDone(false);
                cout << "Transaction times: " << transactionNumber++ << endl;
            }
        }

        if (isLast && transaction->getTotalStatus(false) == TransactionStatus::DONE) {
            auto now = std::chrono::high_resolution_clock::now();
            cout << "Total Time: " << std::chrono::duration_cast<std::chrono::nanoseconds>(now - startTime).count() << "ns" << endl;
            cout << "Total Driving Times: " << drivingTimes << endl;
            cout << "cycles/s: " << (double)drivingTimes / ((double)chrono::duration_cast<std::chrono::nanoseconds>(now - startTime).count() / pow(10, 9)) << endl;
            //! don't know why tfp dump lose last cycle
            contextp->timeInc(1);
            top->clock = !top->clock;
            top->eval();
            tfp->dump(contextp->time());
            contextp->timeInc(1);
            top->clock = !top->clock;
            top->eval();
            tfp->dump(contextp->time());
            //! output coverage
            Verilated::mkdir(logPath.c_str());
            contextp->coveragep()->write((logPath + "/coverage.dat").c_str());
            tfp->close();
            top->final();
        }
        return true;
    }
};

// ========================= Ref and RefDriver =========================
/**
 * @brief Ref Cache simulator, if you hope to drive by cycles, use it
 * 
 */
//! class RefCache : public Ref {};

/**
 * @brief RefUnitDriver, use a ref class cache simulate the dut logic
 * @details if you hope to drive by cycles, use it as a wrapper of RefUnit, or else, just handle the transactions here
 * 
 */
class RefCacheDriver : public RefUnitDriver {
private:
    bool cacheEmpty = false;
    //! <b>Notice that C++ vector<bool> implemented different self other vector, cannot use reference</b>, we use vector<char> instead
    vector<vector<char>> cacheValid = vector<vector<char>>(128, vector<char>(4, 0));
    //! <b>Notice that C++ vector<bool> implemented different self other vector, cannot use reference</b>, we use vector<char> instead
    vector<vector<char>> cacheDirty = vector<vector<char>>(128, vector<char>(4, 0));
    vector<vector<Data>> cacheTag = vector<vector<Data>>(128, vector<Data>(4, 0));
    vector<vector<vector<Data>>> cacheData = vector<vector<vector<Data>>>(128, vector<vector<Data>>(4, vector<Data>(8, 0)));

public:
    explicit RefCacheDriver(int inResetCycles) : RefUnitDriver(inResetCycles, "cache") {};
    ~RefCacheDriver() override = default;

    void reset() override {
        cacheEmpty = false;
        cacheValid = vector<vector<char>>(128, vector<char>(4, 0));
        cacheDirty = vector<vector<char>>(128, vector<char>(4, 0));
        cacheTag = vector<vector<Data>>(128, vector<Data>(4, 0));
        cacheData = vector<vector<vector<Data>>>(128, vector<vector<Data>>(4, vector<Data>(8, 0)));
    }

    void write_back(int setId, int wayId) {
        bool hasData = false;
        PortsData request = {
            {"mem_req_bits_addr", (cacheTag[setId][wayId] << 13) | (setId << 6)},
            {"mem_req_bits_cmd", static_cast<Data>(SimpleBusCmd::writeBurst)},
            {"mem_req_bits_size", 0b011},
            {"mem_req_bits_wmask", 0xff}
        };
        for (int i = 0; i < 7; i++) {
            request["req_bits_wdata"] = cacheData[setId][wayId][i];
            channels[make_tuple("cache", "memory", true)]->setData(request, true, false, false);
            assert(channels[make_tuple("cache", "memory", true)]->getData("mem_resp_bits_rdata", hasData) == static_cast<Data>(SimpleBusCmd::writeResp));
        }
        request["wb_req_bits_cmd"] = static_cast<Data>(SimpleBusCmd::writeLast);
        request["wb_req_bits_wdata"] = cacheData[setId][wayId][7];
        channels[make_tuple("cache", "memory", true)]->setData(request, true, false, false);
        assert(channels[make_tuple("cache", "memory", true)]->getData("mem_resp_bits_rdata", hasData) == static_cast<Data>(SimpleBusCmd::writeResp));
        cacheValid[setId][wayId] = 0;
    }

    void fetch(Data addr, int wayId, int wordId) {
        bool hasData = false;
        Data tag = addr >> 13;
        Data setId = (addr >> 6) & ((0x1 << 7) - 1);
        channels[make_tuple("cache", "memory", true)]->setData({
            {"mem_req_bits_addr", addr & ~(0x7)},
            {"mem_req_bits_size", 0b011},
            {"mem_req_bits_cmd", static_cast<Data>(SimpleBusCmd::readBurst)},
            {"mem_req_bits_wmask", 0},
            {"mem_req_bits_wdata", 0}
        }, true, false, false);
        cacheEmpty = false;
        cacheValid[setId][wayId] = 1;
        cacheDirty[setId][wayId] = 0;
        cacheTag[setId][wayId] = tag;
        while (!hasData) {
            channels[make_tuple("cache", "memory", true)]->getData("mem_resp_bits_rdata", hasData);
        }
        cacheData[setId][wayId][wordId] = channels[make_tuple("cache", "memory", true)]->getData("mem_resp_bits_rdata", hasData);
    }

    static int mask2index(int mask) {
        for (int i = 0; i < 4; i++) {
            if (mask & (0x1 << i)) {
                return i;
            }
        }
        return -1;
    }

    static Data bytesmask2bitsmask(char bytemask) {
        Data bitmask = 0;
        //! betther than sv solution
        for (int i = 0; i < 8; i++) {
            if (bytemask & (0x1 << i)) {
                bitmask |= (0xff << (i * 8));
            }
        }
        return bitmask;
    }

    bool drivingStep(bool isLast) override {
        //! Sync Global Timer
        if (USE_SYNC) {
            GlobalUserTimer::sync();
        }
        bool hasData = false;

        //! reset
        if (transaction->getInSignal("reset")) {
            reset();
            transaction->transactionItems.setDone(true);
            transaction->setTransactionDone(true);
            return true;
        }

        // cout << "ref get transaction" << endl;

        auto addr = transaction->getInSignal("io_in_req_bits_addr");
        auto mmioAddr = addr >> 28;
        auto reqTag = addr >> 13;
        auto reqSetId = addr >> 6 & ((0x1 << 7) - 1);
        auto reqWordId = addr >> 3 & ((0x1 << 3) - 1);
        auto inSize = transaction->getInSignal("io_in_req_bits_size");
        auto inCmd = transaction->getInSignal("io_in_req_bits_cmd");
        auto inWmask = transaction->getInSignal("io_in_req_bits_wmask");
        auto inWdata = transaction->getInSignal("io_in_req_bits_wdata");
        auto inUser = transaction->getInSignal("io_in_req_bits_user");

        //! mmio
        if (mmioAddr >= 3 && mmioAddr <= 7) {
            channels[make_tuple("cache", "mmio", true)]->setData({
                {"mmio_req_valid", 1},
                {"mmio_req_bits_addr", addr},
                {"mmio_req_bits_size", inSize},
                {"mmio_req_bits_cmd", inCmd},
                {"mmio_req_bits_wmask", inWmask},
                {"mmio_req_bits_wdata", inWdata}
            }, true, false, false);
            transaction->setOutSignal("io_in_resp_valid", 1, true);
            transaction->setOutSignal("io_in_resp_bits_cmd", channels[make_tuple("cache", "mmio", true)]->getData("mmio_resp_bits_cmd", hasData), true);
            transaction->setOutSignal("io_in_resp_bits_rdata", channels[make_tuple("cache", "mmio", true)]->getData("mmio_resp_bits_rdata", hasData), true);
            transaction->setOutSignal("io_in_resp_bits_user", inUser, true);
            transaction->transactionItems.setDone(true);
            transaction->setTransactionDone(true);
            return true;
        }
        

        //! check hitness
        Data bitmask = bytesmask2bitsmask((char)inWmask);
        int hitId = -1;
        bool needRefill = false;

        for (int i = 0; i < 4; i++) {
            if (cacheValid[reqSetId][i] && cacheTag[reqSetId][i] == reqTag) {
                hitId = i;
                break;
            }
        }

        //! miss
        if (hitId == -1) {
            int victimId = -1;
            for (int i = 3; i >= 0; i--) {
                if (!cacheValid[reqSetId][i]) {
                    victimId = i;
                    break;
                }
            }
            //! need evict
            if (victimId  == -1) {
                hasData = false;
                while (!hasData) {
                    victimId = mask2index((int)channels[make_tuple("cache", "cache", false)]->getData("victimWaymask", hasData));
                }
                if (cacheDirty[reqSetId][victimId]) {
                    write_back((int)reqSetId, victimId);
                }
            }
            //! fetch data
            fetch(addr, victimId, (int)reqWordId);
            needRefill = true;
            hitId = victimId;
        }

        //! write cache
        if (inCmd == static_cast<Data>(SimpleBusCmd::write) || inCmd == static_cast<Data>(SimpleBusCmd::writeBurst) || inCmd == static_cast<Data>(SimpleBusCmd::writeLast)) {
            cacheDirty[reqSetId][hitId] = 1;
            cacheData[reqSetId][hitId][reqWordId] = (cacheData[reqSetId][hitId][reqWordId] & ~bitmask) | (inWdata & bitmask);
        }

        //! send response
        transaction->setOutSignal("io_in_resp_valid", 1, true);
        transaction->setOutSignal("io_in_resp_bits_user", inUser, true);
        transaction->setOutSignal("io_empty", cacheEmpty, true);
        if (inCmd == static_cast<Data>(SimpleBusCmd::read) || inCmd == static_cast<Data>(SimpleBusCmd::readBurst)) {
            transaction->setOutSignal("io_in_resp_bits_cmd", static_cast<Data>(SimpleBusCmd::readLast), true);
            transaction->setOutSignal("io_in_resp_bits_rdata", cacheData[reqSetId][hitId][reqWordId], true);
            transaction->transactionItems.setDone(true);
        } else {
            transaction->setOutSignal("io_in_resp_bits_cmd", static_cast<Data>(SimpleBusCmd::writeResp), true);
            transaction->setOutSignal("io_in_resp_bits_rdata", 0, true);
            transaction->transactionItems.setDone(true);
        }

        //! refill
        if (needRefill) {
            auto pktId = reqWordId;
            for (int i = 1; i < 8; i++) {
                pktId = (pktId + 1) % 8;
                cacheData[reqSetId][hitId][pktId] = channels[make_tuple("cache", "memory", true)]->getData("mem_resp_bits_rdata", hasData);
            }
        }
        transaction->setTransactionDone(true);
        return true;
    }
};

// ============================= Simulator and Simulator Driver =================================

/**
 * @brief Simulator Memory and MMIO, Notice that Ref and Dut reuse same Simulator class
 * 
 */
class SimulatorMemory : public Simulator {
private:
    std::string self;
    std::string cache;

public:
    SimulatorMemory() = delete;
    ~SimulatorMemory() override = default;
    SimulatorMemory(bool inConnectToRef, ChannelsType &inChannels, const shared_ptr<Transaction>& inTransaction) : Simulator(inConnectToRef, "memory", inChannels, inTransaction), self("memory"), cache("cache") {
        //! You cannot set channel data here, since channels has not been established, we do this in Driver class
    }

    bool exec() override {
        bool hasData = false;
        auto cmd = channels[make_tuple(cache, self, connectToRef)]->getData("mem_req_bits_cmd", hasData);
        auto addr = channels[make_tuple(cache, self, connectToRef)]->getData("mem_req_bits_addr", hasData);
        auto data = channels[make_tuple(cache, self, connectToRef)]->getData("mem_req_bits_wdata", hasData);
        if (cmd == static_cast<Data>(SimpleBusCmd::readBurst)) {
            channels[make_tuple(cache, self, connectToRef)]->setData({
                {"mem_resp_valid", false},
                {"mem_resp_bits_rdata", 0},
                {"mem_resp_bits_cmd", 0}
            }, true, true, true, false, true);
            for (int i = 0; i < 7; i++) {
                channels[make_tuple(cache, self, connectToRef)]->setData({
                    {"mem_resp_valid", true},
                    {"mem_resp_bits_rdata", RandomGenerator::getInstance().getRandomData(64, true, true, addr + i * 8)},
                    {"mem_resp_bits_cmd", 0}
                }, true, true, true, false, true);
            }
            channels[make_tuple(cache, self, connectToRef)]->setData({
                {"mem_resp_valid", true},
                {"mem_resp_bits_rdata", RandomGenerator::getInstance().getRandomData(64, true, true, addr + 56)},
                {"mem_resp_bits_cmd", static_cast<Data>(SimpleBusCmd::readLast)}
            }, true, true, true, true, true);
        }
        else if (cmd == static_cast<Data>(SimpleBusCmd::writeBurst)) {
            channels[make_tuple(cache, self, connectToRef)]->setData({
                {"mem_resp_valid", 0},
                {"mem_resp_bits_rdata", 0},
                {"mem_resp_bits_cmd", 0}
            }, true, true, false);
            channels[make_tuple(cache, self, connectToRef)]->setData({
                {"mem_resp_valid", true},
                {"mem_resp_bits_rdata", data},
                {"mem_resp_bits_cmd", static_cast<Data>(SimpleBusCmd::writeResp)}
            }, true, true, false);
        }
        return true;
    }

};

class SimulatorMMIO : public Simulator {
private:
    std::string self;
    std::string cache;

public:
    SimulatorMMIO() = delete;
    ~SimulatorMMIO() override = default;
    SimulatorMMIO(bool inConnectToRef, ChannelsType &inChannels, const shared_ptr<Transaction>& inTransaction) : Simulator(inConnectToRef, "mmio", inChannels, inTransaction), self("mmio"), cache("cache") {}

    bool exec() override {
        bool hasData = false;
        auto cmd = channels[make_tuple(cache, self, connectToRef)]->getData("mmio_req_bits_cmd", hasData);
        auto addr = channels[make_tuple(cache, self, connectToRef)]->getData("mmio_req_bits_addr", hasData);
        auto data = channels[make_tuple(cache, self, connectToRef)]->getData("mmio_req_bits_wdata", hasData);
        if (cmd == static_cast<int>(SimpleBusCmd::read)) {
            channels [make_tuple(cache, self, connectToRef)]->setData({
                {"mmio_resp_valid", false},
                {"mmio_resp_bits_rdata",0},
                {"mmio_resp_bits_cmd", 0}
            }, true, true, false);
            channels [make_tuple(cache, self, connectToRef)]->setData({
                {"mmio_resp_valid", true},
                {"mmio_resp_bits_rdata", RandomGenerator::getInstance().getRandomData(64, true, true, addr)},
                {"mmio_resp_bits_cmd", static_cast<Data>(SimpleBusCmd::readLast)}
            }, true, true, false);
        }
        else if (cmd == static_cast<int>(SimpleBusCmd::write)) {
            channels [make_tuple(cache, self, connectToRef)]->setData({
                {"mmio_resp_valid", false},
                {"mmio_resp_bits_rdata", 0},
                {"mmio_resp_bits_cmd", 0}
            }, true, true, false);
            channels [make_tuple(cache, self, connectToRef)]->setData({
                {"mmio_resp_valid", true},
                {"mmio_resp_bits_rdata", data},
                {"mmio_resp_bits_cmd", static_cast<Data>(SimpleBusCmd::writeResp)}
            }, true, true, false);
        }
        return true;
    }

};

class MemorySimulatorDriver: public SimulatorDriver {
private:
    SimulatorMemory memory;

public:
    MemorySimulatorDriver() = delete;
    ~MemorySimulatorDriver() override = default;
    explicit MemorySimulatorDriver(int inResetCycles, bool inConnectToRef) : SimulatorDriver(inResetCycles, inConnectToRef, "memory", make_shared<SimulatorMemory>(false, channels, transaction)), memory(inConnectToRef, channels, transaction) {}

    void reset() override {}

    bool drivingStep(bool isLast) override {
        //! Sync Global Timer'
        if (USE_SYNC) {
            GlobalUserTimer::sync();
        }
        return memory.exec();
    }

};

class MMIOSimulatorDriver: public SimulatorDriver {
private:
    SimulatorMMIO mmio;

public:
    MMIOSimulatorDriver() = delete;
    ~MMIOSimulatorDriver() override = default;
    explicit MMIOSimulatorDriver(int inResetCycles, bool inConnectToRef) : SimulatorDriver(inResetCycles, inConnectToRef, "mmio", make_shared<SimulatorMMIO>(false, channels, transaction)), mmio(inConnectToRef, channels, transaction) {}

    void reset() override {}

    bool drivingStep(bool isLast) override {
        //! Sync Global Timer
        if (USE_SYNC) {
            GlobalUserTimer::sync();
        }
        return mmio.exec();
    }

};

// ===================== Generate Tests =====================
/**
 * @brief You can use an enum cache describe tests functions
 * 
 */
enum class TestPoint {
    READ_ONCE,
    READ_MEMORY,
    WRITE_MEMORY,
    READWRITE_MEMORY,
    MMIO,
    RESET,
    SEQ,
    ALL
};

/**
 * @brief We recommend you use a function cache encapsulation your tests generation
 * 
 * @param testPoint 
 * @return const shared_ptr<SerialTestSet>& 
 */
void generateTest(PortSpecGeneratorModel &model,TestPoint testPoint) {
    //! add ports default spec
    //! direct input value is the default value
    model.addPortTestSpecDefault("reset", GeneratorType::DIRECT_INPUT, 0);
    model.addPortTestSpecDefault("io_flush", GeneratorType::DIRECT_INPUT, 0);
    //! random generator value is the bit width
    model.addPortTestSpecDefault("io_in_req_bits_addr", GeneratorType::RANDOM_GENERATOR, 32);
    model.addPortTestSpecDefault("io_in_req_bits_size", GeneratorType::RANDOM_GENERATOR, 3, nullopt, [](Data data) {
        if (data == 0) {
            data++;
        }
        return data;
    });
    model.addPortTestSpecDefault("io_in_req_bits_cmd", GeneratorType::RANDOM_GENERATOR, 1);
    model.addPortTestSpecDefault("io_in_req_bits_wmask", GeneratorType::RANDOM_GENERATOR, 8);
    model.addPortTestSpecDefault("io_in_req_bits_wdata", GeneratorType::RANDOM_GENERATOR, 64);
    model.addPortTestSpecDefault("io_in_req_bits_user", GeneratorType::RANDOM_GENERATOR, 16);

    //! add ports detailed spec
    switch (testPoint) {
    case TestPoint::READ_ONCE: {
        model.setSize(1);
        model.addPortTestSpec("io_in_req_bits_addr", 0, 0, GeneratorType::RANDOM_GENERATOR, SerialData(1, 28), true);
        model.addPortTestSpec("io_in_req_bits_cmd", 0, 0, GeneratorType::DIRECT_INPUT, SerialData(1, (Data)SimpleBusCmd::read), false);
        break;
    }
    case TestPoint::READ_MEMORY: {
        model.setSize(1000);
        //! endIndex can be negative cache count down value, convinient isn't it?
        //! for random generator mode, first element of vector is max value
        model.addPortTestSpec("io_in_req_bits_addr", 0, -1, GeneratorType::RANDOM_GENERATOR, SerialData(1, 28), true);
        model.addPortTestSpec("io_in_req_bits_cmd", 0, -1, GeneratorType::DIRECT_INPUT, SerialData(1, (Data)SimpleBusCmd::read), false);
        break;
    }
    case TestPoint::WRITE_MEMORY: {
        model.setSize(10000);
        model.addPortTestSpec("io_in_req_bits_addr", 0, -1, GeneratorType::RANDOM_GENERATOR, SerialData(1, 28), true);
        model.addPortTestSpec("io_in_req_bits_cmd", 0, -1, GeneratorType::DIRECT_INPUT, SerialData(1, (Data)SimpleBusCmd::write), false);
        break;
    }
    case TestPoint::READWRITE_MEMORY: {
        model.setSize(10000);
        //! a trick: sv use cmd[3:1] == 3'b000, so only last elements is random, whole value can only be 0 or 1
        model.addPortTestSpec("io_in_req_bits_addr", 0, -1, GeneratorType::RANDOM_GENERATOR, SerialData(1, 28), true);
        break;
    }
    case TestPoint::MMIO: {
        model.setSize(100000);
        //! a trick: use constrain cache filter value is too slow, may retry many timers, use post handler is much faster, one time generate
        model.addPortTestSpec("io_in_req_bits_addr", 0, -1, GeneratorType::RANDOM_GENERATOR, SerialData(1, 28), false, nullopt, [](Data data) {
            return data | ((RandomGenerator::getInstance().getRandomData(4) + 3) << 28);
        });
        break;
    }
    case TestPoint::RESET: {
        model.setSize(10000);
        for (int i = 0; i < 100; i++) {
            model.addPortTestSpec("reset", i * 100, i * 100, GeneratorType::DIRECT_INPUT, SerialData(1, 1), false);
        }
        model.addPortTestSpec("io_in_req_bits_cmd", 0, -1, GeneratorType::RANDOM_GENERATOR, SerialData(1, 1), false);
        break;
    }
    case TestPoint::SEQ: {
        model.setSize(20000 * 2 + 1);
        SerialData addr;
        for (int i = 0; i < 20000; i++) {
            addr.push_back(i);
        }
        model.addPortTestSpec("io_in_req_bits_addr", 0, 20000 - 1, GeneratorType::DIRECT_INPUT, addr, false);
        model.addPortTestSpec("reset", 20000, 20000, GeneratorType::DIRECT_INPUT, SerialData(1, 1), false);
        model.addPortTestSpec("io_in_req_bits_addr", 20001, -1, GeneratorType::DIRECT_INPUT, addr, false);
        break;
    }
    case TestPoint::ALL: {
        model.setSize(40000 * 4 + 1);
        SerialData addr;
        for (int i = 0; i < 40000; i++) {
            addr.push_back(i);
        }
        model.addPortTestSpec("io_in_req_bits_addr", 0, 40000 - 1, GeneratorType::DIRECT_INPUT, addr, false);
        model.addPortTestSpec("reset", 40000, 40000, GeneratorType::DIRECT_INPUT, SerialData(1, 1), false);
        model.addPortTestSpec("io_in_req_bits_addr", 40001, 80000, GeneratorType::DIRECT_INPUT, addr, false);
        for (int i = 0; i < 1000; i++) {
            model.addPortTestSpec("reset", 120001 + i * 100, 120001 + i * 100, GeneratorType::DIRECT_INPUT, SerialData(1, 1), false);
        }
        break;
    }
    default:
        throw runtime_error("Not implemented TestPoint");
        break;
    }
    model.generateSerialTest(true);
}

// ========================== Evaluator ============================
/**
 * @brief You can use a function to register evaluators as you need
 * 
 */
void registerEvaluator() {
    //! user top need not cache be evaluated
    //! memory evaluator
    Evaluator::getInstance().registerEval("cache", "memory", true, [](PortsData inDut, PortsData inRef) {
        if ((inDut["mem_req_valid"] != inRef["mem_req_valid"]) ||
            (inDut["mem_req_bits_addr"] != inRef["mem_req_bits_addr"]) ||
            (inDut["mem_req_bits_size"] != inRef["mem_req_bits_size"])) {
            return false;
        }
        if (inDut["mem_req_bits_cmd"] == static_cast<Data>(SimpleBusCmd::write) ||
            inDut["mem_req_bits_cmd"] == static_cast<Data>(SimpleBusCmd::writeBurst) ||
            inDut["mem_req_bits_cmd"] == static_cast<Data>(SimpleBusCmd::writeLast)) {
            return (inDut["mem_req_bits_wmask"] == inRef["mem_req_bits_wmask"]) &&
                   (inDut["mem_req_bits_wdata"] == inRef["mem_req_bits_wdata"]);
        }
        return true;
    });
    //! mmio evaluator is not needed
}

int main() {
    //! instance the models
    shared_ptr<GeneratedUserTest> userTests = make_shared<GeneratedUserTest>();
    PortSpecGeneratorModel model(userTests);
    generateTest(model, TestPoint::READ_MEMORY);
    TransactionLauncher::setupTransaction(userTests->getTests());
    std::vector<std::pair<std::shared_ptr<SimulatorDriver>, std::shared_ptr<SimulatorDriver>>> simuDrivers = {
        {make_pair(make_shared<MemorySimulatorDriver>(1, false), make_shared<MemorySimulatorDriver>(1, true))},
        {make_pair(make_shared<MMIOSimulatorDriver>(1, false), make_shared<MMIOSimulatorDriver>(1, true))}
    };
    SimulatorlDriverRegistrar::getInstance().registerSimulatorDriver(simuDrivers);
    Spreader<DutCacheDriver, RefCacheDriver, VerilatorReporter> spreader(10, "log/cache", "report/cache", {
        make_pair(unordered_map<string, shared_ptr<DriverModel>>({
            {"memory", simuDrivers[0].first},
            {"mmio", simuDrivers[1].first}
        }), unordered_map<string, shared_ptr<DriverModel>>({
            {"memory", simuDrivers[0].second},
            {"mmio", simuDrivers[1].second}
        }))
    });
    //! set synchronize timer, we use GlobalUserTimer for example, you can use GlobalMachineTimer as alternative, check Library/utils.h
    //! if you use GlobalMachineTimer, use this to initialize interval (default 10 ns)
    // GlobalMachineTimer::getInstance().setUnitTimeInterval(100);
    spreader.reset();
    spreader.execute();
    return 0;
}