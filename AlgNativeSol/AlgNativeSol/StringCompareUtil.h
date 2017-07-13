#pragma once
namespace StringCompareUtil
{
	struct KeyCoordinate
	{
		int row;
		int column;
	};

	int KeyboardDistance(char, char);
	KeyCoordinate GetKeyCoordinate(char);
	bool AreSameHand(char firstChar, char secondChar);
	bool AreSameFinger(char firstChar, char secondChar);
	int SubstitutionCost(char firstChar, char secondChar);
	int InsertionCost(char firstChar, char secondChar);
	int DeletionnCost(char firstChar, char secondChar);
};
