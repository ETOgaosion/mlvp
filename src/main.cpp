/**
 * @file main.cpp
 * @author Gao Sion (gaosion2001@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */

/**
 * @mainpage MLVP Documentation
 *
 * **MLVP** is a  M**ulti-**L**anguage **V**erification **P**latform, which is a framework for multi-language verification, coding in C++
 *
 * @section intro_sec Introduction
 *
 * Documentation is in the [official documnetation website](https://mlvp-doc.readthedocs.io/zh-cn/latest/)
 *
 * @section arch_sec Architecture
 *
 * @image html MLVP_BareDut.png
 * 
 */

#include "Channel/channel.h"
#include "Drivers/driver.h"
#include "Drivers/driverModel.h"
#include "Drivers/refDriver.h"
#include "Drivers/simulatorDriver.h"
#include "Library/types.h"
#include "RefPack/ref.h"
#include "Transaction/transaction.h"
#include "mlvp.h"
#include "MCVPack/BareDut/NutshellCache/Vnutshellcache.h"
#include <cassert>
#include <random>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

/**
 * @brief SimpleBusCmd
 * 
 */

enum class SimpleBusCmd : Data {
    //!< req
    read        = 0b0000,
    write       = 0b0001,
    readBurst   = 0b0010,
    writeBurst  = 0b0011,
    writeLast   = 0b0111,
    prob        = 0b1000,
    prefetch    = 0b0100,

    //!< resp
    readLast    = 0b0110,
    writeResp   = 0b0101,
    probHit     = 0b1100,
    probeMiss   = 0b1000
};

// ========================= DutUnitDriver =========================
class DutCacheDriver : public DutUnitDriver {
private:
    const unique_ptr<Vnutshellcache> top;

public:
    DutCacheDriver(int inDriverID, const string& inLogPath) : DutUnitDriver("cacheDut", inDriverID, inLogPath), top(make_unique<Vnutshellcache>(contextp.get(), "top")) {
        contextp->debug(0);
        //!< [notice] > randReset(other value) can cause error, because our first posedge is delayed 1 cycle
        contextp->randReset(0);
        top->clock = 1;
        top->trace(tfp.get(), 99);
        //!< this should be called after trace
        tfp->open((logPath + "/memory.vcd").c_str());
    }

    /**
     * @brief just execute one cycle
     */
    bool drivingStep(bool isLast) override {
        if(!contextp->gotFinish()) {
            bool hasData = false;
            contextp->timeInc(1);

            //!< assign input signals
            //!< c_if
            top->clock = !top->clock;
            top->reset = transaction->getInSignal("reset");
            top->io_flush = transaction->getInSignal("io_flush");

            //!< in_if
            top->io_in_req_valid = transaction->getInSignal("io_in_req_valid");
            top->io_in_req_bits_addr = transaction->getInSignal("io_in_req_bits_addr");
            top->io_in_req_bits_size = transaction->getInSignal("io_in_req_bits_size");
            top->io_in_req_bits_cmd = transaction->getInSignal("io_in_req_bits_cmd");
            top->io_in_req_bits_wmask = transaction->getInSignal("io_in_req_bits_wmask");
            top->io_in_req_bits_wdata = transaction->getInSignal("io_in_req_bits_wdata");
            top->io_in_req_bits_user = transaction->getInSignal("io_in_req_bits_user");

            //!< mem_if
            top->io_out_mem_req_ready = channels[make_pair("cacheDut", "memoryDut")]->getData("mem_req_ready", hasData);
            top->io_out_mem_resp_valid = channels[make_pair("cacheDut", "memoryDut")]->getData("mem_resp_valid", hasData);
            top->io_out_mem_resp_bits_cmd = channels[make_pair("cacheDut", "memoryDut")]->getData("mem_resp_bits_cmd", hasData);
            top->io_out_mem_resp_bits_rdata = channels[make_pair("cacheDut", "memoryDut")]->getData("mem_resp_bits_rdata", hasData);

            //!< coh_if
            top->io_out_coh_req_valid = 0;
            top->io_out_coh_req_bits_addr = 0;
            top->io_out_coh_req_bits_size = 0;
            top->io_out_coh_req_bits_cmd = 0;
            top->io_out_coh_req_bits_wmask = 0;
            top->io_out_coh_req_bits_wdata = 0;
            top->io_out_coh_resp_ready = 0;

            //!< mmio_if
            top->io_mmio_req_ready = channels[make_pair("cacheDut", "mmioDut")]->getData("mmio_req_ready", hasData);
            top->io_mmio_resp_valid = channels[make_pair("cacheDut", "mmioDut")]->getData("mmio_resp_valid", hasData);
            top->io_mmio_resp_bits_cmd = channels[make_pair("cacheDut", "mmioDut")]->getData("mmio_resp_bits_cmd", hasData);
            top->io_mmio_resp_bits_rdata = channels[make_pair("cacheDut", "mmioDut")]->getData("mmio_resp_bits_rdata", hasData);

            //!< evaluate model
            top->eval();
            //!< generate trace
            tfp->dump(contextp->time());

            //!< assign output signals
            //!< c_if
            transaction->setOutSignal("io_empty", top->io_empty, false);

            //!< in_if
            transaction->setOutSignal("io_in_req_ready", top->io_in_req_ready, false);
            if (top->io_in_resp_ready && top->io_in_resp_valid) {
                transaction->setOutSignal("io_in_resp_valid", top->io_in_resp_valid, false);
                transaction->setOutSignal("io_in_resp_bits_cmd", top->io_in_resp_bits_cmd, false);
                transaction->setOutSignal("io_in_resp_bits_rdata", top->io_in_resp_bits_rdata, false);
                transaction->setOutSignal("io_in_resp_bits_user", top->io_in_resp_bits_user, false);
                transaction->transactionItems.setDone(false);
            }

            //!< mem_if
            if (top->io_out_mem_req_ready && top->io_out_mem_req_valid) {
                channels[make_pair("cacheDut", "memoryDut")]->setData({
                    {"mem_req_valid", top->io_out_mem_req_valid},
                    {"mem_req_bits_addr", top->io_out_mem_req_bits_addr},
                    {"mem_req_bits_size", top->io_out_mem_req_bits_size},
                    {"mem_req_bits_cmd", top->io_out_mem_req_bits_cmd},
                    {"mem_req_bits_wmask", top->io_out_mem_req_bits_wmask},
                    {"mem_req_bits_wdata", top->io_out_mem_req_bits_wdata}
                }, true, false, false);
            }

            //!< mmio_if
            if (top->io_mmio_req_ready && top->io_mmio_req_valid) {
                channels[make_pair("cacheDut", "mmioDut")]->setData({
                    {"mmio_req_valid", top->io_mmio_req_valid},
                    {"mmio_req_bits_addr", top->io_mmio_req_bits_addr},
                    {"mmio_req_bits_size", top->io_mmio_req_bits_size},
                    {"mmio_req_bits_cmd", top->io_mmio_req_bits_cmd},
                    {"mmio_req_bits_wmask", top->io_mmio_req_bits_wmask},
                    {"mmio_req_bits_wdata", top->io_mmio_req_bits_wdata}
                }, true, false, false);
            }

            channels[make_pair("cacheDut", "cacheRef")]->setData({{"victimWaymask", top->victimWaymask}}, false, false, false);

            if (isLast) {
                //!< don't know why tfp dump lose last cycle
                contextp->timeInc(1);
                top->clock = !top->clock;
                top->eval();
                tfp->dump(contextp->time());
                contextp->timeInc(1);
                top->clock = !top->clock;
                top->eval();
                tfp->dump(contextp->time());
                //!< output coverage
                Verilated::mkdir(logPath.c_str());
                contextp->coveragep()->write((logPath + "/coverage.dat").c_str());
                tfp->close();
                top->final();
            }
            return true;
        }
        //!< reach test end
        return false;
    }
};

// ========================= Ref and RefDriver =========================
/**
 * @brief Ref Cache simulator
 * 
 */
class RefCache : public Ref {
private:
    bool cacheEmpty = false;
    //!< **Notice that C++ vector<bool> implemented different from other vector, cannot use reference**, we use vector<char> instead
    vector<vector<char>> cacheValid = vector<vector<char>>(128, vector<char>(4, 0));
    //!< **Notice that C++ vector<bool> implemented different from other vector, cannot use reference**, we use vector<char> instead
    vector<vector<char>> cacheDirty = vector<vector<char>>(128, vector<char>(4, 0));
    vector<vector<Data>> cacheTag = vector<vector<Data>>(128, vector<Data>(4, 0));
    vector<vector<vector<Data>>> cacheData = vector<vector<vector<Data>>>(128, vector<vector<Data>>(4, vector<Data>(8, 0)));

public:
    RefCache() = delete;
    ~RefCache() override = default;

    RefCache(const shared_ptr<ChannelsType>& inChannels, const shared_ptr<Transaction>& inTransaction) : Ref("cache", inChannels, inTransaction) {}

    void reset() {
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
            (*channels)[make_pair("cacheRef", "memoryRef")]->setData(request, true, false, false);
            assert((*channels)[make_pair("cacheRef", "memoryRef")]->getData("wb_resp_bits_cmd", hasData) == static_cast<Data>(SimpleBusCmd::writeResp));
        }
        request["wb_req_bits_cmd"] = static_cast<Data>(SimpleBusCmd::writeLast);
        request["wb_req_bits_wdata"] = cacheData[setId][wayId][7];
        (*channels)[make_pair("cacheRef", "memoryRef")]->setData(request, true, false, false);
        assert((*channels)[make_pair("cacheRef", "memoryRef")]->getData("wb_resp_bits_cmd", hasData) == static_cast<Data>(SimpleBusCmd::writeResp));
        cacheValid[setId][wayId] = 0;
    }

    void fetch(Data addr, int wayId, int wordId) {
        bool hasData = false;
        Data tag = addr >> 13;
        Data setId = (addr >> 6) & ((0x1 << 7) - 1);
        (*channels)[make_pair("cacheRef", "memoryRef")]->setData({
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
        cacheData[setId][wayId][wordId] = (*channels)[make_pair("cacheRef", "memoryRef")]->getData("mem_resp_bits_rdata", hasData);
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
        //!< betther than sv solution
        for (int i = 0; i < 8; i++) {
            if (bytemask & (0x1 << i)) {
                bitmask |= (0xff << (i * 8));
            }
        }
        return bitmask;
    }

    static int getPacketId(int addr) {
        return (addr >> 3) & ((1 << 3) - 1);
    }

    /**
     * @brief You can use it to handle the whole transaction at one execution, or multi-cycle executions
     * 
     */
    bool exec() override {
        bool hasData = false;
        //!< reset
        if (transaction->getInSignal("reset")) {
            reset();
        }

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

        // mmio
        if (mmioAddr >= 3 && mmioAddr <= 7) {
            (*channels)[make_pair("cacheRef", "mmioRef")]->setData({
                {"mmio_req_valid", 1},
                {"mmio_req_bits_addr", addr},
                {"mmio_req_bits_size", inSize},
                {"mmio_req_bits_cmd", inCmd},
                {"mmio_req_bits_wmask", inWmask},
                {"mmio_req_bits_wdata", inWdata}
            }, true, false, false);
            transaction->setOutSignal("io_in_resp_valid", 1, true);
            transaction->setOutSignal("io_in_resp_bits_cmd", (*channels)[make_pair("cacheRef", "mmioRef")]->getData("mmio_resp_bits_cmd", hasData), true);
            transaction->setOutSignal("io_in_resp_bits_rdata", (*channels)[make_pair("cacheRef", "mmioRef")]->getData("mmio_resp_bits_rdata", hasData), true);
            transaction->setOutSignal("io_in_resp_bits_user", inUser, true);
        }
        

        // check hitness
        Data bitmask = bytesmask2bitsmask((char)inWmask);
        int hitId = -1;
        bool needRefill = false;

        for (int i = 0; i < 4; i++) {
            if (cacheValid[reqSetId][i] && cacheTag[reqSetId][i] == reqTag) {
                hitId = i;
                break;
            }
        }

        //!< miss
        if (hitId == -1) {
            int victimId = -1;
            for (int i = 3; i >= 0; i--) {
                if (!cacheValid[reqSetId][i]) {
                    victimId = i;
                    break;
                }
            }
            //!< need evict
            if (victimId  == -1) {
                victimId = mask2index((int)(*channels)[make_pair("cacheDut", "cacheRef")]->getData("victimWaymask", hasData));
                if (cacheDirty[reqSetId][victimId]) {
                    write_back((int)reqSetId, victimId);
                }
            }
            //!< fetch data
            fetch(addr, victimId, (int)reqWordId);
            needRefill = true;
            hitId = victimId;
        }

        //!< write cache
        if (inCmd == static_cast<Data>(SimpleBusCmd::write) || inCmd == static_cast<Data>(SimpleBusCmd::writeBurst) || inCmd == static_cast<Data>(SimpleBusCmd::writeLast)) {
            cacheDirty[reqSetId][hitId] = 1;
            cacheData[reqSetId][hitId][reqWordId] = (cacheData[reqSetId][hitId][reqWordId] & ~bitmask) | (inWdata & bitmask);
        }

        //!< send response
        transaction->setOutSignal("io_in_resp_valid", 1, true);
        transaction->setOutSignal("io_in_resp_bits_user", inUser, true);
        transaction->setOutSignal("io_empty", cacheEmpty, true);
        if (inCmd == static_cast<Data>(SimpleBusCmd::read) || inCmd == static_cast<Data>(SimpleBusCmd::readBurst)) {
            transaction->setOutSignal("io_in_resp_bits_cmd", static_cast<Data>(SimpleBusCmd::readLast), true);
            transaction->setOutSignal("io_in_resp_bits_rdata", cacheData[reqSetId][hitId][reqWordId], true);
        } else {
            transaction->setOutSignal("io_in_resp_bits_cmd", static_cast<Data>(SimpleBusCmd::writeResp), true);
            transaction->setOutSignal("io_in_resp_bits_rdata", 0, true);
        }

        // !< refill
        if (needRefill) {
            auto pktId = getPacketId((int)addr);
            (*channels)[make_pair("cacheRef", "memoryRef")] -> setData({
                {"mem_req_valid", 1},
                {"mem_req_bits_addr", addr},
                {"mem_req_bits_size", inSize},
                {"mem_req_bits_cmd", inCmd},
                {"mem_req_bits_wmask", inWmask},
                {"mem_req_bits_wdata", inWdata}
            }, true, false, false);
            for (int i = 1; i < 8; i++) {
                pktId = (pktId + 1) % 8;
                cacheData[reqSetId][hitId][pktId] = (*channels)[make_pair("cacheRef", "memoryRef")]->getData("mem_resp_bits_rdata", hasData);
            }
        }
        return true;
    }
};

/**
 * @brief RefUnitDriver, use a ref class to simulate the dut logic
 * 
 */
class RefCacheDriver : public RefUnitDriver {
private:
    RefCache ref;

public:
    RefCacheDriver() : RefUnitDriver("cacheRef"), ref(make_shared<ChannelsType>(channels), transaction) {};
    ~RefCacheDriver() override = default;

    bool drivingStep(bool isLast) override {
        return ref.exec();
    }
};

// ============================= Simulator and Simulator Driver =================================

/**
 * @brief Simulator Memory and MMIO
 * 
 */
class SimulatorMemory : public Simulator {
private:
    mt19937 rng;
    string from;
    string to;

public:
    SimulatorMemory() = delete;
    ~SimulatorMemory() override = default;
    SimulatorMemory(bool inConnectToRef, const shared_ptr<ChannelsType>& inChannels, const shared_ptr<Transaction>& inTransaction) : from(inConnectToRef ? "cacheRef" : "cacheDut"), to(inConnectToRef ? "memoryRef" : "memoryDut"), Simulator(inConnectToRef, inConnectToRef ? "memoryRef" : "memoryDut", inChannels, inTransaction), rng(chrono::system_clock::now().time_since_epoch().count()) {
        (*channels)[make_pair(from, to)]->setData({{"memory_req_ready", true}}, false, false, false);
    }

    bool exec() override {
        bool hasData = false;
        auto cmd = (*channels)[make_pair(from, to)]->getData("mem_req_bits_cmd", hasData);
        auto data = (*channels)[make_pair(from, to)]->getData("mem_req_bits_wdata", hasData);
        if (cmd == static_cast<Data>(SimpleBusCmd::readBurst)) {
            for (int i = 0; i < 7; i++) {
                (*channels)[make_pair(from, to)]->setData({
                    {"mem_resp_valid", true},
                    {"mem_resp_bits_rdata", (Data)rng()},
                    {"mem_resp_bits_cmd", 0}
                }, true, true, true);
            }
            (*channels)[make_pair(from, to)]->setData({
                {"mem_resp_valid", true},
                {"mem_resp_bits_rdata", (Data)rng()},
                {"mem_resp_bits_cmd", static_cast<Data>(SimpleBusCmd::readLast)}
            }, true, true, true);
        }
        else if (cmd == static_cast<Data>(SimpleBusCmd::writeBurst)) {
            (*channels)[make_pair(from, to)]->setData({
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
    mt19937 rng;
    string from;
    string to;


public:
    SimulatorMMIO() = delete;
    ~SimulatorMMIO() override = default;
    SimulatorMMIO(bool inConnectToRef, const shared_ptr<ChannelsType>& inChannels, const shared_ptr<Transaction>& inTransaction) : from(inConnectToRef ? "cacheRef" : "cacheDut"), to(inConnectToRef ? "mmioRef" : "mmioDut"), Simulator(inConnectToRef, inConnectToRef ? "mmioRef" : "mmioDut", inChannels, inTransaction), rng(chrono::system_clock::now().time_since_epoch().count()) {}

    bool exec() override {
        bool hasData = false;
        auto cmd = (*channels)[make_pair(from, to)]->getData("mmio_req_bits_cmd", hasData);
        auto data = (*channels)[make_pair(from, to)]->getData("mem_req_bits_wdata", hasData);
        if (cmd == static_cast<int>(SimpleBusCmd::read)) {
            (*channels) [make_pair(from, to)]->setData({
                {"mmio_resp_valid", true},
                {"mmio_resp_bits_rdata", (Data)rng()},
                {"mmio_resp_bits_cmd", static_cast<Data>(SimpleBusCmd::readLast)}
            }, true, true, false);
        }
        else if (cmd == static_cast<int>(SimpleBusCmd::write)) {
            (*channels) [make_pair(from, to)]->setData({
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
    explicit MemorySimulatorDriver(bool inConnectToRef) : SimulatorDriver(inConnectToRef ? "memoryRef" : "memoryDut", make_shared<SimulatorMemory>(false, channels, transaction)), memory(inConnectToRef, make_shared<ChannelsType>(channels), transaction) {}

    bool drivingStep(bool isLast) override {
        return memory.exec();
    }

};

class MMIOSimulatorDriver: public SimulatorDriver {
private:
    SimulatorMMIO mmio;

public:
    MMIOSimulatorDriver() = delete;
    ~MMIOSimulatorDriver() override = default;
    explicit MMIOSimulatorDriver(bool inConnectToRef) : SimulatorDriver(inConnectToRef ? "mmioRef" : "mmioDut", make_shared<SimulatorMMIO>(false, channels, transaction)), mmio(inConnectToRef, make_shared<ChannelsType>(channels), transaction) {}

    bool drivingStep(bool isLast) override {
        return mmio.exec();
    }

};


int main() {
    return 0;
}