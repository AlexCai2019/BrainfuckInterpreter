#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <exception>

#if _WIN64 || __x86_64__ || __ppc64__
typedef long long num;
const char* format = "%lld";
#else
typedef long num;
const char* format = "%ld";
#endif

std::vector<unsigned char> memory = { '\0','\0','\0','\0' };
num index = 0;
char exception[50];

void interpret(std::string&); //throws custom exception

int main(int argc, char* argv[])
{
	std::ios_base::sync_with_stdio(false);

	std::ifstream file;
	if (argc >= 2)
		file.open(argv[1]);
	else
	{
		std::cout << "Invalid argument.\nUsage:\n> BrainfuckInterpreter.exe <.bf file>" << std::endl;
		return 0;
	}

	if (!file.good())
	{
		std::cout << "No such file.\nUsage:\n> BrainfuckInterpreter.exe <.bf file>" << std::endl;
		return 0;
	}

	std::string read;
	std::string contents; //every legal characters in the file
	const char* symbols = "><+-.,[]"; //characters that a bf program legal
	while (!file.eof()) //read the whole file
	{
		std::getline(file, read);
		for (char c : read) //check every characters if legal
		{
			for (int i = 0; i < 8; i++)
			{
				if (c == symbols[i])
				{
					contents += c;
					break; //break out from symbols loop, back to the next character of read string
				}
			}
		}
	}
	file.close();

	try
	{
		interpret(contents);
	}
	catch (const std::exception& exception)
	{
		std::cout << exception.what() << std::endl;
	}

	return 0;
}

void interpret(std::string& contents)
{
	num length = contents.length();
	std::string subContents;

	for (num strIndex = 0; strIndex < length; strIndex++)
	{
		switch (contents[strIndex])
		{
		case '>':
			index++;
			if (index == memory.size())
				memory.push_back('\0');
			break;

		case '<':
			index--;
			break;

		case '+':
			memory[index]++;
			break;

		case '-':
			memory[index]--;
			break;

		case '.':
			std::cout << memory[index];
			break;

		case ',':
			std::cin >> memory[index];
			break;

		case '[':
		{
			num rightBracketIndex = -1; //the index of ]
			num bracketsCount = 0; //calculate how many brackets meets in order to offset
			char onTheWayFind;
			for (num findRightBracket = strIndex + 1; findRightBracket < length; findRightBracket++)
			{
				onTheWayFind = contents[findRightBracket];

				if (onTheWayFind == '[')
					bracketsCount++;
				else if (onTheWayFind == ']')
				{
					if (!bracketsCount)
					{
						rightBracketIndex = findRightBracket;
						break;
					}
					else
						bracketsCount--;
				}

				subContents += onTheWayFind;
			}

			if (bracketsCount || rightBracketIndex == -1)
			{
				char exceptionDest[] = "Missing \']\' of \'[\' at character ";
				sprintf(exception, strcat(exceptionDest, format), strIndex);
				throw std::exception(exception);
			}

			while (memory[index])
				interpret(subContents);

			subContents.clear();
			strIndex = rightBracketIndex;
		}
		}

		if (index < 0)
		{
			char exceptionDest[] = "Array out of bounds at character ";
			sprintf(exception, strcat(exceptionDest, format), strIndex);
			throw std::exception(exception);
		}
	}
}