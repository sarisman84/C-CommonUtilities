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
	enum class Token : char
	{
		Identifier,
		Keyword = '=',
		NewLine = ';' | '\n',
		Separator = '(' | ')',
		Operator = '+' | '-',
		Literal = '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9' | '0',
		Comment = '//',
		Exit
	};



private:
	struct DataInfo
	{
		std::any myValue;
	};

	struct  TokenInfo
	{
		size_t myLineSize;
		size_t myStartingIndex;
		size_t myIndex = -1;
		bool myIsValid;
	};


public:
	Parser() = default;




	std::any Evaluate(const char* someInput)
	{
		std::vector<TokenInfo> availableLines;
		TokenInfo info = { 0,0,0, true };
		for (; info.myIsValid;)
		{

			info = GetToken(info.myLineSize, GetTokenID(Token::NewLine), someInput);
			if (info.myIsValid)
				availableLines.push_back(info);
		}

		for (size_t i = 0; i < availableLines.size(); i++)
		{

			if (ReturnStatementFound(availableLines[i]))
			{
				return EvaluateReturn(availableLines[i]);
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


			std::string varName = GetVariableName(availableLines[i], someInput);

			std::cout << varName;

			DataInfo varValue = EvaluateExpression(availableLines[i], someInput);
			std::cout << "= " << std::any_cast<int>(varValue.myValue) << ";" << std::endl;

			myData[varName.c_str()] = varValue.myValue;

		}


	}





private:
	inline static char ourTokenID[2];
	char* GetTokenID(Token aToken)
	{
		ourTokenID[0] = (char)aToken;
		ourTokenID[1] = '\0';
		return ourTokenID;
	}


	bool ReturnStatementFound(TokenInfo someTokenInfo)
	{
		return false;
	}


	std::any EvaluateReturn(TokenInfo someTokenInfo)
	{
		return std::any();
	}

	std::string GetVariableName(TokenInfo someTokenInfo, const char* anInput)
	{

		TokenInfo equalsKeyword = GetToken(someTokenInfo.myStartingIndex, GetTokenID(Token::Keyword), anInput);

		std::string name = "";

		for (size_t i = equalsKeyword.myStartingIndex; i < equalsKeyword.myIndex; i++)
		{
			name += anInput[i];
		}

		return name;
	}



	size_t GetVariable(TokenInfo someTokenInfo, const char* anInput)
	{
		TokenInfo lit = GetToken(someTokenInfo.myStartingIndex, GetTokenID(Token::Literal), anInput); //Get Literal Token info
		return lit.myIndex;
	}



	DataInfo EvaluateExpression(TokenInfo someTokenInfo, const char* anInput)
	{


		return DataInfo{ std::any(anInput[GetVariable(someTokenInfo, anInput)] - '0') };
	}





	TokenInfo GetToken(const size_t aCurLine, const char* aTarget, const char* someContext, std::function<bool(const char* someInput)> anCondition = nullptr)
	{
		bool isValid = false;
		size_t size = 0;
		size_t endPoint = 0;
		std::vector<char> charResult = std::vector<char>();
		charResult.reserve(std::strlen(aTarget) + 1);
		charResult.resize(std::strlen(aTarget) + 1);

		size_t length = std::strlen(aTarget);
		size_t contextLength = std::strlen(someContext);
		for (size_t i = aCurLine; i < contextLength; i += length)
		{

			charResult.clear();


			if (length <= contextLength)
			{
				for (size_t x = 0; x < length; ++x)
				{
					charResult.push_back(someContext[i + x]);

				}
				//charResult.push_back('\0');
			}

			const char* result = charResult.data();
			bool extraCondition = anCondition ? anCondition(result) : true;
			bool haveFoundTarget = *aTarget == *result;
			if (haveFoundTarget && extraCondition)
			{
				size = i + 1;
				endPoint = i;
				isValid = true;
				break;
			}
		}


		return TokenInfo{ size, aCurLine, endPoint, isValid };
	}
	//Map order>> Map(Type, SubMap(Name, Value)) myData
	std::map<const char*, std::any> myData;
};



int EvaluateProgram(const char* anInputText)
{

	Parser parser;

	return std::any_cast<int>(parser.Evaluate(anInputText));
};





