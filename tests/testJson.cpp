#include "../third-party/include/json.hpp"

#include <iostream>
#include <vector>
#include <string>

using namespace std;
using json = nlohmann::json;


int main() {
    json j;
    j.push_back({{1, 2, 3}, {4, 5, 6}});
    cout << j[0][0][0] << endl;

    return 0;
}