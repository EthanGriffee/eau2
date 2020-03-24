// Lang::CwC
#pragma once

#include <assert.h>
#include <stdio.h>
#include "string.h"
#include "object.h"
#include "arraytemplate.h"

class BoolColumn;
class DoubleColumn;
class StringColumn;
class IntColumn;

/**
 * Enum for the different types of SoR columns this code supports.
 */
enum class ColumnType { STRING, INTEGER, DOUBLE, BOOL, UNKNOWN };

/**
 * Converts the given ColumnType to a string.
 * @param type the column type
 * @return A string representing this column type. Do not free or modify this string.
 */
const char* columnTypeToString(ColumnType type) {
    switch (type) {
        case ColumnType::STRING:
            return "STRING";
        case ColumnType::INTEGER:
            return "INTEGER";
        case ColumnType::DOUBLE:
            return "DOUBLE";
        case ColumnType::BOOL:
            return "BOOL";
        default:
            return "UNKNOWN";
    }
}

class Column : public Object {
 public:
 
  /** Type converters: Return same column under its actual type, or
   *  nullptr if of the wrong type.  */
  virtual IntColumn* as_int() {
      return nullptr;
  }

  virtual BoolColumn*  as_bool(){
      return nullptr;
  }

  virtual DoubleColumn* as_double(){
      return nullptr;
  }

  virtual StringColumn* as_string(){
      return nullptr;
  }
 
  /** Type appropriate push_back methods. Calling the wrong method is
    * undefined behavior. **/
  virtual void push_back(int val) {
    return;
  }

  virtual void push_back(bool val) {
    return;
  }

  virtual void push_back(double val) {
    return;
  }

  virtual void push_back(String* val) {
    return;
  }
 
 /** Returns the number of elements in the column. */
  virtual size_t size() {
    return 0;
  }

  virtual void appendMissing() {
      return;
  }
 
  /** Return the type of this column as a char: 'S', 'B', 'I' and 'F'. */
  char get_type() {
    if (this->as_string()) {
      return 'S';
    }
    else if (this->as_double()) {
      return 'F';
    }
    else if (this->as_int()) {
      return 'I';
    }
    else if (this->as_bool()) {
      return 'B';
    }
    else {
      exit(1);
    }
  }

  virtual char* serialize() {
    assert(1 == 0);
  }

  static Column* deserialize(char* s);
};
 
/*************************************************************************
 * IntColumn::
 * Holds int values.
 */
class IntColumn : public Column {
 public:
  Array<int>* arr_;

  IntColumn() {
    arr_ = new Array<int>();
  }

  int get(size_t idx) {
    return arr_->get(idx);
  }

  IntColumn* as_int() {
    return this;
  }

  /** Set value at idx. An out of bound idx is undefined.  */
  void set(size_t idx, int val) {
    return arr_->set(val, idx);
  }

  virtual void push_back(int val) {
    arr_->add(val);
  }


  size_t size() {
    return arr_->getSize();
  }

  virtual char* serialize() {
    char* buff = new char[2048];
    sprintf(buff, "{INTCOLUMN|arr_=%s}", arr_->serialize()); 
    return buff;
  }

  static IntColumn* deserialize(char* s) {

  }

  ~IntColumn() {
    delete arr_;
  }
};
 
// Other primitive column classes similar...
 
/*************************************************************************
 * StringColumn::
 * Holds string pointers. The strings are external.  Nullptr is a valid
 * value.
 */
class StringColumn : public Column {
 public:
  Array<String*>* arr_;
  StringColumn() {
    arr_ = new Array<String*>();
  }

  StringColumn* as_string() {
    return this;
  }

  /** Returns the string at idx; undefined on invalid idx.*/
  String* get(size_t idx)  {
    return arr_->get(idx);
  }

  /** Out of bound idx is undefined. */
  void set(size_t idx, String* val) {
    return arr_->set(val, idx);
  }

  virtual void push_back(String* val) {
    arr_->add(val);
  }

  size_t size() {
    return arr_->getSize();
  }

  virtual char* serialize() {
    char* buff = new char[2048];
    sprintf(buff, "{STRINGCOLUMN|arr_=%s}", arr_->serialize()); 
    return buff;
  }

  static StringColumn* deserialize(char* s) {
    
  }

  ~StringColumn() {
    for (int x = 0; x < arr_->getSize(); x++) {
      delete arr_->get(x);
    }
    delete arr_;
  }
};

/*************************************************************************
 * BoolColumn::
 * Holds bool values.
 */
class BoolColumn : public Column {
 public:
  Array<bool>* arr_;
  BoolColumn() {
    arr_ = new Array<bool>();
  }

  BoolColumn* as_bool() {
    return this;
  }

  /** Returns the string at idx; undefined on invalid idx.*/
  bool get(size_t idx)  {
    return arr_->get(idx);
  }

  /** Out of bound idx is undefined. */
  void set(size_t idx, bool val) {
    return arr_->set(val, idx);
  }

  virtual void push_back(bool val) {
    arr_->add(val);
  }

  size_t size() {
    return arr_->getSize();
  }

  virtual char* serialize() {
    char* buff = new char[2048];
    sprintf(buff, "{BOOLCOLUMN|arr_=%s}", arr_->serialize()); 
    return buff;
  }

  static BoolColumn* deserialize(char* s) {
    int x = 17;
    BoolColumn* returning = new BoolColumn();
    assert(strcmp(returning->substring(s, 0, x), "{BOOLCOLUMN|arr_=") == 0);
    int y = returning->parseUntilClassSeperator(s, x);
    char* c = returning->substring(s, x, y);
    returning->arr_ = Array<bool>::deserialize_boolarray(c);
    return returning;
  }

  ~BoolColumn() {
    delete arr_;
  }
};

/*************************************************************************
 * DoubleColumn::
 * Holds double values.
 */
class DoubleColumn : public Column {
 public:
  Array<double>* arr_;
  DoubleColumn() {
    arr_ = new Array<double>();
  }

  DoubleColumn* as_double() {
    return this;
  }

  /** Returns the string at idx; undefined on invalid idx.*/
  bool get(size_t idx)  {
    return arr_->get(idx);
  }

  /** Out of bound idx is undefined. */
  void set(size_t idx, bool val) {
    return arr_->set(val, idx);
  }

  virtual void push_back(double val) {
    arr_->add(val);
  }

  size_t size() {
    return arr_->getSize();
  }

  virtual char* serialize() {
    char* buff = new char[2048];
    sprintf(buff, "{DOUBLECOLUMN|arr_=%s}", arr_->serialize()); 
    return buff;
  }

  static DoubleColumn* deserialize(char* s) {
    int x = 19;
    DoubleColumn* returning = new DoubleColumn();
    assert(strcmp(returning->substring(s, 0, x), "{DOUBLECOLUMN|arr_=") == 0);
    int y = returning->parseUntilClassSeperator(s, x);
    char* c = returning->substring(s, x, y);
    returning->arr_ = Array<bool>::deserialize_doublearray(c);
    return returning;
  }
    
  ~DoubleColumn() {
    delete arr_;
  }
};




/**
 * Creates the right subclass of Column based on the given type.
 * @param type The type of column to create
 * @return The newly created column. Caller must free.
 */
Column* makeColumnFromType(ColumnType type) {
    switch (type) {
        case ColumnType::STRING:
            return new StringColumn();
        case ColumnType::INTEGER:
            return new IntColumn();
        case ColumnType::DOUBLE:
            return new DoubleColumn();
        case ColumnType::BOOL:
            return new BoolColumn();
        default:
            assert(false);
    }
}

/**
 * Represents a fixed-size set of columns of potentially different types.
 */
class ColumnSet : public Object {
   public:
    /** The array of columns */
    Column** _columns;
    /** The number of columns we have */
    size_t _length;
    /**
     * Creates a new ColumnSet that can hold the given number of columns.
     * Caller must also call initializeColumn for each column to fully initialize this class.
     * @param num_columns The max number of columns that can be held
     */
    ColumnSet(size_t num_columns) : Object() {
        _columns = new Column*[num_columns];
        _length = num_columns;
        for (size_t i = 0; i < num_columns; i++) {
            _columns[i] = nullptr;
        }
    }
    /**
     * Destructor for ColumnSet
     */
    virtual ~ColumnSet() {
        delete[] _columns;
    }

    /**
     * Gets the number of columns that can be held in this ColumnSet.
     * @return The number of columns
     */
    virtual size_t getLength() { return _length; }

    /**
     * Initializes the given column to the given type. Can only be called exactly once per index.
     * @param which The index for the column to initialize
     * @param type The type of column to create
     */
    virtual void initializeColumn(size_t which, ColumnType type) {
        assert(which < _length);
        assert(_columns[which] == nullptr);
        Column* col = makeColumnFromType(type);
        _columns[which] = col;
    }

    /**
     * Gets the column with the given index. initializeColumn must have been called for this index
     * first.
     * @param which The column index to retrieve
     * @return The column with the given index
     */
    virtual Column* getColumn(size_t which) {
        assert(which < _length);
        assert(_columns[which] != nullptr);
        return _columns[which];
    }
};


template <class arrayClass>
Array<Column*>* Array<arrayClass>::deserialize_columnarray(char* s) {
  Sys sys;
  int y;
  Array<Column*>* returning = new Array<Column*>();
  const char* className = typeid(Column*).name();
  char* buff = new char[2048];
  sprintf(buff, "{Array|type=%s|array=", className);
  int x = strlen(buff);
  assert(strcmp(sys.substring(s, 0, x), buff) == 0);

  while(s[x] == '{') {
      y = sys.parseUntilClassSeperator(s, x);
      char* c = sys.substring(s, x, y);
      returning->add(Column::deserialize(c));
      x += y;
  }
  return returning;
}

 Column* Column::deserialize(char* s) {
  Sys sys;
  if (strcmp(sys.substring(s, 1, 9), "INTCOLUMN") == 0) {
    return IntColumn::deserialize(s);
  }
  else if (strcmp(sys.substring(s, 1, 12), "STRINGCOLUMN") == 0) {
    return StringColumn::deserialize(s);
  }
  else if (strcmp(sys.substring(s, 1, 10), "BOOLCOLUMN") == 0) {
    return BoolColumn::deserialize(s);
  }
  else if (strcmp(sys.substring(s, 1, 12), "DOUBLECOLUMN") == 0) {
    return DoubleColumn::deserialize(s);
  }
}