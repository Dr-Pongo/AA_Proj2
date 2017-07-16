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
		{ '1',{ 0, 0, 'P' } },
		{ '2',{ 0, 1, 'R' } },
		{ '3',{ 0, 2, 'M' } },
		{ '4',{ 0, 3, 'I' } },
		{ '5',{ 0, 4, 'I' } },
		{ '6',{ 0, 5, 'I' } },
		{ '7',{ 0, 6, 'I' } },
		{ '8',{ 0, 7, 'M' } },
		{ '9',{ 0, 8, 'R' } },
		{ '0',{ 0, 9, 'P' } },
		//Second row
		{ 'q',{ 1, 0, 'P' } },
		{ 'w',{ 1, 1, 'R' } },
		{ 'e',{ 1, 2, 'M' } },
		{ 'r',{ 1, 3, 'I' } },
		{ 't',{ 1, 4, 'I' } },
		{ 'y',{ 1, 5, 'I' } },
		{ 'u',{ 1, 6, 'I' } },
		{ 'i',{ 1, 7, 'M' } },
		{ 'o',{ 1, 8, 'R' } },
		{ 'p',{ 1, 9, 'P' } },
		//Third row
		{ 'a',{ 2, 0, 'P' } },
		{ 's',{ 2, 1, 'R' } },
		{ 'd',{ 2, 2, 'M' } },
		{ 'f',{ 2, 3, 'I' } },
		{ 'g',{ 2, 4, 'I' } },
		{ 'h',{ 2, 5, 'I' } },
		{ 'j',{ 2, 6, 'I' } },
		{ 'k',{ 2, 7, 'M' } },
		{ 'l',{ 2, 8, 'R' } },
		{ ';',{ 2, 9, 'P' } },
		//Fourth row
		{ 'z',{ 3, 0, 'P' } },
		{ 'x',{ 3, 1, 'R' } },
		{ 'c',{ 3, 2, 'M' } },
		{ 'v',{ 3, 3, 'I' } },
		{ 'b',{ 3, 4, 'I' } },
		{ 'n',{ 3, 5, 'I' } },
		{ 'm',{ 3, 6, 'I' } },
		{ ',',{ 3, 7, 'M' } },
		{ '.',{ 3, 8, 'R' } },
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
		return GetKeyCoordinate(targetChar).finger == GetKeyCoordinate(typoChar).finger;
	}

	char FindActualPrevTargetChar(const char* s, const char* t, int i, int j, Cell arr[][100])
	{
		//TODO: Read s[i-1]'s parent operation?
		int prev = arr[i - 1][j].previous;

		//A Delete operation needs to look one further back. 
		while (prev == DELETE && i > 1)
		{
			prev = arr[--i][j].previous;
		}

		//If s[i-1]'s operation was either Insert, Sub, Match, or Transpose, the "actual" target character is t[i-1]
		return t[i-1];
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


	int InsertionWrapper(const char * s, const char * t, int i, int j, Cell arr[][100])
	{
		int minInsCost = InsertBefore(s, t, i, j);
		int insAfterCost = InsertAfter(s, t, i, j, arr);

		if (insAfterCost < minInsCost)
		{
			minInsCost = insAfterCost;
		}

		return minInsCost;
	}

	int InsertBefore(const char * s, const char * t, int i, int j)
	{
		if (i == (strlen(s) - 1))
		{
			return INFIN;
		}

		//Want to compare current t[j] with s's next character (s[i+1])
		char firstChar = t[j], secondChar = s[i + 1];

		//Repeated character - 1
		if (firstChar == secondChar)
		{
			return 1;
		}

		//Before another key on same hand - d(k1, k2)
		if (AreSameHand(firstChar, secondChar))
		{
			return KeyboardDistance(firstChar, secondChar);
		}
		//Before a key on opposite hand
		else 
		{
			return 5;
		}

		//Character before a space - 6
		if (firstChar != ' ' && secondChar == ' ')
		{
			return 6;
		}
	}

	int InsertAfter(const char * s, const char * t, int i , int j, Cell arr[][100])
	{
		if (i == 1)
		{
			return INFIN;
		}

		//Want to compare current t[j] with s's previous character (s[i-1])
		char firstChar = (i != 0) ? FindActualPrevTargetChar(s, t, i, j, arr) : s[i-1];
		char secondChar = t[j];

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
				return 2;
			//Something else - 6
			else
				return 6;
		}

		//After another key on same hand - d(k1, k2)
		if (AreSameHand(firstChar, secondChar))
		{
			return KeyboardDistance(firstChar, secondChar);
		}
		//After a key on opposite hand - 5
		else {
			return 5;
		}

		//Character after a space - 6
		if (firstChar == ' ' && secondChar != ' ')
		{
			return 6;
		}
	}



	int DeletionWrapper(const char * s, const char * t, int i, int j, Cell arr[][100])
	{
		//Deleting first character in string
		if (i == 1)
			return 6;

		char firstChar = (j != 0) ? FindActualPrevTargetChar(s, t, i, j, arr) : s[i-1];
		char secondChar = s[i];

		return DeletionCost( firstChar, secondChar);
	}

	int DeletionCost(char firstChar, char secondChar)
	{
		//Deleting Repeated character - 1
		if (firstChar == secondChar)
		{
			return 1;

		}

		//Character after another key on same hand - 2
		if (AreSameHand(firstChar, secondChar))
		{
			return 2;
		}

		//Space - 3
		if (secondChar == ' ')
		{
			return 3;
		}

		

		//Deleting character after ...
		if (secondChar != ' ')
		{
			//Another key on same hand
			if (AreSameHand(firstChar, secondChar))
			{
				return 6;
			}

			//Character after space or key on different hand - 6
			if(firstChar == ' ' || !AreSameHand(firstChar, secondChar))
			{
				return 6;
			}
		}

	}


	
	int TranspositionWrapper(const char * s, const char * t, int i, int j, Cell arr[][100])
	{
		return TranspositionCost(FindActualPrevTargetChar(s, t, i, j, arr), t[j]);
	}

	int TranspositionCost(char firstChar, char secondChar)
	{
		//Keys on different hands - 1
		if (!AreSameHand(firstChar, secondChar))
		{
			return 1;
		}

		//Keys on the same hand - 2
		if (AreSameHand(firstChar, secondChar))
		{
			return 2;
		}

		//Transposing Space with anything else - 3
		if (firstChar == ' ' || secondChar == ' ')
		{
			return 3;
		}
	}
}

