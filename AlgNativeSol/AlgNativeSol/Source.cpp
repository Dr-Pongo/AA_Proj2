#include <iostream>
#include <fstream>
#include <string>
#include "StringCompareUtil.h"

#define MATCH 0 /* enumerated type symbol for match */ 
#define INSERT 1 /* enumerated type symbol for insert */ 
#define DELETE 2 /* enumerated type symbol for delete */
#define TRANSP 3
#define INFIN 2000000000
using namespace std;

const int MAXLEN = 100; /*Added in*/

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
					arr[i][j].letter = ' ';
				}
				else {
					arr[i][j].cost = StringCompareUtil::InsertAfter(target.c_str(), typo.c_str(), i , j, arr);
					arr[i][j].previous = INSERT;
					if((j < target.length()) && (j < typo.length()))
						arr[i][j].letter = typo[j];
				}
			}	//Init first column (Base case)
			else if (j == 0)
			{
				//So it doesn't overwrite [0,0]
				if (i != 0)
				{
					arr[i][j].cost = StringCompareUtil::DeletionWrapper(target.c_str(), typo.c_str(), i, j, arr);
					arr[i][j].previous = DELETE;
				}
			}
			else {
					arr[i][j].cost = -1;
					arr[i][j].previous = -1;
			}
		}
	}

	return string_compare(target.c_str(), typo.c_str(), target.length(), typo.length());
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


	opt[INSERT] = string_compare(s, t, i, j - 1) + StringCompareUtil::InsertionWrapper(s, t, i, j, arr);


	opt[DELETE] = string_compare(s, t, i - 1, j) + StringCompareUtil::DeletionWrapper(s, t, i, j, arr);

	if ((i > 1 && j > 1) && (s[i] == t[j - 1] && s[i - 1] == t[j]))
	{
		opt[TRANSP] = string_compare(s, t, i - 2, j - 2) + StringCompareUtil::TranspositionWrapper(s, t, i, j, arr);
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
	if ((arr[i][j].previous == INSERT) || (arr[i][j].previous == MATCH && s[i] != t[j]))
		arr[i][j].letter = t[j];

	return arr[i][j].cost;
}

void output_recursive(string target, string typo, int i, int j, ofstream& outputFile)
{
	//Base case where we reached [0,0]
	if (arr[i][j].previous == -1)
	{
		return;
	}

	//Because our string "technically" starts at index 1 and the project wants 0-based,
	// need to adjust the current i or j to (i-1)/(j-1)
	if (arr[i][j].previous == MATCH)
	{
		output_recursive(target, typo, i - 1, j - 1, outputFile);
		if (target[i] == typo[j])
			return;
		outputFile << "Substitute " << arr[i][j].letter << " at " << j  - 1 << endl;
		return;
	}

	if (arr[i][j].previous == INSERT)
	{
		output_recursive(target, typo, i, j - 1, outputFile);
		outputFile << "Insert " << arr[i][j].letter << " before " << j - 1 << endl; //Not sure if j-1 is correct, but it gives the correct output for the Inserts in the example
		return;
	}

	if (arr[i][j].previous == DELETE)
	{
		output_recursive(target, typo, i - 1, j, outputFile);
		outputFile << "Delete " << i << endl;
		return;
	}

	if (arr[i][j].previous == TRANSP)
	{
		output_recursive(target, typo, i - 2, j - 2, outputFile);
		outputFile << "Transpose " << (j - 2) << "-" << (j - 1) << endl;
		return;
	}

}


int main() {
	string typo;
	string target;
	string pairs;
	int numPairs = 0;
	ifstream inputFile("input.txt");
	ofstream outputFile;
	outputFile.open("output.txt");

	if (!inputFile)
	{
		cout << "Err reading input.txt" << endl;
		system("pause");
		return -1;
	}
	if (!outputFile)
	{
		cout << "Err accessing output.txt" << endl;
		system("pause");
		return -1;
	}

	// Get number of pairs
	getline(inputFile, pairs, '\n');
	numPairs = atoi(pairs.c_str());

	// Get input strings
	for(int i = 0; i < numPairs; i++)
	{
		if (inputFile.eof()) // error checking
		{
			cout << "Pair number mismatch\n\r Exiting..." << endl;
			system("pause");
			return -1;
		}

		getline(inputFile, target, '\n');
		getline(inputFile, typo, '\n');
		
		target.insert(0, 1, ' ');
		typo.insert(0, 1, ' ');

		outputFile << wrapper_function(target, typo) << endl;
		output_recursive(target, typo, target.length() - 1, typo.length() - 1, outputFile);
		outputFile << "\n"; // Blank line between tests

	}

	inputFile.close();
	outputFile.close();
}