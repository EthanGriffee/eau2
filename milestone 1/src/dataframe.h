#pragma once

#include "string.h"
#include "object.h"
#include "wrapper.h"
#include "arraytemplate.h"
#include "helper.h"
#include "column.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

class Key; 
class KVStore;
 
/*************************************************************************
 * Schema::
 * A schema is a description of the contents of a data frame, the schema
 * knows the number of columns and number of rows, the type of each column,
 * optionally columns and rows can be named by strings.
 * The valid types are represented by the chars 'S', 'B', 'I' and 'F'.
 */
class Schema : public Object {
 public:
  Array<String*>* types;

 
  /** Copying constructor */
  Schema(Schema& from) {
    types = from.types;
  }
 
  /** Create an empty schema **/
  Schema() {
    types = new Array<String*>();
  }
 
  /** Create a schema from a string of types. A string that contains
    * characters other than those identifying the four type results in
    * undefined behavior. The argument is external, a nullptr argument is
    * undefined. **/
  Schema(const char* types) {
    this->types = new Array<String*>();
    int x = 0;
    while (types[x] != '\0') {
      add_column(x);
      x += 1;
    }
  }
 
  /** Add a column of the given type and name (can be nullptr), name
    * is external. Names are expectd to be unique, duplicates result
    * in undefined behavior. */
  void add_column(char typ) {
    types->add(new String(&typ));
  }
 
  /** Return type of column at idx. An idx >= width is undefined. */
  char col_type(size_t idx) {
    return types->get(idx)->c_str()[0];
  }
 
  /** The number of columns */
  size_t width() {
    return types->getSize();
  }
};
 
/*****************************************************************************
 * Fielder::
 * A field vistor invoked by Row.
 */
class Fielder : public Object {
public:

  /** Called before visiting a row, the argument is the row offset in the
    dataframe. */
  virtual void start(size_t r) {

  }
 
  /** Called for fields of the argument's type with the value of the field. */
  virtual void accept(bool b) {

  }

  virtual void accept(double f) {

  }

  virtual void accept(int i) {

  }

  virtual void accept(String* s) {

  }

  virtual void accept_(BoolObj* b) {
    accept(b->getBool());
  }

  virtual void accept_(DoubleObj* f) {
    accept(f->getDouble());
  }

  virtual void accept_(IntObj* i) {
    accept(i->getInt());
  }

  virtual void accept_(Object* obj) {
    perror("calling accept on an Object that is not a Bool, Double, Int, or String");
    exit(1);
  }

  virtual void accept_(String* s) {
    accept(s);
  }
 
  /** Called when all fields have been seen. */
  virtual void done() {

  }
};

/*************************************************************************
 * Row::
 *
 * This class represents a single row of data constructed according to a
 * dataframe's schema. The purpose of this class is to make it easier to add
 * read/write complete rows. Internally a dataframe hold data in columns.
 * Rows have pointer equality.
 */
class Row : public Object {
 public:
  Schema* scm;
  Array<Object*>* row_;
 
  /** Build a row following a schema. */
  Row(Schema& scm) {
    this->scm = &scm;
    row_ = new Array<Object*>();
  }

 
  /** Setters: set the given column with the given value. Setting a column with
    * a value of the wrong type is undefined. */
  void set_int(size_t col, int val) {
    if (col_type(col) == 'I') {
      row_->set(new IntObj(val), col);
    }
  }

  void set_double(size_t col, double val) {
    if (col_type(col) == 'F') {
      row_->set(new DoubleObj(val), col);
    }
  }

  void set_bool(size_t col, bool val) {
    if (col_type(col) == 'B') {
      row_->set(new BoolObj(val), col);
    }
  }

  /** The string is external. */
  void set_string(size_t col, String* val) {
    if (col_type(col) == 'S') {
      row_->set(val, col);
    }
  }
 
  /** Getters: get the value at the given column. If the column is not
    * of the requested type, the result is undefined. */
  int get_int(size_t col) {
    IntObj* intobj = dynamic_cast<IntObj*> (row_->get(col));
    if (intobj) {
      return intobj->getInt();
    }
    else {
      perror("calling get_int on a col_indx that is not a int column");
      exit(1);
    }
  }

  bool get_bool(size_t col) {
    BoolObj* boolobj = dynamic_cast<BoolObj*> (row_->get(col));
    if (boolobj) {
      return boolobj->getBool();
    }
    else {
      perror("calling get_bool on a col_indx that is not a bool column");
      exit(1);
    }
  }

  double get_double(size_t col) {
    DoubleObj* doubleobj = dynamic_cast<DoubleObj*> (row_->get(col));
    if (doubleobj) {
      return doubleobj->getDouble();
    }
    else {
      perror("calling get on a get_double that is not a double column");
      exit(1);
    }
  }

  String* get_string(size_t col) {
    String* str = dynamic_cast<String*> (row_->get(col));
    if (str) {
      return str;
    }
    else {
      perror("calling get_string on a col_indx that is not a String column");
      exit(1);
    }
  }
 
  /** Number of fields in the row. */
  size_t width() {
    return row_->getSize();
  }
 
   /** Type of the field at the given position. An idx >= width is  undefined. */
  char col_type(size_t idx) {
    return  scm->col_type(idx);
  }
 
  /** Given a Fielder, visit every field of this row. The first argument is
    * index of the row in the dataframe.
    * Calling this method before the row's fields have been set is undefined. */
  void visit(size_t idx, Fielder& f) {
    for (int x = 0; x < width(); x++) {
      f.accept_(row_->get(x));
    }
    f.done();
  }
 
};
 
/*******************************************************************************
 *  Rower::
 *  An interface for iterating through each row of a data frame. The intent
 *  is that this class should subclassed and the accept() method be given
 *  a meaningful implementation. Rowers can be cloned for parallel execution.
 */
class Rower : public Object {
 public:
  /** This method is called once per row. The row object is on loan and
      should not be retained as it is likely going to be reused in the next
      call. The return value is used in filters to indicate that a row
      should be kept. */
  virtual bool accept(Row& r) {
    return true;
  }
 
  /** Once traversal of the data frame is complete the rowers that were
      split off will be joined.  There will be one join per split. The
      original object will be the last to be called join on. The join method
      is reponsible for cleaning up memory. */
  virtual void join_delete(Rower* other) {
  }
};

/****************************************************************************
 * DataFrame::
 *
 * A DataFrame is table composed of columns of equal length. Each column
 * holds values of the same type (I, S, B, F). A dataframe has a schema that
 * describes it.
 */
class DataFrame : public Object {
 public:
  Schema scm;
  Array<Column*>* columns_;
  Array<Row*>* row_arr_;
 
  /** Create a data frame with the same columns as the given df but with no rows or rownmaes */
  DataFrame(DataFrame& df) {
    Schema scm("");
    Schema scmCoppying = df.get_schema();
    row_arr_ = new Array<Row*>();
    columns_ = new Array<Column*>();
    for (int i = 0; i < df.ncols(); i++) {
      char col_type = scmCoppying.col_type(i);
      scm.add_column(col_type);
      switch(col_type) {
        case 'F':
          columns_->add(new DoubleColumn());
          break;
        case 'B':
          columns_->add(new BoolColumn());
          break;
        case 'I':
          columns_->add(new IntColumn());
          break;
        case 'S':
          columns_->add(new StringColumn());
          break;
      }
    }
  }
 
  /** Create a data frame from a schema and columns. All columns are created
    * empty. */
  DataFrame(Schema& schema) {
    scm = schema;
    columns_ = new Array<Column*>();
    row_arr_ = new Array<Row*>();
    for (int i = 0; i < scm.width(); i++) {
      switch(scm.col_type(i)) {
        case 'F':
          columns_->add(new DoubleColumn());
          break;
        case 'B':
          columns_->add(new BoolColumn());
          break;
        case 'I':
          columns_->add(new IntColumn());
          break;
        case 'S':
          columns_->add(new StringColumn());
          break;
      }
    }
  }
 
  /** Returns the dataframe's schema. Modifying the schema after a dataframe
    * has been created in undefined. */
  Schema& get_schema() {
    return scm;
  }
 
  /** Adds a column this dataframe, updates the schema, the new column
    * is external, and appears as the last column of the dataframe, the
    * name is optional and external. A nullptr colum is undefined. */
  void add_column(Column* col) {
    while (row_arr_->getSize() < col->size()) {
      row_arr_->add(new Row(scm));
    }
    if (row_arr_->getSize() == col->size()) {
      scm.add_column(col->get_type());
      for (int x = 0; x < col->size(); x++) {
        switch (col->get_type()) {
          case 'F':
            row_arr_->get(x)->set_double(ncols(), col->as_double()->get(x));
            break;
          case 'I':
            row_arr_->get(x)->set_int(ncols(), col->as_int()->get(x));
            break;
          case 'S':
            row_arr_->get(x)->set_string(ncols(), col->as_string()->get(x));
            break;
          case 'B':
            row_arr_->get(x)->set_bool(ncols(), col->as_bool()->get(x));
            break;
        }
      }
    columns_->add(col);
    }
    else {
      perror("Tried adding a column without enough rows");
      exit(1);
    }
  }
 
  /** Return the value at the given column and row. Accessing rows or
   *  columns out of bounds, or request the wrong type is undefined.*/
  int get_int(size_t col, size_t row) {
    return columns_->get(col)->as_int()->get(row);
  }

  bool get_bool(size_t col, size_t row) {
    return columns_->get(col)->as_bool()->get(row);
  }

  double get_double(size_t col, size_t row) {
    return columns_->get(col)->as_double()->get(row);

  }

  String*  get_string(size_t col, size_t row) {
    return columns_->get(col)->as_string()->get(row);
  }
 
  /** Set the value at the given column and row to the given value.
    * If the column is not  of the right type or the indices are out of
    * bound, the result is undefined. */
  void set_int(size_t col, size_t row, int val) {
    columns_->get(row)->as_int()->set(col, val);
    row_arr_->get(col)->set_int(row, val);
  }

  void set_bool(size_t col, size_t row, bool val) {
    columns_->get(row)->as_bool()->set(col, val);
    row_arr_->get(col)->set_bool(row, val);
  }

  void set_double(size_t col, size_t row, double val) {
    columns_->get(row)->as_double()->set(col, val);
    row_arr_->get(col)->set_double(row, val);
  }

  void set_string(size_t col, size_t row, String* val) {
    columns_->get(row)->as_string()->set(col, val);
    row_arr_->get(col)->set_string(row, val);
  }
 
  /** Set the fields of the given row object with values from the columns at
    * the given offset.  If the row is not form the same schema as the
    * dataframe, results are undefined.
    */
  void fill_row(size_t idx, Row& row) {
    for(size_t i = 0; i < columns_->getSize(); i++) {
      switch(columns_->get(i)->get_type()) {
        case 'F':
          row.set_double(i, columns_->get(i)->as_double()->get(idx));
          break;
        case 'I':
          row.set_int(i, columns_->get(i)->as_int()->get(idx));
          break;
        case 'S':
          row.set_string(i, columns_->get(i)->as_string()->get(idx));
          break;
        case 'B':
          row.set_bool(i, columns_->get(i)->as_bool()->get(idx));
          break;
      }
    }

  }
 
  /** Add a row at the end of this dataframe. The row is expected to have
   *  the right schema and be filled with values, otherwise undedined.  */
  void add_row(Row& row) {
    Row* r = new Row(get_schema());
    row_arr_->add(r);
    for(size_t i = 0; i < row.width(); i++) {
      Column* curr_column = columns_->get(i);
      switch(columns_->get(i)->get_type()) {
        case 'F': {
          double f = row.get_double(i);
          curr_column->push_back(f);
          r->set_double(i, f);
          break;
        }
        case 'I': {
          int in = row.get_int(i);
          curr_column->push_back(in);
          r->set_int(i, in);
          break;
        }
        case 'B': {
          bool b = row.get_bool(i);
          curr_column->push_back(b);
          r->set_bool(i, b);
          break;
        }
        case 'S': {
          String* s = row.get_string(i);
          curr_column->push_back(s);
          r->set_string(i, s);
          break;
        }
      }
    }
  }
 
  /** The number of rows in the dataframe. */
  size_t nrows() {
    return row_arr_->getSize();
  }
 
  /** The number of columns in the dataframe.*/
  size_t ncols() {
    return columns_->getSize();
  }
 
  /** Visit rows in order   */
  void map(Rower& r) {
    for (int i = 0; i < nrows(); i++) {
      r.accept(*row_arr_->get(i));
    }
  }
 
  /** Create a new dataframe, constructed from rows for which the given Rower
    * returned true from its accept method. */
  DataFrame* filter(Rower& r) {
    DataFrame* n = new DataFrame(scm);
    for (int i = 0; i < nrows(); i++) {
      Row row = *row_arr_->get(i);
      if (r.accept(row)) {
        Sys x;
        x.p("fdsfds");
        
        n->add_row(row);
      }
    }
    return n;
  }

  static DataFrame* fromArray(Key* key, KVStore* store, size_t  size, double* vals) {
    for (int x = 0; x < size; x++) { 
      
    }

  }

  static DataFrame* fromScalar(Key* key, KVStore* store, double sum) {

  }

  static DataFrame* fromFile(char* filename) {
    Schema s;
    DataFrame* df = new  DataFrame(s);
    FILE* file = fopen(filename, "r");
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    SorParser* parser = new SorParser{file, 0, file_size, file_size};
    parser->guessSchema();
    parser->parseFile();
    ColumnSet* set = parser->getColumnSet();
    for (int x = 0; x < set->getLength(); x++) {
      df->add_column(set->getColumn(x));
    }
    delete parser;
    return df;
  }
};