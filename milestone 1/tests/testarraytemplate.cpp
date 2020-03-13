#include <stdlib.h>
#include "../src/arraytemplate.h"
#include "../src/string.h"
#include "../src/helper.h"
#include "../src/object.h"

class TestArrayMethods {
    public:
    Object* a;
    Object* b;
    Object* c;
    Sys* s;
    Object* d;
    String* s1;
    String* s2;
    String* s3;
    String* s4;
    Array<Object*>* arr;
    Array<Object*>* arr2;
    Array<Object*>* arr3;
    Array<Object*>* arr4;
    Array<String*>* starr;
    Array<String*>* starr2;
    Array<String*>* starr3;
    Array<String*>* starr4;
    Array<int>* iarr;
    Array<int>* iarr2;
    Array<int>* iarr3;
    Array<int>* iarr4;
    Array<double>* farr; 
    Array<double>* farr2; 
    Array<double>* farr3; 
    Array<double>* farr4; 
    Array<bool>* barr;
    Array<bool>* barr2;
    Array<bool>* barr3;
    Array<bool>* barr4;

    void initconditions() {
        a = new Object();
        b = new Object();
        c = new Object();
        d = new Object();
        s = new Sys;
        arr = new Array<Object*>();
        arr2 = new Array<Object*>();
        arr3 = new Array<Object*>();
        arr4 = new Array<Object*>();
        s1 = new String("Alpha");
        s2 = new String("Bravo");
        s3 = new String("Charlie");
        s4 = new String("Delta");
        starr = new Array<String*>();
        starr2 = new Array<String*>();
        starr3 = new Array<String*>();
        iarr = new Array<int>();
        farr = new Array<double>();
        barr = new Array<bool>();
        iarr2 = new Array<int>();
        farr2 = new Array<double>();
        barr2 = new Array<bool>();
        iarr3 = new Array<int>();
        farr3 = new Array<double>();
        barr3 = new Array<bool>();
        iarr4 = new Array<int>();
        farr4 = new Array<double>();
        barr4 = new Array<bool>();

    }

    void clear() {
        delete a;
        delete b;
        delete c;
        delete d;
        delete arr;
        delete arr2;
        delete arr3;
        delete arr4;
    }


    void testAdd() {
        initconditions();
        s->t_true(arr->getSize() == 0);
        arr->add(a);
        s->t_true(arr->get(0)->equals(a));
        s->t_true(arr->getSize() == 1);
        arr->add(b);
        s->t_true(arr->getSize() == 2);
        s->t_true(arr->get(1)->equals(b));
        s->t_true(arr->get(0)->equals(a));
        s->t_true(arr->indexOf(a) == 0);
        s->t_true(arr->indexOf(b) == 1);
        s->pln("All basic add test works");
        clear();
    }

    void testAddWithIndexing() {
        initconditions();
        s->t_true(arr->getSize() == 0);
        arr->add(a, 0);
        s->t_true(arr->get(0)->equals(a));
        s->t_true(arr->getSize() == 1);
        arr->add(b, 0);
        s->t_true(arr->getSize() == 2);
        s->t_true(arr->get(1)->equals(a));
        s->t_true(arr->get(0)->equals(b));
        s->pln("Add at beginning works");
        arr->add(c, 1);
        s->t_true(arr->get(1)->equals(c));
        s->t_true(arr->get(2)->equals(a));
        s->pln("Add at middle works");
        arr->add(d, 3);
        s->t_true(arr->get(2)->equals(a));
        s->t_true(arr->get(3)->equals(d));
        s->pln("Add at end works");
        clear();
    }

    void testAddAll() {
        initconditions();
        arr->add(a);
        arr->add(b);
        arr2->addAll(arr);
        s->t_true(arr2->getSize()==2);
        s->pln("Add All works at end");
        arr3->addAll(arr);
        arr3->addAll(arr,0);
        s->t_true(arr3->get(2)->equals(a));
        s->t_true(arr3->get(0)->equals(a));
        s->t_true(arr3->get(1)->equals(b));  
        s->t_true(arr3->getSize() == 4);    
        s->pln("Add All works at beginning");
        arr4->add(d);
        arr4->add(d);
        arr->addAll(arr4,1);
        s->t_true(arr->get(1)->equals(d));
        s->t_true(arr->get(2)->equals(d));
        s->t_true(arr->get(3)->equals(b));
        s->t_true(arr->getSize() == 4);
        s->pln("Add All works in middle");
    clear();

}

    void testRemoveAndRemoveAll() {
        initconditions();
        arr->add(a);
        arr->add(a);
        arr->add(b, 1);
        s->t_true(arr->getSize() == 3);
        arr->remove(a);
        s->t_true(arr->getSize() == 2);
        s->t_true(arr->get(0)->equals(b));
        s->pln("Singular remove works");
        arr->add(b);
        arr->add(b);
        s->t_true(arr->getSize() == 4);
        arr->removeAll(b);
        s->t_true(arr->getSize() == 1);
        s->t_true(arr->get(0)->equals(a));
        s->pln("Remove all works");
        clear();

    }

    void testBasicStringArray() {
        initconditions();
        starr->add(s1);
        starr->add(s2);
        String * s = (String*) starr->get(1);
        s->t_true(s->equals(s2));
        starr->add(s3,0);
        s = (String*) starr->get(0);
        s->t_true(s->equals(s3));
        s->t_true(starr->getSize() == 3);
        starr2->add(s3);
        starr2->add(s3);
        starr->addAll(starr2);
        s->t_true(starr->getSize() == 5);
        starr3->add(s3);
        starr3->add(s1);
        starr3->add(s2);
        starr3->add(s3);
        starr3->add(s3);
        s->t_true(starr3->getSize() == 5);
        starr3->removeAll(s3);
        s->t_true(starr3->getSize() == 2);
        s = (String*) starr3->get(0);
        s->t_true(s->equals(s1));
        starr3->remove(s1);
        s->t_true(starr3->getSize() == 1);
        s = (String*) starr3->get(0);
        s->t_true(s->equals(s2));
        s->pln("Basic String Array Functions work");
        delete s;
        clear();
    }
    
    void testBasicIntArray() {
        initconditions();
        iarr->add(1);
        iarr->add(2);
        s->t_true(iarr->get(0) == 1);
        iarr->add(0,0);
        s->t_true(iarr->get(0) == 0);
        s->t_true(iarr->getSize() == 3);
        iarr2->add(3);
        iarr2->add(4);
        iarr->addAll(iarr2);
        s->t_true(iarr->getSize() == 5);
        iarr3->add(0);
        iarr3->add(1);
        iarr3->add(2);
        iarr3->add(3);
        iarr3->add(4);
        s->t_true(iarr3->getSize() == 5);
        iarr3->removeAll(0);
        s->t_true(iarr3->getSize() == 4);
        s->t_true(iarr3->get(0) == 1);
        iarr3->remove(1);
        s->t_true(iarr3->getSize() == 3);
        s->t_true(iarr3->get(0) == 2);
        s->pln("Basic Int Array Functions work");
        clear();
    }

    void testBasicDoubleArray() {
        initconditions();
        double tolerance = .001;
        farr->add(1.1);
        farr->add(2.1);
        s->t_true(farr->get(0) >= 1.1 - tolerance && farr->get(0) <= 1.1 + tolerance);
        farr->add(0.1,0);
        s->t_true(farr->get(0) >= 0.1 - tolerance && farr->get(0) <= 0.1 + tolerance);
        s->t_true(farr->getSize() == 3);
        farr2->add(3.1);
        farr2->add(4.1);
        farr->addAll(farr2);
        s->t_true(farr->getSize() == 5);
        farr3->add(0.1);
        farr3->add(1.1);
        farr3->add(2.1);
        farr3->add(3.1);
        farr3->add(4.1);
        s->t_true(farr3->getSize() == 5);
        s->pln("Basic Double Array Functions work");
        clear();
    }
    void testBasicBoolArray() {
        initconditions();
        barr->add(true);
        barr->add(false);
        s->t_true(barr->get(0) == true);
        barr->add(false,0);
        s->t_true(barr->get(0) == false);
        s->t_true(barr->getSize() == 3);
        barr2->add(true);
        barr2->add(false);
        barr->addAll(barr2);
        s->t_true(barr->getSize() == 5);
        barr3->add(false);
        barr3->add(true);
        barr3->add(false);
        barr3->add(true);
        barr3->add(false);
        s->t_true(barr3->getSize() == 5);
        s->pln("Basic Bool Array Functions work");
        clear();
    }

};



int main() {
    TestArrayMethods *t = new TestArrayMethods();
    t->testAdd();
    t->testAddWithIndexing();
    t->testAddAll();
    t->testRemoveAndRemoveAll();
    t->testBasicStringArray();
    t->testBasicIntArray();
    t->testBasicDoubleArray();
    t->testBasicBoolArray();
    delete t;
}