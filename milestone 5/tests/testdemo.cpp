#include "../src/applications/demo.h"
#include "../src/network/networkpseudo.h"

void test1() {
    NetworkPseudo net(3);
    Threaded_Demo * producer = new Threaded_Demo(0, net);
    Threaded_Demo * counter = new Threaded_Demo(1, net);
    Threaded_Demo * summarizer = new Threaded_Demo(2, net);
    producer->start();
    counter->start();
    summarizer->start();

    producer->join();
    counter->join();
    summarizer->join();
    net.OK("Demo works");
}



int main() {
    test1();
    return 0;
}