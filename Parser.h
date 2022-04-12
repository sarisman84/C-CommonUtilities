#pragma once
#include <map>
#include <vector>
#include <functional>
#include <any>




class Parser
{
public:
	enum class Token : char
	{
		Identifier,
		Keyword = ' = ',
		NewLine = ';\n',
		Separator = '(' | ')',
		Operator = '+' | '-',
		Literal,
		Comment = '//',
		Exit
	};




	Parser() = default;

	void Init()
	{
		myKeywords.push_back("return");
	}


	std::any Evaluate(const char* someInput)
	{
		size_t curLine = 0;
		char* token = (char*)Token::NewLine;
		auto result = std::strstr(someInput, token);


		Context foundLine = GetContext(curLine, result, someInput);
		while (foundLine.myLineSize != 0)
		{
			curLine = foundLine.myLineSize;


			size_t 
			auto exp = GetContext();
			if ()
			{

			}



			foundLine = GetContext(curLine, result, someInput);
		}

	}

private:
	struct  Context
	{
		size_t myLineSize;
		size_t myStartingIndex;
	};

	Context GetContext(const size_t aCurLine, const char* aTarget, const char* someContext)
	{
		size_t size = 0;
		const char* lineContext = {};
		for (size_t i = aCurLine; i < std::strlen(someContext); i += std::strlen(aTarget))
		{
			const char* cmpChars = {};

			for (size_t x = 0; x < std::strlen(aTarget); ++x)
			{
				cmpChars += someContext[i + x];
			}

			lineContext += someContext[i];
			if (aTarget == cmpChars)
			{
				size = i + 1;
				break;
			}
		}


		return Context{ size, aCurLine };
	}

	std::vector<const char*> myKeywords;

};



int EvaluateProgram(const char* anInputText)
{

	Parser parser;

	return std::any_cast<int>(parser.Evaluate(anInputText));
};





