#include "StringCompareUtil.h"
#include <map>
#include <stdlib.h>

#define MATCH 0 /* enumerated type symbol for match */ 
#define INSERT 1 /* enumerated type symbol for insert */ 
#define DELETE 2 /* enumerated type symbol for delete */
#define TRANSP 3
#define INFIN 2000000000



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


	int KeyboardDistance(char targetChar, char typoChar)
	{
		int rowDiff = abs(GetKeyCoordinate(typoChar).row - GetKeyCoordinate(targetChar).row);
		int colDiff = abs(GetKeyCoordinate(typoChar).column - GetKeyCoordinate(targetChar).column);

		if (rowDiff < colDiff)
			return colDiff;

		return rowDiff;
	}

	KeyCoordinate GetKeyCoordinate(char key)
	{
		return KeyboardMap[key];
	}

	bool AreSameHand(char targetChar, char typoChar)
	{
		int targetKeyCol = GetKeyCoordinate(targetChar).column;
		int typoKeyCol = GetKeyCoordinate(typoChar).column;
		bool isTargetKeyLeftHand, isTypoKeyOnSameHand;

		isTargetKeyLeftHand = (0 <= targetKeyCol && targetKeyCol <= 4);

		if (isTargetKeyLeftHand)
		{
			//Check if typoChar is in the same cols for left hand 
			isTypoKeyOnSameHand = (0 <= typoKeyCol && typoKeyCol <= 4);
		}
		else {
			//FirstKey is on RH, check if typoChar is in the same cols for right hand 
			isTypoKeyOnSameHand = (5 <= typoKeyCol && typoKeyCol <= 9);
		}

		return isTypoKeyOnSameHand;
	}

	bool AreSameFinger(char targetChar, char typoChar)
	{
		int targetKeyCol = GetKeyCoordinate(targetChar).column;
		int typoKeyCol = GetKeyCoordinate(typoChar).column;

		int targetKeyFinger = (targetKeyCol >= 5) ? targetKeyCol - 5 : targetKeyCol;
		int typoKeyFinger = (typoKeyCol >= 5) ? typoKeyCol - 5 : typoKeyCol;

		return targetKeyFinger == typoKeyFinger;
	}

	char FindActualPrevTargetChar(const char* s, const char* t, int i, int j, int prev, Cell **arr)
	{
		//if (prev == -1)
		//	return ' ';

		//A Delete operation needs to look one further back. 
		while (prev == DELETE && i > 0)
		{
			prev = arr[--i][j].previous;
		}

		//If s[i-1]'s operation was either Insert, Sub, Match, or Transpose, the "actual" target character is t[i-1]
		return (i >= 1) ? t[i - 1] : ' ';
	}


	int DeletionWrapper(const char * s, const char * t, int i, int j)
	{
		return 0;
	}

	int SubstitutionCost(char targetChar, char typoChar)
	{
		int subCost = 0;

		if (targetChar == typoChar)
		{
			return 0;
		}

		//Space for anything or anything for space
		if (targetChar == ' ' || typoChar == ' ')
		{
			return 6;
		}
		else {
			//Key for another on same hand - d(k1, k2)
			if (AreSameHand(targetChar, typoChar))
			{
				subCost = KeyboardDistance(targetChar, typoChar);
			}
			//Key for another on other hand - 
			else {
				//On same finger - 1
				if (AreSameFinger(targetChar, typoChar))
				{
					subCost = 1;
				}
				//On different finger - 5
				else {
					subCost = 5;
				}
			}
		}
		return subCost;
	}


	int InsertBefore(const char * s, const char * t, int i, int j, Cell arr[][100])
	{
		if (i == (strlen(s) - 1))
		{
			return INFIN;
		}

		//Want to compare current t[j] with s's next character (s[i+1])
		char firstChar = t[j], secondChar = s[i + 1];
		int insCost = 0;

		//Repeated character - 1
		if (firstChar == secondChar)
		{
			return 1;
		}

		//Character before a space - 6
		if (firstChar != ' ' && secondChar == ' ')
		{
			insCost = 6;
		}
		//Before another key on same hand - d(k1, k2)
		if (AreSameHand(firstChar, secondChar))
		{
			insCost = KeyboardDistance(firstChar, secondChar);
		}
		//Before a key on opposite hand
		else 
		{
			insCost = 5;
		}

		return insCost;
	}

	int InsertAfter(const char * s, const char * t, int i , int j, Cell arr[][100])
	{
		if (i == 1)
		{
			return INFIN;
		}

		//Want to compare current t[j] with s's previous character (s[i-1])
		char firstChar = s[i - 1], secondChar = t[j];
		int insCost = 0;

		//Repeated character - 1
		if (firstChar == secondChar)
		{
			return 1;
		}

		//Space after key on .. 
		if (secondChar == ' ')
		{
			//Bottom row - 2
			if (GetKeyCoordinate(firstChar).row == 3)
				insCost = 2;
			//Something else - 6
			else
				insCost = 6;
		}
		//Character after a space - 6
		if (firstChar == ' ' && secondChar != ' ')
		{
			insCost = 6;
		}


		//After another key on same hand - d(k1, k2)
		if (AreSameHand(firstChar, secondChar))
		{
			insCost = KeyboardDistance(firstChar, secondChar);
		}
		//After a key on opposite hand - 5
		else {
			insCost = 5;
		}

		return insCost;
	}

	int InsertionWrapper(const char * s, const char * t, int i, int j, Cell arr[][100])
	{
		int minInsCost = InsertBefore(s, t, i , j, arr);
		int insAfterCost = InsertAfter(s, t, i , j, arr);

		if (insAfterCost < minInsCost)
		{
			minInsCost = insAfterCost;
		}

		return minInsCost;
	}

	int InsertionCost(char firstChar, char secondChar)
	{
		int insCost = 0;
		int temp = 0;

		//Repeated character - 1
		//if (firstChar == secondChar)
		//{
		//	insCost = 1;
		//	return insCost;
		//}

		//Space after key on bottom row - 2
		if (GetKeyCoordinate(firstChar).row == 3 && secondChar == ' ')
		{
			insCost = 2;
			return insCost;
		}

		//Space after something else - 6
		//Character before a space - 6
		if (secondChar == ' ' || firstChar == ' ')
		{
			insCost = 6;
		}

		//Before or after another key on same hand - d(k1, k2)
		if (AreSameHand(firstChar, secondChar))
		{
			temp = KeyboardDistance(firstChar, secondChar);

			//if this costs less than a previouis cost
			if (insCost != 0 && temp < insCost)
			{
				insCost = temp;
			}
			//if there is no previous cost
			else if (insCost == 0)
			{
				insCost = temp;
			}
			//else cheaper function already exists,
			//keep subCost at the same value
		}

		//Before or after a key on opposite hand - 5
		if (!AreSameHand(firstChar, secondChar))
		{
			temp = 5;

			//if this costs less than a previouis cost
			if (insCost != 0 && temp < insCost)
			{
				insCost = temp;
			}
			//if there is no previous cost
			else if (insCost == 0)
			{
				insCost = temp;
			}
			//else cheaper function already exists,
			//keep subCost at the same value
		}
		return insCost;
	}

	int DeletionCost(char firstChar, char secondChar)
	{
		int delCost = 0;
		int temp = 0;

		//Deleting Repeated character - 1
		if (firstChar == secondChar)
		{
			delCost = 1;
			return delCost;
		}

		//Space - 3
		if (secondChar == ' ')
		{
			delCost = 3;
		}

		//Character after another key on same hand - 2
		if (AreSameHand(firstChar, secondChar))
		{
			delCost = 2;
			return 2;
		}

		//Character after space or key on different hand - 6
		if (firstChar == ' ' || !AreSameHand(firstChar, secondChar))
		{
			//if subCost is not 3
			if (delCost == 0)
			{
				delCost = 6;
			}
		}

		//First character in string - 6
		//************************************************
		//TODO: find out if character is first in string
		//************************************************

		return delCost;
	}

	int TranspositionCost(char firstChar, char secondChar)
	{
		int transpCost = 0;

		//Transposing Space with anything else - 3
		if (firstChar == ' ' || secondChar == ' ')
		{
			transpCost = 3;
		}

		//Keys on different hands - 1
		if (!AreSameHand(firstChar, secondChar))
		{
			transpCost = 1;
			return transpCost;
		}

		//Keys on the same hand - 2
		if (AreSameHand(firstChar, secondChar))
		{
			transpCost = 2;
			return transpCost;
		}

		return transpCost;
	}
}

