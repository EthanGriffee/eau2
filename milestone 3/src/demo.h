#pragma once

#include "helper.h"
#include "application.h"
#include "kvstore.h"
#include "arraytemplate.h"
#include "dataframe.h"
#include "networkifc.h"
#include "thread.h"


class Demo : public Application {
public:
  Key main;
  Key verify;
  Key check;
 
  Demo(size_t idx, NetworkIfc& net): Application(idx, net), check("ck",0), verify("verif",0), main("main",0) {
  }
 
  void run_() override {
    switch(this_node()) {
    case 0:   producer();     break;
    case 1:   counter();      break;
    case 2:   summarizer();
   }
  }
 
  void producer() {
    size_t SZ = 100*1000;
    double sum = 0;
    Array<double>* vals = new Array<double>(SZ);
    for (size_t i = 0; i < SZ; ++i) {
      sum += i;
      vals->add(i);
    }
    DataFrame::fromArray(&main, &kv, SZ, vals);
    DataFrame::fromScalar(&check, &kv, sum);
    delete vals;
  }
 
  void counter() {
    DataFrame* v = kv.waitAndGet(main);
    size_t sum = 0;
    for (size_t i = 0; i < 100*1000; ++i) sum += v->get_double(0,i);
    p("The sum is  ").pln(sum);
    DataFrame::fromScalar(&verify, &kv, sum);
  }
 
  void summarizer() {
    DataFrame* result = kv.waitAndGet(verify);
    DataFrame* expected = kv.waitAndGet(check);
    pln(expected->get_double(0,0)==result->get_double(0,0) ? "SUCCESS":"FAILURE");
  }
};

class Threaded_Demo : public Thread {
  public:
    size_t idx;
    NetworkIfc* net;

    Threaded_Demo(size_t idx, NetworkIfc& net) {
      this->idx = idx;
      this->net = &net;
    }

    virtual void run() {
      Demo * d = new Demo(idx, *net);
      d->run_();
    }

};