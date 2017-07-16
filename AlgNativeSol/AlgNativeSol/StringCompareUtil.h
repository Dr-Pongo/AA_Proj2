#pragma once
#ifndef STRINGCOMPAREUTIL_H
#define STRINGCOMPAREUTIL_H


namespace StringCompareUtil
{
	struct Cell
	{
		int cost;
		int previous;
		char letter;
	};

	struct KeyCoordinate
	{
		int row;
		int column;
	};

	int KeyboardDistance(char, char);
	KeyCoordinate GetKeyCoordinate(char);
	bool AreSameHand(char firstChar, char secondChar);
	bool AreSameFinger(char firstChar, char secondChar);
	char FindActualPrevTargetChar(const char* s, const char* t, int i, int j, int prev, Cell **arr);

	int InsertionWrapper(const char * s, const char * t, int i, int j, Cell arr[][100]);
	int InsertBefore(const char * s, const char * t, int i, int j, Cell **arr);
	int InsertAfter(const char * s, const char * t, int i, int j, Cell **arr);

	int DeletionWrapper(const char * s, const char * t, int i, int j);

	int SubstitutionCost(char firstChar, char secondChar);
	int InsertionCost(char firstChar, char secondChar);
	int DeletionCost(char firstChar, char secondChar);
	int TranspositionCost(char firstChar, char secondChar);
};
#endif // !STRINGCOMPAREUTIL_H

