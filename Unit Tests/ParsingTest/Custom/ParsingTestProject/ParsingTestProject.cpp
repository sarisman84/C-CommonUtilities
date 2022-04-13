

#include <iostream>
#include <..\..\..\C-CommonUtilities\Parser.h>

int main()
{
    Parser parser;

    auto result = parser.Evaluate("testName = 1;testName2 = 2;");


    //std::cout << std::any_cast<const char*>(result) << "\n";
    std::cin;
}


