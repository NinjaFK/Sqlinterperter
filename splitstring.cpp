
#include <iostream>
#include <string>
#include <sstream>

int main()
{
    std::string query;
    std::string partofQuery;

    std::cout << "Entera query: ";

    std::getline(std::cin, query);

    std::cout << "\n";

    std::stringstream ss(query);

    ss >> partofQuery;

    while (!ss.fail())
    {
        std::cout << partofQuery << "\n";
        ss >> partofQuery;
    }

    return 0;
}
