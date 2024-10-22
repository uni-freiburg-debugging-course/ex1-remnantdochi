#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

// define the range of random numbers
constexpr int MIN_NUM = 0;
constexpr int MAX_NUM = 999;
constexpr int RAND_CNT = 10;

char operators[] = {'+', '-', '*'};

void generate_random_input(std::string &input)
{
    int num1 = (rand() % (MAX_NUM - MIN_NUM + 1)) + MIN_NUM;
    int num2 = (rand() % (MAX_NUM - MIN_NUM + 1)) + MIN_NUM;
    char operator_ = operators[rand() % sizeof(operators)];

    input = "(simplify (" + std::string(1, operator_) + " " + std::to_string(num1) + " " + std::to_string(num2) + "))";
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "USE: " << argv[0] << " <output_file.smt2>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];

    std::ofstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "ERR: Unable to open file" << std::endl;
        return 1;
    }

    srand(static_cast<unsigned int>(time(nullptr)));

    std::string input;

    for (int i = 0; i < RAND_CNT; i++)
    {
        generate_random_input(input);
        file << input << std::endl;
    }

    file.close();
    std::cout << "RES: Random SMT-LISP expressions saved to " << filename << std::endl;

    return 0;
}