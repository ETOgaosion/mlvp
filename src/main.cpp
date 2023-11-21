#include "Channel/channel.h"
#include "Transaction/transaction.h"
#include "mvm.h"
#include "MCVPack/BareDut/NutshellCache/Vnutshellcache.h"

// ========================= Announcer =========================

class DutCacheDriver : public DutUnitDriver {
private:
    const unique_ptr<Vnutshellcache> top;

public:
    DutCacheDriver(int inDriverID, const string& inLogPath) : DutUnitDriver("cache", inDriverID, inLogPath), top(std::make_unique<Vnutshellcache>(contextp.get(), "top")) {
        contextp->debug(0);
        //!< [notice] > randReset(other value) can cause error, because our first posedge is delayed 1 cycle
        contextp->randReset(0);
        top->clock = 1;
        top->trace(tfp.get(), 99);
        //!< this should be called after trace
        tfp->open((logPath + "/memory.vcd").c_str());
        ChannelRegistrar::getInstance().registerChannel("cacheDut", "cacheRef", false);
    }

    /**
     * @brief just execute one cycle
     */
    bool drivingStep(bool isLast) override {
        if(!contextp->gotFinish()) {
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
            //!< you'd better add a check to Channel, or directly get data if you can promise that the data is ready
            if (!ChannelRegistrar::getInstance().hasChannelData("memory", "cache", false, "io_in_resp_ready")) {
                throw runtime_error("io_in_resp_ready is not ready");
            }
            top->io_out_mem_req_ready = ChannelRegistrar::getInstance().getData("memory", "cache", false, "io_out_mem_req_ready");

            if (transaction->checkResponseExistence("memroy", "cache", 0, false)) {
                auto resp = transaction->getResponse("memroy", "cache", 0, false);
                top->io_out_mem_resp_valid = resp.outSignal["io_out_mem_resp_valid"];
                top->io_out_mem_resp_bits_cmd = resp.outSignal["io_out_mem_resp_bits_cmd"];
                top->io_out_mem_resp_bits_rdata = resp.outSignal["io_out_mem_resp_bits_rdata"];
            }

            //!< coh_if
            top->io_out_coh_req_valid = 0;
            top->io_out_coh_req_bits_addr = 0;
            top->io_out_coh_req_bits_size = 0;
            top->io_out_coh_req_bits_cmd = 0;
            top->io_out_coh_req_bits_wmask = 0;
            top->io_out_coh_req_bits_wdata = 0;
            top->io_out_coh_resp_ready = 0;

            //!< mmio_if
            if (!ChannelRegistrar::getInstance().hasChannelData("mmio", "cache", false, "io_mmio_req_ready")) {
                throw runtime_error("io_mmio_req_ready is not ready");
            }
            top->io_mmio_req_ready = ChannelRegistrar::getInstance().getData("mmio", "cache", false, "io_mmio_req_ready");

            if (transaction->checkResponseExistence("mmio", "cache", 0, false)) {
                auto resp = transaction->getResponse("mmio", "cache", 0, false);
                top->io_mmio_resp_valid = resp.outSignal["io_mmio_resp_valid"];
                top->io_mmio_resp_bits_cmd = resp.outSignal["io_mmio_resp_bits_cmd"];
                top->io_mmio_resp_bits_rdata = resp.outSignal["io_mmio_resp_bits_rdata"];
            }

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
                transaction->setOutSignal("io_out_mem_req_valid", top->io_out_mem_req_valid, false);
                transaction->setOutSignal("io_out_mem_req_bits_addr", top->io_out_mem_req_bits_addr, false);
                transaction->setOutSignal("io_out_mem_req_bits_size", top->io_out_mem_req_bits_size, false);
                transaction->setOutSignal("io_out_mem_req_bits_cmd", top->io_out_mem_req_bits_cmd, false);
                transaction->setOutSignal("io_out_mem_req_bits_wmask", top->io_out_mem_req_bits_wmask, false);
                transaction->setOutSignal("io_out_mem_req_bits_wdata", top->io_out_mem_req_bits_wdata, false);
                transaction->transactionItems.setDone(false);
            }

            //!< mmio_if
            if (top->io_mmio_req_ready && top->io_mmio_req_valid) {
                transaction->setOutSignal("io_mmio_req_valid", top->io_mmio_req_valid, false);
                transaction->setOutSignal("io_mmio_req_bits_addr", top->io_mmio_req_bits_addr, false);
                transaction->setOutSignal("io_mmio_req_bits_size", top->io_mmio_req_bits_size, false);
                transaction->setOutSignal("io_mmio_req_bits_cmd", top->io_mmio_req_bits_cmd, false);
                transaction->setOutSignal("io_mmio_req_bits_wmask", top->io_mmio_req_bits_wmask, false);
                transaction->setOutSignal("io_mmio_req_bits_wdata", top->io_mmio_req_bits_wdata, false);
                transaction->transactionItems.setDone(false);
            }

            ChannelRegistrar::getInstance().setData("cacheDut", "cacheRef", false, "victimWaymask", top->victimWaymask);

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

// ========================= Verifier =========================
class RefMemory : public Ref {
private:
	/* USER JOIN TODO */
    unsigned short mem[4];

public:
    Data reset;
    Data addr;
    Data wr_en;
    Data rd_en;
    Data wdata;
    Data rdata;

    RefMemory() {
        memset(mem, 0, sizeof(mem));   
    }
    void exec() override {
		/* USER JOIN TODO */
        if (reset) {
            memset(mem, 0, sizeof(mem));
            rdata = 0;
        }
        else {
            if (wr_en) {
                mem[addr] = wdata;
            }
            if (rd_en) {
                rdata = mem[addr];
            }
        }
	}
};


class RefMemoryDriver : public RefUnitDriver {
private:
    unique_ptr<RefMemory> top;

public:
    RefMemoryDriver() = delete;
    ~RefMemoryDriver() = default;
    RefMemoryDriver(shared_ptr<Transaction> inTransaction) : RefUnitDriver(inTransaction), top(make_unique<RefMemory>()) {}

    /* just execute one test */
    bool drivingStep(bool isLast) override {
        return true;
    }
};

int main() {
    //!< Generate Tests
    shared_ptr<GeneratedUserTest> userTests = make_shared<GeneratedUserTest>();

    PortSpecGeneratorModel portSpecGeneratorModel(userTests);
    for (Data i = 1; i < 5; i++) {
        portSpecGeneratorModel.setSize(6);
        portSpecGeneratorModel.addPortTestSpec("reset", 0, 1, GeneratorType::DIRECT_INPUT, {1});
        portSpecGeneratorModel.addPortTestSpec("addr" , 0, 5, GeneratorType::DIRECT_INPUT, {i});
        portSpecGeneratorModel.addPortTestSpec("wr_en", 2, 3, GeneratorType::DIRECT_INPUT, {1});
        portSpecGeneratorModel.addPortTestSpec("rd_en", 4, 5, GeneratorType::DIRECT_INPUT, {1});
        portSpecGeneratorModel.addPortTestSpec("wdata", 0, 5, GeneratorType::DIRECT_INPUT, {i});
        portSpecGeneratorModel.generateSerialTest(true);
    }


    //!< Execution
    TransactionLauncher::setupTransaction(1000, userTests->getTests());
    Spreader<DutMemoryDriver, RefMemoryDriver, VerilatorReporter> spreader("log/memory", "report/memory");
    spreader.execute();
    return 0;
}