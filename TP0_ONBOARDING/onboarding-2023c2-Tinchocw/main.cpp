#include <fstream>
#include <iostream>
#include <string>

#include "wordscounter.h"

#define SUCCESS 0
#define ERROR 1


int main(int argc, char* argv[]) {
    Wordscounter* counter = new Wordscounter;
    std::ifstream* input = new std::ifstream;

    if (argc > 1) {
        input->open(argv[1]);
        counter->process(*input);
        if (!input->is_open()){

            delete counter;
            delete input;
            return ERROR;
        }

    } else {
        counter->process(std::cin);
    }


    size_t words = counter->get_words();
    std::cout << words << std::endl;

    delete counter;
    delete input;
    return SUCCESS;
}
