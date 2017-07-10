#pragma once
class StringCompareUtil
{
public:
	struct KeyCoordinate
	{
		int row;
		int column;
	};

	StringCompareUtil();
	~StringCompareUtil();

	int KeyboardDistance(char, char);
	KeyCoordinate GetKeyCoordinate(char);
	bool AreSameHand(char firstChar, char secondChar);
	bool AreSameFinger(char firstChar, char secondChar);
};
