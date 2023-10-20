#pragma once

#include <string>

namespace MVM {
namespace Config {

class Config
{
private:
    /* data */
    std::string programPath;
    

public:
    Config() = delete;
    ~Config() = default;

    Config(std::string configPath);
};

    
} // namespace Config

    
} // namespace MVM