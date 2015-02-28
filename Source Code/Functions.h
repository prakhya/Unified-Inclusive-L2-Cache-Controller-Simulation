/*********************************************************************************
 * File: 		Functions.h
 * Project: 	ECE 585 L2 Cache Model
 * Version: 	0.1
 * Date:		11/20/2014
 * Author:		Prakhya, Doguparthi, Mutyala, Kothakapu
 *********************************************************************************/

#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#include <iostream>
#include <sstream>
#include <bitset>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <fstream>
#include "Defines.h"

using namespace std;

string functionsBinAddress,trace,traceAddress,binTag,binIndex,BusAddress,dec2hex;
char traceOperation;
int result = 0;

/**************************************
* Prints welcome screen
* Inputs: None
* Output: None
**************************************/

void welcome()
{
	cout <<"********************************************************************************" << endl;
	cout << endl;
	cout <<"			Welcome to Cache Simulation Software" << endl;
	cout <<"		Credits: Prakhya, Doguparthi, Mutyala, Kothakapu" << endl;
	cout << endl;
	cout <<"********************************************************************************" << endl;
}

/**************************************
* Clears the screen with 100 new lines
* Inputs: None
* Output: None
**************************************/

void clear_screen()
{
	for (int i = 0; i <= 100; i++)
	{
		cout << endl;
	}
}

/**************************************
* Displays the main user menu and returns the users choice
* Inputs: None
* Output: char - user choice
**************************************/

char displayMenu()
{
	char choice;											// User Choice
	clear_screen();
	cout << "0. Read request from L1 Data Cache" << endl;
	cout << "1. Write request from L1 Data Cache" << endl;
	cout << "2. Read request from L1 Instruciton Cache" << endl;
	cout << "3. Snooped Invalidate Command" << endl;
	cout << "4. Snooped Read Request" << endl;
	cout << "5. Snooped Write Request" << endl;
	cout << "6. Snooped Read With Intent to Modify" << endl;
	cout << "7. Invalidate a line" << endl;
	cout << "8. Clear the cache and reset all states" << endl;
	cout << "9. Print contents and state of each valid cache line" << endl;
	cout << "a. Get state of a line" << endl;
	cout << "s. Display current cache stats" << endl;
	cout << "q. Back" << endl;
	cout << "Please select from the above list: ";
	cin >> choice;
	return choice;
}

/**************************************
* Convert given hexadecimal address to binary address
* Inputs: string - Hexadecimal Address
* Output: string - Binary Address
**************************************/

string getBinaryAddress(string functionsHexAddress)
{
    stringstream ss;
    unsigned n;
	BusAddress = functionsHexAddress;
    ss << hex << functionsHexAddress;
    ss >> n;
    bitset<ADDRESSBITS> b(n);
	if (SILENTFUNCTIONS == 0)
	{
		cout << "Binary address of given hexadecimal address: ";
		cout << b.to_string() << endl;
	}
	functionsBinAddress = b.to_string();
	return functionsBinAddress;
}

/**************************************
* Returns the index from a Binary Address
* Inputs: string - Binary Address
* Output: int - returns the index from the address passed into it
**************************************/

int get_index(string functionsBinIndex)
{
	binIndex = functionsBinIndex.substr(13,int(INDEXBITS));
	if (SILENTFUNCTIONS == 0)
	{
		cout << "Binary Index: ";
		cout << binIndex << endl;
	}
	int decIndex = 0;
	reverse(binIndex.begin(), binIndex.end());
	for (unsigned int i = 0; i < binIndex.size(); ++i) {
		decIndex += int((int(binIndex[i]) - 48)*pow(2, i));
	}
	if (SILENTFUNCTIONS == 0)
	{
		cout << "Decimal Index: ";
		cout << decIndex << endl;
	}
	return decIndex;	
}

/**************************************
* Returns the tag from a Binary Address
* Inputs: string - Binary Address
* Output: int - returns the tag from the address passed into it
**************************************/

int get_tag(string functionsBinTag)
{
	binTag = functionsBinTag.substr(0,int(TAGBITS));
	if (SILENTFUNCTIONS == 0)
	{
		cout << "Binary Tag: ";
		cout << binTag << endl;
	}
	int decTag = 0;
	reverse(binTag.begin(), binTag.end());
	for (unsigned int i = 0; i < binTag.size(); ++i) {
		decTag += int((int(binTag[i]) - 48)*pow(2, i));
	}
	if (SILENTFUNCTIONS == 0)
	{
		cout << "Decimal Tag: ";
		cout << decTag << endl;
	}
	return decTag;
}

/**************************************
* Increments cache hit counter
* Inputs: None
* Output: None
**************************************/
void cache_hit()
{
	if (SILENTFUNCTIONS == 0)
		cout << "Hit incremented" << endl;
	hits++;
}

/**************************************
* Increments cache miss counter
* Inputs: None
* Output: None
**************************************/
void cache_miss()
{
	if (SILENTFUNCTIONS == 0)
		cout << "Miss incremented" << endl;
	misses++;
}

/**************************************
* Shows the cache statistics
* Inputs: None
* Output: None
**************************************/
void cache_stats()
{
	float ratio = 0;
	cout << "Displaying Cache Statistics" << endl;
	cout << "Total Number of References to Cache: " << totalReferences << endl;
	cout << "Number of Cache Reads: " << reads << endl;
	cout << "Number of Cache Writes: " << writes << endl;
	cout << "Number of Cache Hits: " << hits << endl;
	cout << "Number of Cache Misses: " << misses << endl;
	cout << "Cache Hit Ratio: " << (hits/totalReferences)*100 << "%" << endl;
}

/**************************************
* Increments cache write counter
* Inputs: None
* Output: None
**************************************/
void write_increment()
{
	if (SILENTFUNCTIONS == 0)
		cout << "Write incremented" << endl;
	writes++;
}

/**************************************
* Increments cache read counter
* Inputs: None
* Output: None
**************************************/
void read_increment()
{
	if (SILENTFUNCTIONS == 0)
		cout << "Read incremented" << endl;
	reads++;
}

int getSnoopResult()
{
	result = 0;
	if ((functionsBinAddress[30] == '0') && (functionsBinAddress[31] == '0'))
	{
		result = 0;
	}
	else if ((functionsBinAddress[30] == '0') && (functionsBinAddress[31] == '1'))
	{
		result = 1;
	}
	else if ((functionsBinAddress[30] == '1') && (functionsBinAddress[31] == '0'))
	{
		result = 2;
	}
	return result;
}

void BusOperation (int BusOp)
{
	if (PRINT == 1 )
		cout << "BusOp: " << BusOp << " " << "Address: " << BusAddress << " " <<"Snoop Result: " << result << endl;
}

void PutSnoopResult(int fSnoop)
{
	if (PRINT == 1 )
		cout << "Address: " << BusAddress << " " <<"SnoopResult: " << fSnoop << endl;
}

void MessageToL1Cache (int BusOp)
{
	if (PRINT == 1 )
		cout << "L1: " << BusOp << " " << BusAddress << endl;
}
#endif
