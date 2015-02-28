/*********************************************************************************
 * File: 		Set.h
 * Project: 	ECE 585 L2 Cache Model
 * Version: 	0.1
 * Date:		11/20/2014
 * Author:		Prakhya, Doguparthi, Mutyala, Kothakapu
 *********************************************************************************/

#ifndef SET_H
#define SET_H

#include "Line.h"
#include "Functions.h"

using namespace std;

/************************************
* Find whether the line is a hit' or hit or hitm
* Inputs: int - cCacheIndex, char - setOperation and int - setTag
* Outputs: None
*************************************/
void snoopOperation(int cCacheIndex, char setOperation, int setTag)
{
	int i = 0;
	int putSnoop = 0;														// Line Index
	while (i < WAYS)
	{
		if(isValid(cCacheIndex,i))										// Check is line is valid (M,E,S)
		{
			if (checkMatch(cCacheIndex,i,setTag))							// Check if tag matches
			{
				if (getState(cCacheIndex,i) == 0)
				{													// if line is hitM
					putSnoop = HITM;
					break;
				}
				putSnoop = HIT;										// if line is hit
				break;
			}
		}
		else
		{
			putSnoop = NOHIT;											// if line is no hit
		}
		i++;
	}
	PutSnoopResult(putSnoop);
	if (putSnoop == 1 || putSnoop == 2)
	{
		if (i == WAYS)
		{
			myUpdate(cCacheIndex,i-1,setTag,setOperation,putSnoop);
		}
		else
		{
			myUpdate(cCacheIndex,i,setTag,setOperation,putSnoop);
		}
	}
	else
		return;
}

/************************************
* Invalidates all lines in a set
* Inputs: int - cache index
* Outputs: None
*************************************/
void invalidateAll(int cCacheIndex)
{
	for (int i = 0; i < WAYS; i++)
	{
		invalidate(cCacheIndex,i);
	}
	return;
}

/************************************
* Prints all the valid lines in set
* Inputs: None
* Outputs: None
*************************************/
void printValid()
{
	cout << "\tIndex\t" << "\tWay\t" << "\tState\t" << "\tTag\t" << endl;
	for (int j=0; j < NUMSETS; j++)
	{
		for (int i = 0; i < WAYS; i++)
		{
			if (isValid(j,i))
			{
				print(j,i);
			}
		}
	}
	return;
}

/************************************
* Determines LRU
* Inputs: None
* Outputs: int - way
*************************************/

int determineLru()
{
	int pos = 1;
	int local = LRULOOPINDEX;
	while (local>0)
	{
		if (lruBits[pos-1] == 1)
		{
			pos = pos*2 + 1;
		}
		else
		{
			pos = pos*2;
		}
		local--;
	}
	return pos-WAYS;
}

/************************************
* Updates LRU when a line is accessed
* Inputs: int - cache index and int way
* Outputs: None
*************************************/

void accessing(int sCacheIndex, int way)
{
	int position = (WAYS-1) + way;
	int local = LRULOOPINDEX;
	while(local>0)
	{
		if (position%2 == 0)							//even
		{
			position = (position/2)-1;
			updateLruBits(sCacheIndex,position,0);
		}
		else											//odd
		{
			position = position/2;
			updateLruBits(sCacheIndex,position,1);
		}
		local--;
	}
}


void myReadOrWrite(int cCacheIndex, char setOperation, int setTag)
{
	int lineIndex = 0;
	int LRUIndex = 0;
	int invalidLine = WAYS; 												// Invalid Line Index
	totalReferences++;
	while (lineIndex < WAYS)
	{
		if( isValid(cCacheIndex,lineIndex) )									// Check if line is valid (M,E,S,F)
		{
			if ( checkMatch(cCacheIndex,lineIndex,setTag) )						// Check if tag matches
			{
				switch (setOperation)
				{
					case '0':
						if (PRINT == 1)
						{
							cout << "L1 D Cache Read request HIT in L2 Cache" << endl;
							MessageToL1Cache(2);
						}
						break;
					case '1':
						if (PRINT == 1)
						{
							cout << "L1 D Cache Write request HIT in L2 Cache" << endl;
							MessageToL1Cache(2);
						}
						break;
					case '2':
						if (PRINT == 1)
						{
							cout << "L1 I Cache Read request HIT in L2 Cache" << endl;
							MessageToL1Cache(2);
						}
						break;
					default:
						cout << "Error in read or write operation in Set.h" << endl;
						return;
				}
				cache_hit();
				accessing(cCacheIndex,lineIndex);
				myUpdate(cCacheIndex,lineIndex,setTag,setOperation,3);
				return;
			}
		}
		else
		{
			invalidLine = lineIndex;
		}
		lineIndex++;
	}
	if (invalidLine != WAYS)
	{
		if (PRINT == 1)
			cout << "Found an invalid line" << invalidLine << endl;
		myUpdate(cCacheIndex,invalidLine,setTag,setOperation,3);
		accessing(cCacheIndex,invalidLine);
	}
	else
	{
		if (PRINT == 1)
			cout << "All lines in set are valid evicting one" << endl;
		LRUIndex = determineLru();
		invalidate(cCacheIndex,LRUIndex);
		myUpdate(cCacheIndex,LRUIndex,setTag,setOperation,3);
		accessing(cCacheIndex,LRUIndex);
	}
	cache_miss();
	return;
}


void cacheBehaviour (int index, char operation, int mytag)
{
	int sWay = 0;
	int ai = 0;
	int setNum = 0;
	switch (operation)
	{
		case '0':
			myReadOrWrite(index, operation, mytag);
			read_increment();
			break;
		case '2':
			myReadOrWrite(index, operation, mytag);
			read_increment();
			break;
		case '1':
			myReadOrWrite(index, operation, mytag);
			write_increment();
			break;
		case '3':
			snoopOperation(index, operation, mytag);
			break;
		case '4':
			snoopOperation(index, operation, mytag);
			break;
		case '5':
			snoopOperation(index, operation, mytag);
			break;
		case '6':
			snoopOperation(index, operation, mytag);
			break;
		case '7':
			if (mytag % 2 == 0)
			{
				sWay = 2;
				cout << sWay << endl;
			}
			else if (mytag % 3 == 0)
			{
				sWay = 3;
				cout << sWay << endl;
			}
			else if (mytag % 5 == 0)
			{
				sWay = 5;
				cout << sWay << endl;
			}
			else if (mytag % 7 == 0)
			{
				sWay = 5;
				cout << sWay << endl;
			}
			invalidate(index, sWay);
			break;
		case 'a':
			while (ai < WAYS)
			{
				if (checkMatch(index,ai,mytag))							// Check if tag matches
				{
					cout << getState(index, ai) << endl;
				}
				ai++;
			}
			break;
		default:
			cout << "Not a valid choice, please try again"<< endl;
			break;
	}
}

#endif
