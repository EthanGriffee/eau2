#include "../src/arraytemplate.h"
#include "../src/dataframe.h"
#include "../src/string.h"
#include "../src/column.h"
#include <math.h>


void testStringArraySerialization() {
    Array<String*>* arr = new Array<String*>();
    arr->add(new String("jack"));
    arr->add(new String("jill"));
    arr->add(new String("hill"));
    Array<String*>* new_array = Array<String*>::deserialize_stringarray(arr->serialize());
    arr->t_true(new_array->equals(arr));
    arr->delete_contents(); 
    new_array->delete_contents();
    delete arr;
    delete new_array;
    arr->OK("String Array OK");
}

void testDoubleArraySerialization() {
    Array<double>* arr = new Array<double>();
    arr->add(241);
    arr->add(342.132343);
    arr->add(65.32);
    Array<double>* new_array = Array<double>::deserialize_doublearray(arr->serialize());
    arr->t_true(fabs(241 - new_array->get(0)) < .00001);
    arr->t_true(fabs(342.132343 - new_array->get(1)) < .00001);
    arr->t_true(fabs(65.32 - new_array->get(2)) < .00001);
    delete new_array;
    delete arr;
    arr->OK("DoubleArray OK");
}

void testCharArraySerialization() {
    Array<char>* arr = new Array<char>();
    arr->add('a');
    arr->add('b');
    arr->add('c');
    Array<char>* new_array = Array<char>::deserialize_chararray(arr->serialize());
    arr->t_true(new_array->equals(arr));
    delete new_array;
    delete arr;
    arr->OK("CharArray OK");
}

void testBoolArraySerialization() {
    Array<bool>* arr = new Array<bool>();
    arr->add(true);
    arr->add(true);
    arr->add(false);
    Array<bool>* new_array = Array<bool>::deserialize_boolarray(arr->serialize());
    arr->t_true(new_array->equals(arr));
    delete new_array;
    delete arr;
    arr->OK("BoolArray OK");
}

void testBoolColumnSerialization() {
    BoolColumn* b = new BoolColumn();
    b->push_back(true);
    b->push_back(false);
    b->push_back(true);
    BoolColumn* b2 = BoolColumn::deserialize(b->serialize());
    b->t_true(b->get(0) == b2->get(0));
    b->t_true(b->get(1) == b2->get(1));
    b->t_true(b->get(2) == b2->get(2));
    delete b;
    delete b2;
    b->OK("BoolColumn OK");
}

void testColumnArraySerialization() {
    Array<Column*>* arr = new Array<Column*>();
    BoolColumn* b = new BoolColumn();
    b->push_back(true);
    b->push_back(false);
    arr->add(b);
    DoubleColumn* d = new DoubleColumn();
    d->push_back(3.14);
    d->push_back(12.12);
    arr->add(d);
    Array<Column*>* new_array = Array<Column*>::deserialize_columnarray(arr->serialize());
    b->OK("SerializingColumnArraus OK");
    arr->delete_contents();
    new_array->delete_contents();
    delete arr;
    delete new_array;
}

void testDataFrameSerialization() {
    Schema s("BF");
    DataFrame df(s);
    Row  r(df.get_schema());
    for(size_t i = 0; i <  10; i++) {
        r.set_bool(0,true);
        r.set_double(1,3.14);
        df.add_row(r);
    }
    DataFrame* newDataFrame = DataFrame::deserialize(df.serialize());
    s.t_true(newDataFrame->ncols() == df.ncols());
    s.t_true(newDataFrame->nrows() == df.nrows());
    s.t_true(newDataFrame->get_bool(0,0) == df.get_bool(0,0));
    s.OK("Serializing Dataframes OK");
}


int main(int argc, char **argv) {
    testStringArraySerialization();
    testDoubleArraySerialization();
    testCharArraySerialization();
    testBoolArraySerialization();
    testBoolColumnSerialization();
    testColumnArraySerialization();
    testDataFrameSerialization();
}