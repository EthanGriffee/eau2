#pragma once

#include "string.h"
#include "object.h"
#include "array.h"
#include "helper.h"
#include "string_to_char_map.h"
#include "column.h"
#include "kvstore.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

class RowArray;
class ColumnArray;

/**
 * Incomplete implementation of Column Array. No methods overriden from
 * Array. Created for testing purposes, so we can design tests. 
 **/
class ColumnArray : public Object {
  public:
      Array* col_arr;

      /**
       * Default constructor which will set the initial max-capacity to the array to 10. 
       **/
      ColumnArray() {
          col_arr = new Array();
      }

      /**
       * Initalized this array with the characteristics of the passed in array.
       * @arg arr Array containing values to be used in initialization. 
       **/
      ColumnArray(ColumnArray* arr) {
          col_arr = arr->col_arr;
      }

      /**
       * Destructor which will free the memory of the underlying array as well. 
       **/
      ~ColumnArray() {
          delete col_arr;
      }

      /**
       * Will return the Column pointer at the provided index. if the index is invalid, 
       * then the method will return NULL.
       * @arg index Location to get the value in the array at. 
       **/
      Column* get(size_t index) {
          return static_cast <Column*> (col_arr->get(index));
      }


      /**
       * Clears the array and reinitializes the underlying array to 10 spots with no elements. 
       * Reinitializes the size to 0. 
       **/
      void clear() {
          col_arr->clear();
      }

      /**
       * Resizing the underlying array. And then copying over the elements to a new array with
       * the updated size. 
       * Default is doubling the array size when the max capacity of the 
       * underlying array less the number of elements is 2. 
       **/
      void resize() {
          col_arr->resize();
      }

      /**
       * Returns the first index of the of given Column pointer. 
       * If the pointer is not a Column, return -1. 
       * If the Column pointer is not found then -1 is returned.
       * @arg to_find Column to find the index of. 
       **/
      int indexOf(Object* to_find) {
          return col_arr->indexOf(to_find);
      }

      /**
       * Adds the element provided to the end of the list, unless the given element is NULL, 
       * then it will not be added. The size is incremented by 1. If resizing the array is necessary, 
       * then that should be done.
       * @arg to_add Object to be added to the array. 
       **/
      void add(Column* to_add) {
          return col_arr->add(to_add);
      }

  /**
   * Adds the provided array to the end of the list, unless the given array is NULL, 
   * then it will not be added. 
   * Assuming a valid move, the size of this array is incremented by the size of the 
   * added array. If resizing the array is necessary, then that should be done.
   * @arg to_add Array of Column that all need to be added to this array. 
   **/
    void addAll(ColumnArray* to_add) {
        return col_arr->addAll(to_add->col_arr);
    }

  /**
   * Adds the provided Column at the given index, unless the given object is NULL, 
   * then it will not be added. Otherwise, all elements previously at the index and
   * to the right will be shifted accordingly. 
   * If the index is invalid, then nothing will be added/shifted. 
   * The size of this array is incremented by 1. 
   * If resizing the array is necessary, then that should be done.
   * If the object provided is not a Column, then do nothing.  
   * @arg to_add Object to be added to the array
   * @arg index Location to add the Object at
   **/
    void add(Column* to_add, size_t index) {
        return col_arr->add(to_add, index);
    }

  /**
   * Adds all the elements of the provided array at the given index, 
   * unless the given array is NULL, then it will not be added. Otherwise, 
   * all elements previously at the index and
   * to the right will be shifted accordingly by the size of the procided array, 
   * If the index is invalid, then nothing will be added/shifted.
   * Assuming a valid move, the size of this array is incremented by the size of the 
   * added array.  If resizing the array is necessary, then that should be done.
   * @arg to_add Array of Objects to be added to the array
   * @arg index Location to add the objects to the array at
   **/
    void addAll(ColumnArray* to_add, size_t index) {
      col_arr->addAll(to_add->col_arr, index);
    }

  /**
   * Returns the subarray starting from the provided index to the end of the array. 
   * If the index is invalid, then the method returns NULL
   * @arg index Starting place for the subarray
   **/
    ColumnArray* subArray(size_t index) {
      ColumnArray* returning = new ColumnArray();
      if (index > getSize()) {
          return NULL;
      }
      while(index < getSize()) {
          returning->add(get(index));
          index += 1;
      }
      return returning;
    }

  /**
   * Returns the subarray starting from the provided index to the ending index
   * The starting index must always be greater than the ending index. If either index is invalid, 
   * then NULL is returned. The set is [start, end)
   * @arg startIndex starting place for the subarray
   * @arg endIndex location of the last object to be put in the subarray
   **/
    ColumnArray* subArray(size_t startIndex, size_t endIndex) {
      ColumnArray* returning = new ColumnArray();
      if (endIndex > getSize() || startIndex > endIndex) {
          return NULL;
      }
      while(startIndex < endIndex) {
          returning->add(get(startIndex));
          startIndex += 1;
      }
      return returning;
    }

  /**
   * Removes the first instance of the given Object in this array. If nothing 
   * is found, then no action will occur. The size reduces by 1 if the 
   * element is found.
   * If the object to be removed is not a Column, do nothing. 
   * @arg to_remove Column to be removed from the array
   **/
    void remove(Object* to_remove) {
        col_arr->remove(to_remove);
    }

  /**
   * Removes all instances of the given Object in this array. If nothing 
   * is found, then no action will occur. The size reduces the number of found
   * elements there are.
   * If the object to remove is not a Column, do nothing. 
   * @arg to_remove Column that all instances in the array will be removed of
   **/
    void removeAll(Object* to_remove) {
        col_arr->removeAll(to_remove);
    }

  /**
   * Returns number of elements in the array. 
   **/
    size_t getSize() {
        return col_arr->getSize();
    }

  /**
   * Overriding the Column equals method. 
   * Returns if all the elements in this array and the given object are equal and 
   * in the same other. 
   * If the given object is NULL or not an array, then false will be returned.
   * @arg other Object to check if this array is equal to
   **/
  bool equals(Object* other) {
      ColumnArray* o = dynamic_cast<ColumnArray*> (other);
      return o->col_arr->equals(this->col_arr);
  }

  /**
   * Overriding the Object hash_me() method. 
   * Hashes the array based on user specifications. Default implementation is
   * to hash all internal elements and sum them up. 
   **/
  size_t hash_me_() {
      return col_arr->hash() + 1;
  }

};
 
 
/*************************************************************************
 * Schema::
 * A schema is a description of the contents of a data frame, the schema
 * knows the number of columns and number of rows, the type of each column,
 * optionally columns and rows can be named by strings.
 * The valid types are represented by the chars 'S', 'B', 'I' and 'F'.
 */
class Schema : public Object {
 public:
  StringArray* types;

 
  /** Copying constructor */
  Schema(Schema& from) {
    types = from.types;
  }
 
  /** Create an empty schema **/
  Schema() {
    types = new StringArray();
  }
 
  /** Create a schema from a string of types. A string that contains
    * characters other than those identifying the four type results in
    * undefined behavior. The argument is external, a nullptr argument is
    * undefined. **/
  Schema(const char* types) {
    this->types = new StringArray();
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
    return *(types->get(idx))->c_str();
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
  Array* row_;
 
  /** Build a row following a schema. */
  Row(Schema& scm) {
    this->scm = &scm;
    row_ = new Array();
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

/**
 * Incomplete implementation of Row Array. No methods overriden from
 * Array. Created for testing purposes, so we can design tests. 
 **/
class RowArray : public Object {
  public:
      Array* str_arr;

      /**
       * Default constructor which will set the initial max-capacity to the array to 10. 
       **/
      RowArray() {
          str_arr = new Array();
      }

      /**
       * Initalized this array with the characteristics of the passed in array.
       * @arg arr Array containing values to be used in initialization. 
       **/
      RowArray(RowArray* arr) {
          str_arr = arr->str_arr;
      }

      /**
       * Destructor which will free the memory of the underlying array as well. 
       **/
      ~RowArray() {
          delete str_arr;
      }

      /**
       * Will return the Row pointer at the provided index. if the index is invalid, 
       * then the method will return NULL.
       * @arg index Location to get the value in the array at. 
       **/
      Row* get(size_t index) {
          return static_cast <Row*> (str_arr->get(index));
      }


      /**
       * Clears the array and reinitializes the underlying array to 10 spots with no elements. 
       * Reinitializes the size to 0. 
       **/
      void clear() {
          str_arr->clear();
      }

      /**
       * Resizing the underlying array. And then copying over the elements to a new array with
       * the updated size. 
       * Default is doubling the array size when the max capacity of the 
       * underlying array less the number of elements is 2. 
       **/
      void resize() {
          str_arr->resize();
      }

      /**
       * Returns the first index of the of given Row pointer. 
       * If the pointer is not a Row, return -1. 
       * If the Row pointer is not found then -1 is returned.
       * @arg to_find Row to find the index of. 
       **/
      int indexOf(Object* to_find) {
          return str_arr->indexOf(to_find);
      }

      /**
       * Adds the element provided to the end of the list, unless the given element is NULL, 
       * then it will not be added. The size is incremented by 1. If resizing the array is necessary, 
       * then that should be done.
       * @arg to_add Object to be added to the array. 
       **/
      void add(Row* to_add) {
          return str_arr->add(to_add);
      }

  /**
   * Adds the provided array to the end of the list, unless the given array is NULL, 
   * then it will not be added. 
   * Assuming a valid move, the size of this array is incremented by the size of the 
   * added array. If resizing the array is necessary, then that should be done.
   * @arg to_add Array of Row that all need to be added to this array. 
   **/
    void addAll(RowArray* to_add) {
        return str_arr->addAll(to_add->str_arr);
    }

  /**
   * Adds the provided Row at the given index, unless the given object is NULL, 
   * then it will not be added. Otherwise, all elements previously at the index and
   * to the right will be shifted accordingly. 
   * If the index is invalid, then nothing will be added/shifted. 
   * The size of this array is incremented by 1. 
   * If resizing the array is necessary, then that should be done.
   * If the object provided is not a Row, then do nothing.  
   * @arg to_add Object to be added to the array
   * @arg index Location to add the Object at
   **/
    void add(Row* to_add, size_t index) {
        return str_arr->add(to_add, index);
    }

  /**
   * Adds all the elements of the provided array at the given index, 
   * unless the given array is NULL, then it will not be added. Otherwise, 
   * all elements previously at the index and
   * to the right will be shifted accordingly by the size of the procided array, 
   * If the index is invalid, then nothing will be added/shifted.
   * Assuming a valid move, the size of this array is incremented by the size of the 
   * added array.  If resizing the array is necessary, then that should be done.
   * @arg to_add Array of Objects to be added to the array
   * @arg index Location to add the objects to the array at
   **/
    void addAll(RowArray* to_add, size_t index) {
      str_arr->addAll(to_add->str_arr, index);
    }

  /**
   * Returns the subarray starting from the provided index to the end of the array. 
   * If the index is invalid, then the method returns NULL
   * @arg index Starting place for the subarray
   **/
    RowArray* subArray(size_t index) {
      RowArray* returning = new RowArray();
      if (index > getSize()) {
          return NULL;
      }
      while(index < getSize()) {
          returning->add(get(index));
          index += 1;
      }
      return returning;
    }

  /**
   * Returns the subarray starting from the provided index to the ending index
   * The starting index must always be greater than the ending index. If either index is invalid, 
   * then NULL is returned. The set is [start, end)
   * @arg startIndex starting place for the subarray
   * @arg endIndex location of the last object to be put in the subarray
   **/
    RowArray* subArray(size_t startIndex, size_t endIndex) {
      RowArray* returning = new RowArray();
      if (endIndex > getSize() || startIndex > endIndex) {
          return NULL;
      }
      while(startIndex < endIndex) {
          returning->add(get(startIndex));
          startIndex += 1;
      }
      return returning;
    }

  /**
   * Removes the first instance of the given Object in this array. If nothing 
   * is found, then no action will occur. The size reduces by 1 if the 
   * element is found.
   * If the object to be removed is not a Row, do nothing. 
   * @arg to_remove Row to be removed from the array
   **/
    void remove(Object* to_remove) {
        str_arr->remove(to_remove);
    }

  /**
   * Removes all instances of the given Object in this array. If nothing 
   * is found, then no action will occur. The size reduces the number of found
   * elements there are.
   * If the object to remove is not a Row, do nothing. 
   * @arg to_remove Row that all instances in the array will be removed of
   **/
    void removeAll(Object* to_remove) {
        str_arr->removeAll(to_remove);
    }

  /**
   * Returns number of elements in the array. 
   **/
    size_t getSize() {
        return str_arr->getSize();
    }

  /**
   * Overriding the Row equals method. 
   * Returns if all the elements in this array and the given object are equal and 
   * in the same other. 
   * If the given object is NULL or not an array, then false will be returned.
   * @arg other Object to check if this array is equal to
   **/
  bool equals(Object* other) {
      RowArray* o = dynamic_cast<RowArray*> (other);
      return o->str_arr->equals(this->str_arr);
  }

  /**
   * Overriding the Object hash_me() method. 
   * Hashes the array based on user specifications. Default implementation is
   * to hash all internal elements and sum them up. 
   **/
  size_t hash_me_() {
      return str_arr->hash() + 1;
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
  ColumnArray* columns_;
  RowArray* row_arr_;
 
  /** Create a data frame with the same columns as the given df but with no rows or rownmaes */
  DataFrame(DataFrame& df) {
    Schema scm("");
    Schema scmCoppying = df.get_schema();
    row_arr_ = new RowArray();
    columns_ = new ColumnArray();
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
    columns_ = new ColumnArray();
    row_arr_ = new RowArray();
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
  void add_column(Column* col, String* name) {
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



};