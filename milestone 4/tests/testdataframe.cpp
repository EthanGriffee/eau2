#include "../src/dataframe.h" 


void basic_test() {
  Schema s("II");
  DataFrame df(s);
  Row  r(df.get_schema());
  for(size_t i = 0; i <  1000 * 1000; i++) {
    r.set_int(0,(int)i);
    r.set_int(1,(int)i+1);
    df.add_row(r);
  }
  df.t_true(df.get_int((size_t)0,1) ==  1);
  df.OK("Basic Test works");
}

/*******************************************************************************
 *  Rower::
 *  A Rower that only displays rows where all bool values are true
 */
class OnlyTrueRower : public Rower {
 public:
  /** This method is called once per row. The row object is on loan and
      should not be retained as it is likely going to be reused in the next
      call. The return value is used in filters to indicate that a row
      should be kept. */
  virtual bool accept(Row& r) {
    for (int x = 0; x < r.width(); x++) {
      if (r.col_type(x) == 'B' && !r.get_bool(x, false)) {
        return  false;
      }
    }
    return true;
  }
 
  /** Once traversal of the data frame is complete the rowers that were
      split off will be joined.  There will be one join per split. The
      original object will be the last to be called join on. The join method
      is reponsible for cleaning up memory. */
  virtual void join_delete(Rower* other) {
    delete other;
  }
};


void copying_dataframe_test() {
  Schema s("II");
  DataFrame df(s);
  Row  r(df.get_schema());
  for(size_t i = 0; i <  1000; i++) {
    r.set_int(0,(int)i);
    r.set_int(1,(int)i+1);
    df.add_row(r);
  }

  DataFrame df2(df);
  df.t_true(df2.nrows() == 0);
  df.t_true(df2.ncols() == 2);
  df.OK("Copying Dataframe Test works");
}

void get_dataframe_test() {
  Schema s("IBS");
  DataFrame df(s);
  Row  r(df.get_schema());
  String* hello_string = new String("Hello");
  String* world_string = new String("World");
  String* welcome_string = new String("Welcome");

  r.set_int(0,1);
  r.set_bool(1, true);
  r.set_string(2, hello_string);
  df.add_row(r);
  r.set_int(0,2);
  r.set_bool(1, false);
  r.set_string(2, world_string);
  df.add_row(r);
  r.set_int(0,3);
  r.set_bool(1, false);
  r.set_string(2, welcome_string);
  df.add_row(r);


  df.t_true(df.nrows() == 3);
  df.t_true(df.ncols() == 3);
  df.t_true(df.get_bool(1,0));
  df.t_true(df.get_string(2,1)->equals(world_string));
  df.t_true(df.get_int(0,2) == 3);
  df.OK("Get Dataframe Test works");
}

void fill_row_dataframe_test() {
  String* hello_string = new String("Hello");
  String* world_string = new String("World");
  String* welcome_string = new String("Welcome");

  Schema s("IBS");
  DataFrame df(s);
  Row  r(df.get_schema());
  r.set_int(0,1);
  r.set_bool(1, true);
  r.set_string(2, hello_string);
  df.add_row(r);
  r.set_int(0,2);
  r.set_bool(1, false);
  r.set_string(2, world_string);
  df.add_row(r);
  r.set_int(0,3);
  r.set_bool(1, false);
  r.set_string(2, welcome_string);
  df.add_row(r);

  df.fill_row(1, r);
  df.t_true(r.get_int(0, true) == 2);
  df.t_true(r.get_bool(1, true) == false);
  df.t_true(r.get_string(2)->equals(world_string));
  df.t_true(r.width() == 3);
  df.OK("Fill Row Dataframe Test works");
}


void schema_test() {
  Schema s("IDBS");
  s.add_column('I');

  s.t_true(s.width() == 5);
  s.t_true(s.col_type(3) == 'S');
  s.t_true(s.col_type(2) == 'B');
  s.t_true(s.col_type(1) == 'D');
  s.t_true(s.col_type(0) == 'I');
  s.OK("Schema Test works");
}

void row_test() {
  Schema s("IDBS");
  Row r(s);

  String* str = new String("You did it buddy");

  r.set_int(0,12);
  r.set_double(1, double(12.01));
  r.set_bool(2, true);
  r.set_string(3, str);
  r.t_true(r.get_int(0, true) == 12);
  double x = r.get_double(1, true);
  r.t_true(x >= 12.01 - .001 && x <= 12.01 + .001);
  r.t_true(r.get_bool(2, true) == true);
  r.t_true(r.get_string(3)->equals(str));
  r.t_true(r.width() == 4);
  s.OK("Row Test works");
}

void rower_filter_dataframe_test() {
  Schema s("IBB");
  DataFrame df(s);
  Row  r(df.get_schema());
  r.set_int(0,1);
  r.set_bool(1, false);
  r.set_bool(2, true);
  df.add_row(r);
  r.set_int(0, 0);
  r.set_bool(1, true);
  r.set_bool(2, true);
  df.add_row(r);
  r.set_int(0, 2);
  r.set_bool(1, true);
  r.set_bool(2, false);
  df.add_row(r);

  OnlyTrueRower rower;
  DataFrame* df2 = df.filter(rower);

  df.t_true(df2->get_int(0, 0) == df.get_int(0, 1));
  df.t_true(df2->nrows() == 1);
  df.t_true(df2->get_bool(1, 0) == true);
  df.OK("Row Filter Dataframe Test works");
}

int main() {
    basic_test();
    row_test();
    schema_test();
    fill_row_dataframe_test();
    copying_dataframe_test();
    get_dataframe_test();
    rower_filter_dataframe_test();
    return 0;
}