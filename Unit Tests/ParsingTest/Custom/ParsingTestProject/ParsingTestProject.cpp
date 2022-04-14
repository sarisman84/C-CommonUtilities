

#include <iostream>
#include <..\..\..\C-CommonUtilities\Parser.h>

int main()
{
    Parser parser;

    auto result = parser.Evaluate("testName = 1;testName = 2;testName = 10;testName = 10;");


    //std::cout << std::any_cast<const char*>(result) << "\n";
    std::cin;
}


