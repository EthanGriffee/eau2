#include "../src/applications/wordcountdemo.h"
#include "../src/network/networkpseudo.h"


class Threaded_Word_Demo : public Thread {
  public:
    size_t idx;
    NetworkIfc* net;

    Threaded_Word_Demo(size_t idx, NetworkIfc& net) {
      this->idx = idx;
      this->net = &net;
    }

    virtual void run() {
      WordCount * d = new WordCount(idx, *net, "./data/small_datafile.txt", 3);
      d->run_();
    }

};


void test1() {
    NetworkPseudo net(3);
    Threaded_Word_Demo * one = new Threaded_Word_Demo(0, net);
    Threaded_Word_Demo * two = new Threaded_Word_Demo(1, net);
    Threaded_Word_Demo * three = new Threaded_Word_Demo(2, net);
    one->start();
    two->start();
    three->start();

    one->join();
    two->join();
    three->join();
    net.OK("Demo works");
}



int main() {
    test1();
    return 0;
}