/**
 * @file config.h
 * @author Gao Sion (gaosion2001@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-24
 * 
 * @copyright Copyright (c) Gao Sion, BOSC Institute, 2023
 * 
 */

#pragma once

#include <string>
#include <utility>

namespace MLVP::Config {

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

    
} // namespace MLVP::Config