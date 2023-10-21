#include "testDef.h"

void B::test() {
}

template <typename T>
void C<T>::test() {
}

int main() {
    C<int> c(1);
    return 0;
}