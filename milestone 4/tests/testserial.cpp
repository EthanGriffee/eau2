#include "../src/utilities/arraytemplate.h"
#include "../src/dataframe/dataframe.h"
#include "../src/utilities/string.h"
#include "../src/dataframe/column.h"
#include "../src/store/kvstore.h"
#include "../src/network/message.h"
#include <math.h>


void testStringArraySerialization() {
    Array<String*>* arr = new Array<String*>();
    arr->add(new String("jack"));
    arr->add(new String("jill"));
    arr->add(new String("hill"));
    char* serialized = arr->serialize();
    Array<String*>* new_array = Array<String*>::deserialize_stringarray(serialized);
    arr->t_true(new_array->equals(arr));
    arr->delete_contents(); 
    new_array->delete_contents();
    delete[] serialized;
    delete arr;
    delete new_array;
    arr->OK("String Array OK");
}

void testDoubleArraySerialization() {
    Array<double>* arr = new Array<double>();
    arr->add(241);
    arr->add(342.132343);
    arr->add(65.32);
    char* serialized = arr->serialize();
    Array<double>* new_array = Array<double>::deserialize_doublearray(serialized);
    arr->t_true(fabs(241 - new_array->get(0)) < .00001);
    arr->t_true(fabs(342.132343 - new_array->get(1)) < .00001);
    arr->t_true(fabs(65.32 - new_array->get(2)) < .00001);
    delete[] serialized;
    delete new_array;
    delete arr;
    arr->OK("DoubleArray OK");
}

void testCharArraySerialization() {
    Array<char>* arr = new Array<char>();
    arr->add('a');
    arr->add('b');
    arr->add('c');
    char* serialized = arr->serialize();
    Array<char>* new_array = Array<char>::deserialize_chararray(serialized);
    arr->t_true(new_array->equals(arr));
    delete[] serialized;
    delete new_array;
    delete arr;
    arr->OK("CharArray OK");
}

void testBoolArraySerialization() {
    Array<bool>* arr = new Array<bool>();
    arr->add(true);
    arr->add(true);
    arr->add(false);
    char* serialized = arr->serialize();
    Array<bool>* new_array = Array<bool>::deserialize_boolarray(serialized);
    arr->t_true(new_array->equals(arr));
    delete[] serialized;
    delete new_array;
    delete arr;
    arr->OK("BoolArray OK");
}

void testBoolColumnSerialization() {
    BoolColumn* b = new BoolColumn();
    b->push_back(true);
    b->push_back(false);
    b->push_back(true);
    char* serialized = b->serialize();
    BoolColumn* b2 = BoolColumn::deserialize(serialized);
    b->t_true(b->get(0) == b2->get(0));
    b->t_true(b->get(1) == b2->get(1));
    b->t_true(b->get(2) == b2->get(2));
    delete[] serialized;
    delete b;
    delete b2;
    b->OK("BoolColumn OK");
}

void testStringColumnSerialization() {
    StringColumn* b = new StringColumn();
    b->push_back(new String("bob"));
    b->push_back(new String("bob2"));
    b->push_back(new String("bob3"));
    char* serialized = b->serialize();
    StringColumn* b2 = StringColumn::deserialize(serialized);
    b->t_true(b->get(0)->equals(b2->get(0)));
    b->t_true(b->get(1)->equals(b2->get(1)));
    b->t_true(b->get(2)->equals(b2->get(2)));
    delete[] serialized;
    delete b;
    delete b2;
    b->OK("StringColumn OK");
}

void testIntColumnSerialization() {
    IntColumn* b = new IntColumn();
    b->push_back(2);
    b->push_back(5);
    b->push_back(1);
    char* serialized = b->serialize();
    IntColumn* b2 = IntColumn::deserialize(serialized);
    b->t_true(b->get(0) == b2->get(0));
    b->t_true(b->get(1) == b2->get(1));
    b->t_true(b->get(2) == b2->get(2));
    delete[] serialized;
    delete b;
    delete b2;
    b->OK("IntColumn OK");
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
    char* serialized = arr->serialize();
    Array<Column*>* new_array = Array<Column*>::deserialize_columnarray(serialized);
    b->OK("SerializingColumnArrays OK");
    arr->delete_contents();
    new_array->delete_contents();
    delete[] serialized;
    delete arr;
    delete new_array;
}

void testDataFrameSerialization() {
    Schema s("BD");
    DataFrame df(s);
    Row  r(df.get_schema());
    for(size_t i = 0; i <  10; i++) {
        r.set_bool(0,true);
        r.set_double(1,3.14);
        df.add_row(r);
    }
    char* serialized = df.serialize();
    DataFrame* newDataFrame = DataFrame::deserialize(serialized);
    s.t_true(newDataFrame->ncols() == df.ncols());
    s.t_true(newDataFrame->nrows() == df.nrows());
    s.t_true(newDataFrame->get_bool(0,0) == df.get_bool(0,0));
    delete[] serialized;
    delete newDataFrame;
    s.OK("Serializing Dataframes OK");
}

void testKeySerialization() {
    Key k("seven", 9);
    char* serialized = k.serialize();
    Key* newKey = Key::deserialize(serialized);
    k.t_true(k.getNode() == newKey->getNode());
    k.t_true(strcmp(newKey->getKey(), k.getKey()) == 0);
    delete [] serialized;
    delete newKey;
    k.OK("Serializing Keys OK");
}

void testMessageWaitAndGetSerialization() {
    Key k("seven", 9);
    WaitAndGet w(&k, 2, 3);
    char* serialized = w.serialize();
    WaitAndGet* newMess = WaitAndGet::deserialize(serialized);
    k.t_true(w.getKey()->equals(newMess->getKey()));
    k.t_true(w.getSender() == newMess->getSender());
    k.t_true(w.getTarget() == newMess->getTarget());
    delete [] serialized;
    delete newMess;
    k.OK("Serializing WaitAndGets Messages OK");
}

void testMessageKeyValueSerialization() {
    Key k("seven", 9);
    KeyValue w(&k, "justin", 2, 3);
    char* serialized = w.serialize();
    KeyValue* newMess = KeyValue::deserialize(serialized);
    k.t_true(w.getKey()->equals(newMess->getKey()));
    k.t_true(w.getSender() == newMess->getSender());
    k.t_true(w.getTarget() == newMess->getTarget());
    k.t_true(strcmp(w.getVal(), newMess->getVal()) == 0);
    delete [] serialized;
    delete newMess;
    k.OK("Serializing KeyValue Messages OK");
}

void testMessageRegisterSerialization() {
    sockaddr_in reg_addr;

    reg_addr.sin_family = AF_INET;
    reg_addr.sin_port = htons(8080);
    assert(inet_pton(AF_INET, "127.0.0.1", &reg_addr.sin_addr)>0);

    Register* m = new Register(reg_addr, 1, 1);
    Register* message = Register::deserialize(m->serialize());
    m->t_true(ntohs(message->getSockAddr().sin_port) == 8080);
    m->OK("Reg OK");
}

void testDirectorySerialization() {

    Array<String*>* arr = new Array<String*>();
    arr->add(new String("jack"));
    arr->add(new String("jill"));
    arr->add(new String("hill"));
    Array<int>* arr2 = new Array<int>();
    arr2->add(1);
    arr2->add(2);
    Directory* m = new Directory(arr, arr2, 5, 6);
    Directory* newMess = Directory::deserialize(m->serialize());
    arr->t_true(m->getSender() == newMess->getSender());
    arr->t_true(m->getTarget() == newMess->getTarget());
    arr->t_true(m->ports_->get(0) == 1);
    arr->t_true(strcmp(m->ips_->get(2)->c_str(), "hill") == 0);
    arr->OK("Dir OK");
}



int main(int argc, char **argv) {
    testStringArraySerialization();
    testDoubleArraySerialization();
    testCharArraySerialization();
    testBoolArraySerialization();
    testBoolColumnSerialization();
    testStringColumnSerialization();
    testIntColumnSerialization();
    testColumnArraySerialization();
    testDataFrameSerialization();
    testKeySerialization();
    testMessageWaitAndGetSerialization();
    testMessageKeyValueSerialization();
    testMessageRegisterSerialization();
    testDirectorySerialization();
}