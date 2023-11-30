/**
 * @file types.h
 * @author Gao Sion (gaosion2001@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-24
 * 
 * @copyright Copyright (c) Gao Sion, BOSC Institute, 2023
 * 
 */

#pragma once

#include <vector>
#include <unordered_map>
#include <string>

namespace MLVP::Type {
/**
 * @brief All port data support maximum 64 bits, if you need more, please split into multiple wire ports
 * 
 */
typedef unsigned long long Data;

typedef std::vector<Data> SerialData;

/**
 * @brief This one's basic unit is multiple ports
 * @example 
 * {
 *  {
 *     {port1_data, port2_data, port3_data},    //! 1st transaction
 *     {port1_data, port2_data, port3_data},    //! 2nd transaction
 *   }
 * }
 * 
 */
typedef std::vector<SerialData> SerialDatasSet;

/**
 * @brief This one's basic unit is one port multiple data, we use it to compress memory consumption
 * @example
 * {                //! 1st transaction | 2nd transaction
 *  { "port1_name": {trans1_data,        trans2_data} }
 *  { "port2_name": {trans1_data,        trans2_data} }
 * }
 * 
 */
typedef std::unordered_map<std::string, SerialData> SerialTest;

typedef std::unordered_map<std::string, Data> PortsData;

typedef std::vector<SerialTest> SerialTestSet;

} // namespace MLVP::Types