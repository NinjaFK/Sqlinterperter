
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

int main()
{
    std::string line;
    std::string partOfLine;
    const int col = 27;
    std::ifstream infile("UCS-Satellite-Database-5-1-2022.txt");

    std::getline(infile, line);

    std::stringstream ss(line);
    // reads a substring in a line until tab character reached

    std::getline(ss, partOfLine, '\t');
    // while we are not at the end of the string stream

    for (int i = 0; i < col; i++)
    {
        std::getline(ss, partOfLine, '\t');
        std::cout << partOfLine << '\n';
    }
    return 0;
}