#include <stdio.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include <string>
#include "StringCompareUtil.h"

#define MATCH 0 /* enumerated type symbol for match */ 
#define INSERT 1 /* enumerated type symbol for insert */ 
#define DELETE 2 /* enumerated type symbol for delete */
#define TRANSP 3
#define INFIN 2000000000
using namespace std;


const int MAXLEN = 1000; /*Added in*/

int string_compare(const char * s, const char * t, int i, int j);
StringCompareUtil::Cell arr[100][100];


int wrapper_function(string target, string typo)
{
	for (int i = 0; i < MAXLEN; i++)
	{
		for (int j = 0; j < MAXLEN; j++)
		{	//Init first row (Base case)
			if (i == 0)
			{
				if (j == 0)
				{	//Setting the [0,0] cell.
					arr[i][j].cost = 0;
					arr[i][j].previous = -1;
				}
				else {
					arr[i][j].cost = j;
					arr[i][j].previous = INSERT;
					arr[i][j].letter = target[i];
				}
			}	//Init first column (Base case)
			else if (j == 0)
			{
				//So it doesn't overwrite [0,0]
				if (i != 0)
				{
					arr[i][j].cost = i;
					arr[i][j].previous = DELETE;
				}
			}
			else {
					arr[i][j].cost = -1;
					arr[i][j].previous = -1;
			}
		}
	}

	// Inserting space at the beginning of each string
	target.insert(0, 1, ' ');
	typo.insert(0, 1, ' ');
	return string_compare(typo.c_str(), target.c_str(), typo.length(), target.length());

}

int string_compare(const char * s, const char * t, int i, int j)
{
	if (arr[i][j].cost != -1)
	{
		return arr[i][j].cost;
	}

	int k;
	int opt[4];
	StringCompareUtil::Cell lowest_cost;

	//Substitution costs don't care about previous characters, only the current i & j chars
	opt[MATCH] = string_compare(s, t, i - 1, j - 1) + StringCompareUtil::SubstitutionCost(s[i], t[j]);

	opt[INSERT] = string_compare(s, t, i, j - 1) + StringCompareUtil::InsertionCost(s[i], t[j]);

	opt[DELETE] = string_compare(s, t, i - 1, j) + StringCompareUtil::DeletionCost(s[i], s[j]);

	if ((i > 1 && j > 1) && (s[i] == t[j - 1] && s[i - 1] == t[j]))
	{
		opt[TRANSP] = string_compare(s, t, i - 2, j - 2) + StringCompareUtil::TranspositionCost(s[i], s[j]);
	}
	else
	{
		opt[TRANSP] = INFIN; //Add a large cost so that string_compare won't pick this as a min. cost
	}


	lowest_cost.cost = opt[MATCH];
	lowest_cost.previous = MATCH;

	for (k = INSERT; k <= TRANSP; k++)
	{
		if (opt[k] < lowest_cost.cost)
		{
			lowest_cost.cost = opt[k];
			lowest_cost.previous = k;
		}
	}

	arr[i][j].cost = lowest_cost.cost;
	arr[i][j].previous = lowest_cost.previous;
	if ((arr[i][j].previous == INSERT) || (arr[i][j].previous == MATCH))
		arr[i][j].letter = s[i];

	return arr[i][j].cost;
}

void print(int i, char c, int check)
{
	switch (check)
	{
		case INSERT:
			cout << "Insert " << c << " before " << i << endl;
			break;
		case DELETE:
			cout << "Delete " << i << endl;
			break;
		case TRANSP:
			cout << "Transpose " << i << "-" << (i + 1) << endl;
			break;
		case MATCH:
			cout << "Substitute " << c << " at " << i << endl;
			break;
	}
	return;
}

void output()
{
	int i = 0;
	int j = 0;
	int itmp = i;
	int jtmp = j;
	int func = 0;

	while ((j < MAXLEN - 1) || (i < MAXLEN - 1))
	{
		StringCompareUtil::Cell minFind;
		minFind.cost = 100;
		
		if (j == (MAXLEN - 1))
		{
			print(i, arr[i][j].letter, arr[++i][j].previous);
			i++;
			continue;
		}
		if (i == (MAXLEN - 1))
		{
			j++;
			print(i, arr[i][j].letter, arr[i][++j].previous);
			continue;
		}

		if ((arr[i + 1][j].cost < minFind.cost) && (arr[i + 1][j].cost >= 0))
		{
			minFind.cost = arr[i + 1][j].cost;
			minFind.letter = arr[i + 1][j].letter;
			itmp = ++i;
			jtmp = j;
		}
		if ((arr[i][j + 1].cost < minFind.cost) && (arr[i][j + 1].cost >= 0))
		{
			minFind.cost = arr[i][j + 1].cost;
			minFind.letter = arr[i][j + 1].letter;
			itmp = i;
			jtmp = ++j;
		}
		if ((arr[i + 1][j + 1].cost < minFind.cost) && (arr[i + 1][j].cost >= 0))
		{
			minFind.cost = arr[i + 1][j + 1].cost;
			minFind.letter = arr[i + 1][j + 1].letter;
			itmp = ++i;
			jtmp = ++j;
		}
		i = itmp;
		j = jtmp;

		print(i, minFind.letter, arr[i][j].previous);
	}
	return;
}

int main() {
	string typo;
	string target;
	string pairs;
	int numPairs = 0;
	ifstream inputFile("input.txt");

	if (!inputFile)
	{
		cout << "Err reading input.txt" << endl;
		system("pause");
		return -1;
	}

	// Get number of pairs
	getline(inputFile, pairs, '\n');
	numPairs = atoi(pairs.c_str());

	// Get input strings
	for(int i = 0; i < numPairs - 1; i++)
	{
		if (inputFile.eof()) // error checking
		{
			cout << "Pair number mismatch\n\r Exiting..." << endl;
			system("pause");
			return -1;
		}
		getline(inputFile, target, '\n');
		getline(inputFile, typo, '\n');
		cout << wrapper_function(target, typo) << endl;
		output();
	}

	//cout << wrapper_function() << endl;
	system("pause");
}