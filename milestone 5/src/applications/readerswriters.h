#pragma once

#include "../dataframe/dataframe.h"

class Writer : public Rower {
  public:

    virtual void visit(Row& r) = 0;

    virtual bool accept(Row& r) {
      if(!done()) {
        this->visit(r);
      }
      return true;
    }

    virtual bool done() {
      return false;
    }
   
};

class Reader : public Rower {
  public:

    virtual bool visit(Row& r) = 0;

    virtual bool accept(Row& r) {
      if(!done()) {
        return visit(r);
      }
      return false;
    }

    virtual bool done() {
      return false;
    }

};

DataFrame*  DataFrame::fromVisitor(Key* in, KDStore* kv, char* schema, Writer& fr) {
  Schema sch(schema);
  Row r(sch);
  DataFrame* returning = new DataFrame(sch);
  while (!fr.done()) {
    fr.visit(r);
    returning->add_row(r);
  }
  kv->put(in, returning);
  return returning;
}