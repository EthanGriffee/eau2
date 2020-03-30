#include "../src/networkpseudo.h"


void testStringToUnsignedMap() {
    StringToUnsignedMap t;
    String* key1 = new String("one");
    String* key2 = new String("two");
    String* key3 = new String("three");
    t.put(key1, 1);
    t.put(key2, 2);
    t.put(key3, 3);

    t.t_true(t.get(key1) == 1);
    t.t_true(t.get(key2) == 2);
    t.t_true(t.get(key3) == 3);
    t.OK("TestStringToUnisdnedMap Works");
}

void testMessageQueue() {
    MessageQueue m;
    Key k("seven", 9);
    KeyValue* kv = new KeyValue(&k, "justin", 2, 3);
    KeyValue* kv2 = new KeyValue(&k, "josh", 1, 4);
    m.push(kv);
    m.push(kv2);
    m.t_true(m.pop()->getTarget() == 3);
    m.t_true(m.pop()->getTarget() == 4);

    delete kv;
    delete kv2;
    m.OK("testMessageQueue Works");
}

int main() {
    testStringToUnsignedMap();
    testMessageQueue();
}