/*********************************************************************************
 * File: 		Defines.h
 * Project: 	ECE 585 L2 Cache Model
 * Version: 	0.1
 * Date:		11/20/2014
 * Author:		Prakhya, Doguparthi, Mutyala, Kothakapu
 *********************************************************************************/

#ifndef DEFINES_H
#define DEFINES_H

#include <vector>
#include <string>
#include <iterator>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <math.h>

#define M 0
#define E 1
#define S 2
#define I 3
#define F 4
#define B 8												// Define B as 8 bits
#define K 1024											// Define K as 1024
#define M1 K*1024										// Define M as 1024
#define ADDRESSBITS 32
#define BYTESPERLINE 64								    // Define the number of bytes for one line
#define WAYS 16											// Define the associativity of the cache (number of ways)
#define NOHIT 0
#define HIT 1
#define HITM 2

#define SILENTDEFINES 1
#define SILENTFUNCTIONS 1
#define PRINT 0
#define READ 1
#define WRITE 2
#define INVALIDATE 3
#define RWIM 4

const long BYTESINCACHE = 8*1024*1024;
const long SETSIZE = WAYS*BYTESPERLINE;
const long NUMSETS = BYTESINCACHE/SETSIZE;
const long BYTEOFFSET = ceil(log(BYTESPERLINE)/log(2));				// Define the number of bits used for byte offset in line
const long INDEXBITS = ceil(log(NUMSETS)/log(2));					// Define the number of bits used for indexing
const long TAGBITS  = (ADDRESSBITS-(INDEXBITS+BYTEOFFSET));			// Defines the number of tag bits

extern float hits;
extern float misses;
extern unsigned int reads;
extern unsigned int writes;
extern unsigned int totalReferences;

using namespace std;

int cacheIndex,setIndex,lineIndex;
extern int lruBits[WAYS-1];
int LRULOOPINDEX = ceil(log(WAYS)/log(2));

void updateState(int cacheIndex, int setIndex, int setLineState);
int getState(int cacheIndex, int setIndex);
void updateTag(int cacheIndex, int setIndex, int setLineTag);
int getTag(int cacheIndex, int setIndex);
void getLruBits(int cacheIndex);
void updateLruBits(int sCacheIndex,int lruBit,int value);

vector<int> Line(2);
vector< vector<int> > Set(WAYS, Line);
vector< vector< vector<int> > > Cache(NUMSETS, Set);
vector<int> vLruBits(WAYS-1);
vector< vector<int> > Lru(NUMSETS, vLruBits);

void updateState(int cacheIndex, int setIndex, int setLineState)
{
		Cache[cacheIndex][setIndex][0] = setLineState;
		if (SILENTDEFINES == 0)
			cout << "State is updated to: " << setLineState << endl;
}

int getState(int cacheIndex, int setIndex)
{
	if (SILENTDEFINES == 0)
		cout << "Present state is: " << Cache[cacheIndex][setIndex][0] << endl;
	return Cache[cacheIndex][setIndex][0];
}

void updateTag(int cacheIndex, int setIndex, int setLineTag)
{
	Cache[cacheIndex][setIndex][1] = setLineTag;
	if (SILENTDEFINES == 0)
		cout << "Tag is updated to: " << setLineTag << endl;
}

int getTag(int cacheIndex, int setIndex)
{
	if (SILENTDEFINES == 0)
		cout << "Present tag is: " << Cache[cacheIndex][setIndex][1] << endl;
	return Cache[cacheIndex][setIndex][1];
}

void getLruBits(int cacheIndex)
{
	for (int lruIndex = 0; lruIndex<WAYS-1; lruIndex++)
	{
		lruBits[lruIndex] = Lru[cacheIndex][lruIndex];
	}
}

void updateLruBits(int cacheIndex,int lruBit,int value)
{
	Lru[cacheIndex][lruBit] = value;
	getLruBits(cacheIndex);
	return;
}

void init()
{
	for (int i=0;i<NUMSETS;i++)
	{
		for (int j=0;j<WAYS;j++)
		{
			updateState(i,j,I);
			updateTag(i,j,100000);
		}
		for (int k=0;k<WAYS-1;k++)
			updateLruBits(i,k,1);
	}
}

#endif
