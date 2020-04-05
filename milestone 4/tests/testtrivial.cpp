#include "../src/applications/trivial.h"

void test1() {
    Trivial x(0);
    x.run_();
    x.OK("Trivial works");
}



int main() {
    test1();
    return 0;
}