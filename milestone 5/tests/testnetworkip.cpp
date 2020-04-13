#include "../src/network/networkip.h"
#include "../src/applications/demo.h"

int main(int argh, char** argv) {
    char* server_ip;
    size_t num_nodes = 0;
    size_t node_number = 0;
    int input_port, server_port;

    // Starts at 1 as the first argument in argv is a.out
    if (argh == 11) {
        for (size_t x = 1; x < argh; x += 2) {
            // handles -f flag, any input after the f flag will be considered valid
            if (strcmp(argv[x], "-num-nodes") == 0) {
                int p = 0;
                while (argv[x + 1][p] != '\0') {
                    if (!isdigit(argv[x + 1][p])) {
                        perror("Expected positive int value following -i flag");
                        exit(1);
                    }
                    num_nodes = num_nodes * 10;
                    num_nodes += argv[x + 1][p] - '0';
                    p += 1;
                }
            }

            else if (strcmp(argv[x], "-node-number") == 0) {
                int p = 0;
                while (argv[x + 1][p] != '\0') {
                    if (!isdigit(argv[x + 1][p])) {
                        perror("Expected positive int value following -i flag");
                        exit(1);
                    }
                    node_number = node_number * 10;
                    node_number += argv[x + 1][p] - '0';
                    p += 1;
                }
            }
            else if (strcmp(argv[x], "-port") == 0) {
                input_port = atoi(argv[x + 1]);
            }
            else if (strcmp(argv[x], "-serverip") == 0) {
                server_ip = argv[x + 1];
            }
            else if (strcmp(argv[x], "-serverport") == 0) {
                server_port = atoi(argv[x + 1]);
            }
            else {
                perror("Invalid argument");
                exit(1);
            }
            // handles everything else
        }
    }
    else {
        perror("Invalid number of arguments");
        exit(1);
    }
    printf("PPENER");
    printf("%zu\n", node_number);
    NetworkIP net(num_nodes, input_port, server_ip, server_port);
    Demo * d = new Demo(node_number, net);
    d->run_();
}