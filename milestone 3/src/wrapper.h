#include "object.h"


// Wrapper for a Double
class DoubleObj : public Object {
    public:
        double f;

        // sets the value of f to input_f
        DoubleObj(double input_f) {
            f = input_f;
        }

        DoubleObj(DoubleObj& f) {
            f = f.getDouble();
        }

        // returns if other is a double that has the same f
        bool equals(Object *other) {
            double epsilon = .01;
            DoubleObj * f2 = dynamic_cast<DoubleObj*> (other);
            if (f2 && (f2->getDouble() - f < epsilon)  && (f2->getDouble() - f > -epsilon)){
                return true;
            }
            return false;
        }

        // returns f
        double getDouble() {
            return f;
        }
};

// Wrapper for a Boolean
class BoolObj : public Object {
    public:
        bool b;

        // sets the value of b to input_b
        BoolObj(bool input_b) {
            b = input_b;
        }

        BoolObj(BoolObj& b) {
            b = b.getBool();
        }

        // returns if other is a boolean that has the same b
        bool equals(Object *other){
            BoolObj * b2 = dynamic_cast<BoolObj*> (other);
            if (b2 && b2->getBool() == b ){
                return true;
            }
            return false;
        }

        // returns b
        bool getBool() {
            return b;
        }
};

// Wrapper for a Integer
class IntObj : public Object{
    public:
        int i;

        // sets the value of i to input_i
        IntObj(int input_i) {
            i = input_i;
        }

        // returns if other is a integer that has the same i
        bool equals(Object *other){
            IntObj * i2 = dynamic_cast<IntObj*> (other);
            if (i2 && i2->getInt() == i ){
                return true;
            }
            return false;
            
        }

        
        IntObj(IntObj& i) {
            i = i.getInt();
        }

        // returns i
        int getInt() {
            return i;
        }
};
