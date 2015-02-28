/*********************************************************************************
 * File: 		Main.cpp
 * Project: 	ECE 585 L2 Cache Model
 * Version: 	0.1
 * Date:		11/20/2014
 * Author:		Prakhya, Doguparthi, Mutyala, Kothakapu
 *********************************************************************************/

#include <iostream>
#include "Defines.h"
#include "Line.h"
#include "Set.h"
#include "Functions.h"

using namespace std;

float hits = 0;
float misses = 0;
unsigned int reads = 0;
unsigned int writes = 0;
unsigned int totalReferences = 0;
int lruBits[WAYS-1];

int main()
{
	init();
	string hexAddress,binAddress;
	int mainIndex = 0;
	int exit = 0;
	int mainTag = 0;
	char choice0;
	string traceFile;
	ifstream myfile;

	hits,misses,reads,writes = 0;
	welcome();
	cout << endl;
	while (exit == 0)
	{
		cout << "1. Import and run a trace file" << endl;
		cout << "2. Test this application" << endl;
		cout << "3. Exit" << endl;
		cout << "Please enter your choice: ";
		cin >> choice0;
		switch (choice0)
		{
		case '1':
			cout << "Please enter a file name: ";
			cin >> traceFile;
			myfile.open(traceFile.c_str());
			if (myfile.is_open())
			{
				while (getline (myfile,trace) )
    			{
					traceOperation = trace[0];
					if ((traceOperation == '8') || (traceOperation == '9'))
					{
						if (traceOperation == '8')
							init();
						else if (traceOperation == '9')
							printValid();
					}
					else
					{
      					traceAddress = trace.substr(2,8);
						binAddress = getBinaryAddress(traceAddress);
						mainIndex = get_index(binAddress);
						mainTag = get_tag(binAddress);
						if (mainIndex < NUMSETS)
							cacheBehaviour (mainIndex, traceOperation, mainTag);
						else
							cout << "Sorry! Index of " << traceAddress << " exceeds number of sets" << endl;
    				}
				}
				myfile.close();
				cout << "Task Completed!" << endl;
				cache_stats();
			}
			else cout << "Unable to open file" << endl;
			break;
		case '2':
			traceOperation = displayMenu();
			if (traceOperation == '8')
			{
				init();
			}
			else if (traceOperation == '9')
			{
				printValid();
			}
			else if (traceOperation == 's')
			{
				cache_stats();
			}
			else if (traceOperation == 'q')
			{
				break;
			}
			else if (traceOperation == '0' || traceOperation != '1' || traceOperation != '2' ||
				traceOperation == '3' || traceOperation != '4' || traceOperation != '5' ||
				traceOperation == '6' || traceOperation != '7' || traceOperation != 'a')
			{
				cout << "Please enter a hexa decimal address:";
				cin >> hexAddress;
				binAddress = getBinaryAddress(hexAddress);
				mainIndex = get_index(binAddress);
				mainTag = get_tag(binAddress);
				if (mainIndex < NUMSETS)
					cacheBehaviour (mainIndex, traceOperation, mainTag);
				else
					cout << "Sorry! Index of " << hexAddress << " exceeds number of sets" << endl;
			}
			break;
		case '3':
			exit = 1;
			break;
		default:
			cout << "Please enter a correct choice" << endl;
			break;
		}
	}
	return 0;
}
