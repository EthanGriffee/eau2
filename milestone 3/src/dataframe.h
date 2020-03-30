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
#include "key.h"

class KDStore;
 
/*************************************************************************
 * Schema::
 * A schema is a description of the contents of a data frame, the schema
 * knows the number of columns and number of rows, the type of each column,
 * optionally columns and rows can be named by strings.
 * The valid types are represented by the chars 'S', 'B', 'I' and 'D'.
 */
class Schema : public Object {
 public:
  Array<char>* types;

 
  /** Copying constructor */
  Schema(Schema& from) {
    types = new Array<char>();
    for (int x = 0; x < from.width(); x++) {
      types->add(from.col_type(x));
    }
  }
 
  /** Create an empty schema **/
  Schema() {
    types = new Array<char>();
  }
 
  /** Create a schema from a string of types. A string that contains
    * characters other than those identifying the four type results in
    * undefined behavior. The argument is external, a nullptr argument is
    * undefined. **/
  Schema(const char* types) {
    this->types =  new Array<char>();
    int x = 0;
    while (types[x] != '\0') {
      add_column(types[x]);
      x += 1;
    }
  }
 
  /** Add a column of the given type and name (can be nullptr), name
    * is external. Names are expectd to be unique, duplicates result
    * in undefined behavior. */
  void add_column(char typ) {
    types->add(typ);
  }
 
  /** Return type of column at idx. An idx >= width is undefined. */
  char col_type(size_t idx) {
    return types->get(idx);
  }
 
  /** The number of columns */
  size_t width() {
    return types->getSize();
  }

  char* serialize() {
    StrBuff strbuff;
    char* ser_type = types->serialize();
    strbuff.c("{SCHEMA|types=");
    strbuff.c(ser_type);
    strbuff.c("|}");
    delete[] ser_type;
    return strbuff.get_char();
  }

  static Schema* deserialize(char* s) {
    int x = 14;
    Schema* returning = new Schema();
    char* c = returning->substring(s, 0, x);
    assert(strcmp(c, "{SCHEMA|types=") == 0);
    delete[] c;
    int y = returning->parseUntilClassSeperator(s, x);
    c = returning->substring(s, x, y);
    delete returning->types;
    returning->types = Array<char>::deserialize_chararray(c);
    delete[] c;
    return returning;
  }

  ~Schema() {
    delete types;
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
  Array<Object*> row_;
 
  /** Build a row following a schema. */
  Row(Schema& scm) {
    this->scm = &scm;
  }

 
  /** Setters: set the given column with the given value. Setting a column with
    * a value of the wrong type is undefined. */
  void set_int(size_t col, int val) {
    if (col_type(col) == 'I') {
      row_.set(new IntObj(val), col);
    }
  }

  void set_double(size_t col, double val) {
    if (col_type(col) == 'D') {
      row_.set(new DoubleObj(val), col);
    }
  }

  void set_bool(size_t col, bool val) {
    if (col_type(col) == 'B') {
      row_.set(new BoolObj(val), col);
    }
  }

  /** The string is external. */
  void set_string(size_t col, String* val) {
    if (col_type(col) == 'S') {
      row_.set(val, col);
    }
  }
 
  /** Getters: get the value at the given column. If the column is not
    * of the requested type, the result is undefined. */
  int get_int(size_t col, bool deleting) {
    IntObj* intobj = dynamic_cast<IntObj*> (row_.get(col));
    assert(intobj);
    int returning =  intobj->getInt();
    if (deleting) {
      delete intobj;
    }
    return returning;
  }

  bool get_bool(size_t col, bool deleting) {
    BoolObj* boolobj = dynamic_cast<BoolObj*> (row_.get(col));
    assert(boolobj);
    bool returning =  boolobj->getBool();
    if (deleting) {
      delete boolobj;
    }
    return returning;
  }

  double get_double(size_t col, bool deleting) {
    DoubleObj* doubleobj = dynamic_cast<DoubleObj*> (row_.get(col));
    assert(doubleobj);
    double returning = doubleobj->getDouble();
    if (deleting) {
      delete doubleobj;
    }
    return returning;
  }

  String* get_string(size_t col) {
    String* str = dynamic_cast<String*> (row_.get(col));
    assert(str);
    return str;
  }
 
  /** Number of fields in the row. */
  size_t width() {
    return row_.getSize();
  }
 
   /** Type of the field at the given position. An idx >= width is  undefined. */
  char col_type(size_t idx) {
    return  scm->col_type(idx);
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
  Schema* scm;
  Array<Column*>* columns_;
 
  /** Create a data frame with the same columns as the given df but with no rows or rownmaes */
  DataFrame(DataFrame& df) {
    Schema scm("");
    Schema scmCoppying = df.get_schema();
    columns_ = new Array<Column*>();
    for (int i = 0; i < df.ncols(); i++) {
      char col_type = scmCoppying.col_type(i);
      scm.add_column(col_type);
      switch(col_type) {
        case 'D':
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
    scm = &schema;
    columns_ = new Array<Column*>();
    for (int i = 0; i < scm->width(); i++) {
      switch(scm->col_type(i)) {
        case 'D':
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
    return *scm;
  }
 
  /** Adds a column this dataframe, updates the schema, the new column
    * is external, and appears as the last column of the dataframe, the
    * name is optional and external. A nullptr colum is undefined. */
  void add_column(Column* col) {
    scm->add_column(col->get_type());
    columns_->add(col);
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
    columns_->get(col)->as_int()->set(row, val);
  }

  void set_bool(size_t col, size_t row, bool val) {
    columns_->get(col)->as_bool()->set(row, val);
  }

  void set_double(size_t col, size_t row, double val) {
    columns_->get(col)->as_double()->set(row, val);
  }

  void set_string(size_t col, size_t row, String* val) {
    columns_->get(col)->as_string()->set(row, val);
  }
 
  /** Set the fields of the given row object with values from the columns at
    * the given offset.  If the row is not form the same schema as the
    * dataframe, results are undefined.
    */
  void fill_row(size_t idx, Row& row) {
    for(size_t i = 0; i < columns_->getSize(); i++) {
      switch(columns_->get(i)->get_type()) {
        case 'D':
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
    for(size_t i = 0; i < row.width(); i++) {
      Column* curr_column = columns_->get(i);
      switch(columns_->get(i)->get_type()) {
        case 'D': {
          double f = row.get_double(i, true);
          curr_column->push_back(f);
          break;
        }
        case 'I': {
          int in = row.get_int(i, true);
          curr_column->push_back(in);
          break;
        }
        case 'B': {
          bool b = row.get_bool(i, true);
          curr_column->push_back(b);
          break;
        }
        case 'S': {
          String* s = row.get_string(i);
          curr_column->push_back(s);
          break;
        }
      }
    }
  }
 
  /** The number of rows in the dataframe. */
  size_t nrows() {
    if (columns_->getSize() == 0) {
      return 0;
    }
    return columns_->get(0)->size();
  }
 
  /** The number of columns in the dataframe.*/
  size_t ncols() {
    return columns_->getSize();
  }
 
  /** Visit rows in order   */
  void map(Rower& r) {
    Row row(*this->scm);
    for (int i = 0; i < nrows(); i++) {
      this->fill_row(i, row);
      r.accept(row);
    }
  }
 
  /** Create a new dataframe, constructed from rows for which the given Rower
    * returned true from its accept method. */
  DataFrame* filter(Rower& r) {
    Row row(*this->scm);
    DataFrame* n = new DataFrame(*scm);
    for (int i = 0; i < nrows(); i++) {
      this->fill_row(i, row);
      if (r.accept(row)) {
        n->add_row(row);
      }
    }
    return n;
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

  static DataFrame* fromArray(Key* key, KDStore* kv, size_t size, Array<double>* vals);

  static DataFrame* fromScalar(Key* key, KDStore* kv, double val);


  static DataFrame* deserialize(char* des) {
    Sys s;
    int x = 18;
    int y;

    char* c = s.substring(des, 0, x);
    assert(strcmp(c, "{DATAFRAME|schema=") == 0);
    delete[] c;
    y = s.parseUntilClassSeperator(des, x);
    c = s.substring(des, x, y);
    Schema* scm = Schema::deserialize(c);
    DataFrame* newFrame =  new DataFrame(*scm);
    delete scm;
    x = x + y + 1;
    delete[] c;
    c = s.substring(des, x, 9);
    assert(strcmp(c, "columns_=") == 0);
    delete[] c;
    x = x + 9;
    y = s.parseUntilClassSeperator(des, x);
    c = s.substring(des, x, y);
    for (int i = 0; i < newFrame->ncols(); i++) {
        delete newFrame->columns_->get(i);
      }
    delete newFrame->columns_;
    newFrame->columns_ = Array<Column>::deserialize_columnarray(c);
    delete[] c;
    return newFrame;
  }

  char* serialize() {
    StrBuff strbuff;
    char* ser_schema = get_schema().serialize();
    char* ser_col = columns_->serialize();
    strbuff = strbuff.c("{DATAFRAME|schema=");
    strbuff = strbuff.c(ser_schema);
    strbuff = strbuff.c("|columns_=");
    strbuff = strbuff.c(ser_col);
    strbuff = strbuff.c("}}");
    delete[] ser_schema;
    delete[] ser_col;
    return strbuff.get_char();
  }

  ~DataFrame() {
    for (int i = 0; i < this->ncols(); i++) {
      delete columns_->get(i);
    }
    delete columns_;
  }
};