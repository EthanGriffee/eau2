#include <stdlib.h>
#include "../src/dataframe.h"


void test1() {
    DataFrame* x = DataFrame::fromFile("./data/small_datafile.txt");
    x->t_true(x->ncols() == 11);
    x->t_true(x->nrows() == 20);
    x->t_true(x->get_int(0,0) == 1);
    String* bone = new String("Bone");
    x->t_true(x->get_string(2,2)->equals(bone));
    x->t_true(x->get_string(2,6)->equals(bone));
    delete bone;
    delete x;
}



int main() {
    Sys s;
    test1();
    s.OK("Parsing Dataframe works");
    return 0;
}