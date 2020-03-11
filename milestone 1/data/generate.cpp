#include <stdio.h>


int main(int argc, char** argv) {
    FILE* file = fopen("datafile.txt", "w");
    for (int i = 0; i < 866666; i++) {
        fputs("<+1><0><Jimmy><12><1><josh><1.1><0><323><.323><josh>\n", file);
        fputs("<-1><0><Ham><2><0><Bone><2.2><1><32><.222><chicken>\n", file);
        fputs("<437><1><Bone><2><0><Ham><3.33><1><54><.666><soup>\n", file);
        fputs("<-32><0><Ham><2><0><Bone><2.1><0><32><.222><chicken>\n", file);
    }
    return 0;
}