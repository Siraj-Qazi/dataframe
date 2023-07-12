#include "DataFrame.h"


int main() {
    qz::DataFrame df;

    // read data from a CSV file
    df.read_csv("data.csv");

    // print DataFrame
    std::cout << df << std::endl;
    return 0;
}
