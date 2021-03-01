#include <iostream>
#include <map>
#include <string>
#include <array>
#include <Windows.h>

//forward function declaratins
bool isFieldAvailable(int fieldNumber);
void displayFields();

std::map<int, char> fields =
{
	{1, '1'},
	{2, '2'},
	{3, '3'},
	{4, '4'},
	{5, '5'},
	{6, '6'},
	{7, '7'},
	{8, '8'},
	{9, '9'}
};

static const std::array<std::array<const int, 3>, 8> winningFields =
{
{
		{1,2,3},
		{4,5,6},
		{7,8,9},

		{1,4,7},
		{2,5,8},
		{3,6,9},

		{1,5,9},
		{3,5,7}
}
};

class Player
{
private:
	std::string m_Name;
	char m_Symbol;
public:
	//returns player name
	std::string GetName() const { return m_Name; };
	//returns player symbol
	char GetSymbol() const { return m_Symbol; };

	Player(std::string name, char symbol) : m_Name(name), m_Symbol(symbol) {};

	void Play()
	{
		printf("Your turn %s.\n", m_Name.c_str());

		bool keyPressed = false;
		int keyCode = 0;

		printf("Please enter numpad key of field.\n");
		//we will loop until user presses numpad key of field
		while (!keyPressed)
		{
			for (keyCode = 1; keyCode <= 9; ++keyCode)
			{
				//if user pressed key we will store index of that key in keyCode
				if (GetAsyncKeyState(keyCode + VK_NUMPAD0) & 1)
				{
					//if field is not available we will continue with our loop
					if (!isFieldAvailable(keyCode))
						continue;

					//find the symbol of field[keyCode] and assign it the value of player symbol
					fields.find(keyCode)->second = m_Symbol;

					//exit the loop
					keyPressed = true;
					break;
				}
			}
			//sleep for 20 milliseconds just to lower CPU usage
			Sleep(20);
		}
	}
};

bool isFieldAvailable(int fieldNumber)
{
	//get iterator in hash map
	auto fieldsIterator = fields.find(fieldNumber);

	//if it fails to find iterator with given key it will return fields.end() iterator and we return false
	if (fieldsIterator == fields.end())
		return false;

	//if value of iterator is 'x' or 'y' it means field is taken and we return false
	if ((fieldsIterator->second == 'x') || (fieldsIterator->second == 'o'))
		return false;

	return true;
}

void displayFields()
{
	//clean the console
	system("cls");

	//loop through fields hash map
	size_t mapSize = fields.size();
	for (size_t counter = 1; counter <= mapSize; ++counter)
	{
		//print new line every 4th index to make 3x3 grid
		if ((counter > 1) && ((counter % 3) == 1))
			printf("\n");

		//if field is not available we will print symbol that is on that field
		if (!isFieldAvailable(counter))
		{
			printf("%c |", fields.find(counter)->second);
			continue;
		}

		//if field is available we print a number
		printf("%i |", counter);
	}
	printf("\n");
}

bool isWinner(char marker)
{
	bool won = true;

	//loop through winningFields array [8][3]
	for (auto& counter : winningFields)
	{
		//reassign won to true every time we start looping through another subarray
		won = true;

		for (auto& fieldPosition : counter)
		{
			//get the current symbol that is on the position of winning positions[counter][n]
			char currentCharacter = fields.find(fieldPosition)->second;

			//if current symbol is not the same as symbol we are looking for we break and scan through another subarray
			if (marker != currentCharacter)
			{
				won = false;
				break;
			}
		}
		//if we find the same symbol in all winning fields won not be changed and we can exit the loop
		if (won)
			break;
	}
	return won;
}

bool areAllFieldsSet()
{
	int fieldsSize = fields.size();

	for (int field = 1; field <= fieldsSize; ++field)
	{
		auto fieldsIterator = fields.find(field);

		if ((fieldsIterator->second != 'x') && (fieldsIterator->second != 'o'))
			return false;
	}

	return true;
}

bool isGameOver(char playerSymbol, char* outputString)
{
	if (isWinner(playerSymbol))
	{
		strcpy_s(outputString, 13, "Someone win.");
		return true;
	}

	if (areAllFieldsSet())
	{
		strcpy_s(outputString, 12, "It's a tie!");
		return true;
	}

	return false;
}

int main()
{
	printf("Game Started\n");
	displayFields();

	Player playerOne("Player One", 'x');
	Player playerTwo("Player Two", 'o');

	char endGameMessage[255] = "";

	while (true)
	{
		playerOne.Play();
		displayFields();
		if (isGameOver(playerOne.GetSymbol(), endGameMessage))
		{
			break;
		}
		playerTwo.Play();
		displayFields();
		if (isGameOver(playerTwo.GetSymbol(), endGameMessage))
		{
			break;
		}
		Sleep(20);
	}
	printf("%s\n", endGameMessage);
	printf("Game Ended\n");
	system("PAUSE");
}