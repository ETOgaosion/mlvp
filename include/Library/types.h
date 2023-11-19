#pragma once

#include <vector>
#include <unordered_map>

namespace MVM {
namespace Type {
/**
 * @brief All port data support maximum 64 bits, if you need more, please split into multiple wire ports
 * 
 */
typedef unsigned long long uint64;

typedef std::vector<uint64> TestData;

/**
 * @brief This one's basic unit is multiple ports
 * @example 
 * {
 *  {
 *     {port1_data, port2_data, port3_data},    // 1st transaction
 *     {port1_data, port2_data, port3_data},    // 2nd transaction
 *   }
 * }
 * 
 */
typedef std::vector<TestData> SerialData;

/**
 * @brief This one's basic unit is one port multiple data, we use it to compress memory consumption
 * @example
 * {                // 1st transaction | 2nd transaction
 *  { "port1_name": {trans1_data,        trans2_data} }
 *  { "port2_name": {trans1_data,        trans2_data} }
 * }
 * 
 */
typedef std::unordered_map<std::string, TestData> SerialTest;

typedef std::unordered_map<std::string, uint64> SerialTestSingle;

typedef std::vector<SerialData> SerialDatasSet;
typedef std::vector<SerialTest> SerialTestSet;

} // namesapce Type

} // namespace MVM