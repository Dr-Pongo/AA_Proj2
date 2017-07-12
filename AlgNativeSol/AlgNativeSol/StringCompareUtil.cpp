#include "StringCompareUtil.h"
#include <map>
#include <stdlib.h>

namespace StringCompareUtil
{
	std::map<char, KeyCoordinate> KeyboardMap =
	{
		//First row
		{ '1',{ 0, 0 } },
		{ '2',{ 0, 1 } },
		{ '3',{ 0, 2 } },
		{ '4',{ 0, 3 } },
		{ '5',{ 0, 4 } },
		{ '6',{ 0, 5 } },
		{ '7',{ 0, 6 } },
		{ '8',{ 0, 7 } },
		{ '9',{ 0, 8 } },
		{ '0',{ 0, 9 } },
		//Second row
		{ 'q',{ 1, 0 } },
		{ 'w',{ 1, 1 } },
		{ 'e',{ 1, 2 } },
		{ 'r',{ 1, 3 } },
		{ 't',{ 1, 4 } },
		{ 'y',{ 1, 5 } },
		{ 'u',{ 1, 6 } },
		{ 'i',{ 1, 7 } },
		{ 'o',{ 1, 8 } },
		{ 'p',{ 1, 9 } },
		//Third row
		{ 'a',{ 2, 0 } },
		{ 's',{ 2, 1 } },
		{ 'd',{ 2, 2 } },
		{ 'f',{ 2, 3 } },
		{ 'g',{ 2, 4 } },
		{ 'h',{ 2, 5 } },
		{ 'j',{ 2, 6 } },
		{ 'k',{ 2, 7 } },
		{ 'l',{ 2, 8 } },
		{ ';',{ 2, 9 } },
		//Fourth row
		{ 'z',{ 3, 0 } },
		{ 'x',{ 3, 1 } },
		{ 'c',{ 3, 2 } },
		{ 'v',{ 3, 3 } },
		{ 'b',{ 3, 4 } },
		{ 'n',{ 3, 5 } },
		{ 'm',{ 3, 6 } },
		{ ',',{ 3, 7 } },
		{ '.',{ 3, 8 } },
	};


	int KeyboardDistance(char firstChar, char secondChar)
	{
		int rowDiff = abs(GetKeyCoordinate(secondChar).row - GetKeyCoordinate(firstChar).row);
		int colDiff = abs(GetKeyCoordinate(secondChar).column - GetKeyCoordinate(firstChar).column);

		if (rowDiff < colDiff)
			return colDiff;

		return rowDiff;
	}

	KeyCoordinate GetKeyCoordinate(char key)
	{
		return KeyboardMap[key];
	}

	bool AreSameHand(char firstChar, char secondChar)
	{
		int firstKeyCol = GetKeyCoordinate(firstChar).column;
		int secondKeyCol = GetKeyCoordinate(secondChar).column;
		bool isFirstKeyLeftHand, isSecondKeyOnSameHand;

		isFirstKeyLeftHand = (0 <= firstKeyCol && firstKeyCol <= 4);

		if (isFirstKeyLeftHand)
		{
			//Check if secondChar is in the same cols for left hand 
			isSecondKeyOnSameHand = (0 <= secondKeyCol && secondKeyCol <= 4);
		}
		else {
			//FirstKey is on RH, check if secondChar is in the same cols for right hand 
			isSecondKeyOnSameHand = (5 <= secondKeyCol && secondKeyCol <= 9);
		}

		return isSecondKeyOnSameHand;
	}

	bool AreSameFinger(char firstChar, char secondChar)
	{
		int firstKeyCol = GetKeyCoordinate(firstChar).column;
		int secondKeyCol = GetKeyCoordinate(secondChar).column;

		int firstKeyFinger = (firstKeyCol >= 5) ? firstKeyCol - 5 : firstKeyCol;
		int secondKeyFinger = (secondKeyCol >= 5) ? secondKeyCol - 5 : secondKeyCol;

		return firstKeyFinger == secondKeyFinger;
	}

	int InsertionCost(char firstChar, char secondChar)
	{
		int subCost = 0;
		int temp = 0;

		//Repeated character - 1
		if (firstChar == secondChar)
		{
			subCost = 1;
			return subCost;
		}

		//Space after key on bottom row - 2
		if (GetKeyCoordinate(firstChar).column == 3 && secondChar == ' ')
		{
			subCost = 2;
			return subCost;
		}

		//Space after something else - 6
		//Character before a space - 6
		if (secondChar == ' ' || firstChar == ' ')
		{
			subCost = 6;
		}

		//Before or after another key on same hand - d(k1, k2)
		if (AreSameHand(firstChar, secondChar))
		{
			temp = KeyboardDistance(firstChar, secondChar);

			//if this costs less than a previouis cost
			if (subCost != 0 && temp < subCost)
			{
				subCost = temp;
			}
			//if there is no previous cost
			else if (subCost == 0)
			{
				subCost = temp;
			}
			//else cheaper function already exists,
			//keep subCost at the same value
		}

		//Before or after a key on opposite hand - 5		if (!AreSameHand(firstChar, secondChar))
		{
			temp = 5;

			//if this costs less than a previouis cost
			if (subCost != 0 && temp < subCost)
			{
				subCost = temp;
			}
			//if there is no previous cost
			else if (subCost == 0)
			{
				subCost = temp;
			}
			//else cheaper function already exists,
			//keep subCost at the same value
		}
		return subCost;
	}

	int DeletionnCost(char firstChar, char secondChar)
	{
		int subCost = 0;
		int temp = 0;

		//Deleting Repeated character - 1
		if (firstChar == secondChar)
		{
			subCost = 1;
			return subCost;
		}

		//Space - 3
		if (secondChar == ' ')
		{
			subCost = 3;
		}

		//Character after another key on same hand - 2
		if (AreSameHand(firstChar, secondChar)) 
		{
			subCost = 2;
			return 2;
		}

		//Character after space or key on different hand - 6
		if (firstChar == ' ' || !AreSameHand(firstChar, secondChar))
		{
			//if subCost is not 3
			if (subCost == 0)
			{
				subCost = 6;
			}
		}

		//First character in string - 6
		//************************************************
		//TODO: find out if character is first in string
		//************************************************
		
		return subCost;
	}

	int SubstitutionCost(char firstChar, char secondChar)
	{
		int subCost = 0;
		if (firstChar == ' ' || secondChar == ' ')
		{
			//TODO: Handle the cases for space
		}
		else {
			//Values from Cost table
			if (AreSameHand(firstChar, secondChar))
			{
				subCost = KeyboardDistance(firstChar, secondChar);
			}
			else {
				if (AreSameFinger(firstChar, secondChar))
				{
					subCost = 1;
				}
				else {
					subCost = 5;
				}
			}
		}
		return subCost;
	}

	int TransposingCost(char firstChar, char secondChar)
	{
		int subCost = 0;
		
		//Transposing Space with anything else - 3
		if (firstChar == ' ' || secondChar == ' ') 
		{
			subCost = 3;
		}

		//Keys on different hands - 1
		if (!AreSameHand(firstChar, secondChar))
		{
			subCost = 1;
			return subCost;
		}

		//Keys on the same hand - 2
		if (AreSameHand(firstChar, secondChar))
		{
			subCost = 2;
			return subCost;
		}

		return subCost;
	}
}

