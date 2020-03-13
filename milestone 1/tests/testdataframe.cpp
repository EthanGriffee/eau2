#include <stdlib.h>
#include "../src/dataframe.h"


void test1() {
    DataFrame* x = DataFrame::fromFile("data/datafile.txt");
    x->t_true(x->ncols() == 11);
    x->t_true(x->nrows() == 3466664);
    delete x;
}



int main() {
    test1();
    return 0;
}