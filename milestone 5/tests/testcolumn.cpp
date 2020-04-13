#include "../src/dataframe/dataframe.h" 

 
// tests column as int
void column_as_int_test() {
    StringColumn* c = new StringColumn();
    c->t_true(c->as_int() == nullptr);
    IntColumn* i = new IntColumn();
    c->t_true(i->as_int() == i);
    delete c;
    delete i;
}


// tests column as string 
void column_as_string_test() {
    StringColumn* c = new StringColumn();
    c->t_true(c->as_string() == c);
    IntColumn* i = new IntColumn();
    c->t_true(i->as_string() == nullptr);
    delete c;
    delete i;
}

// tests column as boolean 
void column_as_double_test() {
    BoolColumn* b = new BoolColumn();
    b->t_true(b->as_bool() == b);
    DoubleColumn* d = new DoubleColumn();
    b->t_true(d->as_bool() == nullptr);
    exit(0);
    delete b;
    delete d;
}

// column_type testing
void column_type_test() {
    StringColumn* c = new StringColumn();
    IntColumn* i = new IntColumn();
    BoolColumn* b = new BoolColumn();
    DoubleColumn* f = new DoubleColumn();
    Column* f2 = static_cast<Column*>(f);
    c->t_true(c->get_type() == 'S');
    c->t_true(i->get_type() == 'I');
    c->t_true(b->get_type() == 'B');
    c->t_true(f->get_type() == 'D');
    c->t_true(f2->get_type() == 'D');
    delete c;
    delete i;
    delete b;
    delete f;
}


// column_type testing
void column_methods_test() {
    BoolColumn* b = new BoolColumn();
    Column* b2 = static_cast<Column*>(b);
    b2->push_back(true);
    b2->push_back(false);
    b->t_true(b->get(0));
    b->t_false(b->get(1));
    b->set(0, false);
    b->t_true(b->get(0));
    b->t_true(b->size() == 2);
    delete b;
    delete b2;
}


int main() {
    column_as_int_test();
    column_as_string_test();
    column_as_double_test();
    column_type_test();
    column_type_test();
    column_methods_test();
    return 1;
}