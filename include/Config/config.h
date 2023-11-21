#pragma once

#include <string>
#include <utility>

namespace MVM::Config {

class Config
{
private:
    /* data */
    std::string programPath;
    std::string configPath;
    

public:
    Config() = delete;
    

    explicit Config(std::string inConfigPath) : configPath(std::move(inConfigPath)) {}
};

    
} // namespace MVM::Config