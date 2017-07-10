#include "StringCompareUtil.h"
#include <map>
#include <stdlib.h>

std::map<char, StringCompareUtil::KeyCoordinate> KeyboardMap =
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


StringCompareUtil::StringCompareUtil()
{
}


StringCompareUtil::~StringCompareUtil()
{
}

int StringCompareUtil::KeyboardDistance(char firstChar, char secondChar)
{
	int rowDiff = abs(GetKeyCoordinate(secondChar).row - GetKeyCoordinate(firstChar).row);
	int colDiff = abs(GetKeyCoordinate(secondChar).column - GetKeyCoordinate(firstChar).column);

	if (rowDiff < colDiff)
		return colDiff;

	return rowDiff;
}

StringCompareUtil::KeyCoordinate StringCompareUtil::GetKeyCoordinate(char key)
{
	return KeyboardMap[key];
}

bool StringCompareUtil::AreSameHand(char firstChar, char secondChar)
{
	int firstKeyCol = GetKeyCoordinate(firstChar).column;
	int secondKeyCol = GetKeyCoordinate(secondChar).column;
	bool isFirstKeyLeftHand, isSecondKeyOnSameHand;

	isFirstKeyLeftHand = (0 <= firstKeyCol && firstKeyCol <= 3);

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

bool StringCompareUtil::AreSameFinger(char firstChar, char secondChar)
{
	int firstKeyCol = GetKeyCoordinate(firstChar).column;
	int secondKeyCol = GetKeyCoordinate(secondChar).column;

	int firstKeyFinger = (firstKeyCol >= 5) ? firstKeyCol - 5 : firstKeyCol;
	int secondKeyFinger = (secondKeyCol >= 5) ? secondKeyCol - 5 : secondKeyCol;

	return firstKeyFinger == secondKeyFinger;
}
