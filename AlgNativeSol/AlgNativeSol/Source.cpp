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
using namespace std;

struct Cell
{
	int cost;
	int previous;
};

const int MAXLEN = 1000; /*Added in*/

int string_compare(const char * s, const char * t, int i, int j);
int match(char c, char d);
int indel(char c);
Cell arr[100][100];


int wrapper_function(string target, string typo) 
{
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 100; j++)
		{
			arr[i][j].cost = -1;
		}
	}

	
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
	if (i == 0) return(j * indel(' '));
	if (j == 0) return(i * indel(' '));

	int k;
	int opt[3];
	int lowest_cost;
	
	opt[MATCH] = string_compare(s, t, i - 1, j - 1) + match(s[i], t[j]);
	opt[INSERT] = string_compare(s, t, i, j - 1) + StringCompareUtil::InsertionCost(s[i], t[j]);
	opt[DELETE] = string_compare(s, t, i - 1, j) + StringCompareUtil::DeletionnCost(s[i], s[j]);

	lowest_cost = opt[MATCH];
	
	for (k = INSERT; k <= DELETE; k++)
	{
		if (opt[k] < lowest_cost)
		{
			lowest_cost = opt[k];
		}
	}
	arr[i][j].cost = lowest_cost;
	return arr[i][j].cost;
}

int match(char c, char d)
{
	if (c == d)
	{
		return(0);
	}
	else
	{
		return(1);
	}
}

int indel(char c)
{
	return(1);
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
	}

	//cout << wrapper_function() << endl;
	system("pause");
}