#include "application.h"
#include "arraytemplate.h"
#include "dataframe.h"


class Trivial : public Application {
 public:
  Trivial(size_t idx) : Application(idx) { }
  void run_() {
    size_t SZ = 1000*1000;
    double sum = 0;
    Array<double>* vals = new Array<double>(SZ);
    for (size_t i = 0; i < SZ; ++i) {
      sum += i;
      vals->add(i);
    }
    Key key("triv",0);
    DataFrame* df = DataFrame::fromArray(&key, &kv, SZ, vals);
    assert(df->get_double(0,1) == 1);
    DataFrame* df2 = kv.get(key);
    df2->p(df2->serialize());
    for (size_t i = 0; i < SZ; ++i) sum -= df2->get_double(0,i);
    assert(sum==0);
    delete vals;
    delete df; delete df2;
  }
};