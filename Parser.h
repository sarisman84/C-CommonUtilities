#pragma once
#include <map>
#include <functional>
#include <any>
#include <vector>
#include <any>
#include <string>



class Parser
{
public:
	enum class Token
	{
		Identifier,
		NewLine,
		BeginExp,
		EndExp,
		Operator,
		Literal,
		Exit
	};

	Parser() = default;


private:
	struct TokenResult
	{
		size_t myTokenPos;
		size_t myStartingPos;
		Token myType;
	};
public:



	std::any Evaluate(const char* someInput)
	{
		std::vector<size_t> availableLines;
		size_t pos = 0;
		size_t inputLength = std::strlen(someInput);
		while (true)
		{
			size_t size = pos + 1;
			if (size >= inputLength)
				break;

			auto result = GetToken(Token::NewLine, someInput, pos + availableLines.size());
			pos = result.myTokenPos;


			availableLines.push_back(result.myStartingPos);
		}

		for (size_t i = 0; i < availableLines.size(); i++)
		{
			auto line = availableLines[i] - (i % 2 == 0 ? 1 : 0);
			if (ReturnStatementFound(line))
			{
				return EvaluateReturn(line);
			}


			//Get Variable Name
			//-Save name on myData
			//Get Expression, Contains: Term, <none or many>((+ or -) Term)
				//Terms Contains: Factor,<none or many>(* Factor)
					//Factor Contains: <none or one>(-), (Number, Name, ( '(' Expression ')' ) )
				//Name Contains: Characters
				//Number Contains: <one or many>Digit
				//Digit Contains: Numbers
			//Return Contains: 'return', Expression


			std::string varName = GetVariableName(line, someInput);

			std::cout << varName;

			std::any varValue = EvaluateExpression(line, someInput);
			if (varValue.has_value())
				std::cout << "= " << std::any_cast<int>(varValue) << ";" << std::endl;
			else
				std::cout << std::endl;
			myData[varName.c_str()] = varValue;

		}


	}





private:
	char GetTokenID(Token aToken)
	{
		return (char)aToken;
	}


	bool ReturnStatementFound(size_t someTokenInfo)
	{
		return false;
	}


	std::any EvaluateReturn(size_t someTokenInfo)
	{
		return std::any();
	}

	std::string GetVariableName(size_t aPos, const char* anInput)
	{

		auto equalsKeyword = GetToken(Token::Operator, anInput, aPos);

		std::string name = "";


		for (size_t i = equalsKeyword.myStartingPos;;)
		{


			if (CheckChar(&anInput[i]) == Token::Operator && anInput[i] != 32 && anInput[i] != ' ')
			{
				break;
			}
			name += anInput[i];


			++i;
		}

		return name;
	}



	std::vector<char> GetVariable(size_t aPos, const char* anInput)
	{
		std::vector<char> output;
		
		auto equalsOp = GetToken(Token::Operator, anInput, aPos);
		TokenResult result = GetToken(Token::Literal, anInput, equalsOp.myTokenPos);
		//make sure that the number is after the = operator
		bool isValidType = true;
		for (size_t i = aPos; (result = GetToken(Token::Literal, anInput, ++i)).myType == Token::Literal;)
		{
			isValidType = CheckChar(&anInput[result.myTokenPos]) == Token::Literal;
			if (isValidType)
				output.push_back(anInput[result.myTokenPos]);
		}

		return output; //Get Literal Token info
	}



	std::any EvaluateExpression(size_t aPos, const char* anInput)
	{
		auto o = GetVariable(aPos, anInput);
		char val[20] = "\0";


		for (size_t i = 0; i < o.size(); i++)
		{
			strcat_s(val, 20, &o[i]);
		}
		char r;
		if (o.size() > 1)
			r = *val - '0' + ((*(val + 1) - '0') << 4);
		else
			r = *val - '0';
		return  std::any((int)r);
	}





	TokenResult GetToken(Token aToken, const char* someContext, size_t aStartPos = 0)
	{
		TokenResult result;

		result.myStartingPos = aStartPos;

		bool exit = false;

		size_t i = aStartPos;

		//Set start point here
		someContext += aStartPos;

		while (true)
		{

			if (CheckChar(someContext) == aToken)
			{

				result.myTokenPos = i;
				result.myType = aToken;
				return result;
			}

			++someContext;
			++i;
		}

		return result;

	}

	Token CheckChar(const char* anInput)
	{
		if (std::isdigit(*anInput))
		{
			return Token::Literal;
		}

		if (std::isalpha(*anInput))
		{
			return  Token::Identifier;
		}

		if (*anInput == '+' || *anInput == '-' || *anInput == '=')
		{
			return Token::Operator;
		}
		if (*anInput == ';')
		{
			return Token::NewLine;
		}

		if (*anInput == '(')
		{
			return Token::BeginExp;
		}

		if (*anInput == ')')
		{
			return Token::EndExp;
		}


		if (anInput == "return")
		{
			return Token::Exit;
		}
	}








private:
	std::map<const char*, std::any> myData;


};



int EvaluateProgram(const char* anInputText)
{

	Parser parser;




	return std::any_cast<int>(parser.Evaluate(anInputText));
};





