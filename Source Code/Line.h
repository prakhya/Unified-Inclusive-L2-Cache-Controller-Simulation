/*********************************************************************************
 * File: 		Line.h
 * Project: 	ECE 585 L2 Cache Model
 * Version: 	0.1
 * Date:		11/20/2014
 * Author:		Prakhya, Doguparthi, Mutyala, Kothakapu
 *********************************************************************************/

#ifndef LINE_H
#define LINE_H

#include "Defines.h"
#include "Functions.h"

using namespace std;

/************************************
* Calls get state and returns true if line is valid
* Inputs: int - Cache index and int - set index
* Outputs: bool - false if invalid, otherwise true
*************************************/
bool isValid(int lCacheIndex, int lSetIndex)
{
	if (getState(lCacheIndex,lSetIndex) == 3)
	{
		return false;
	}
	else
	{
		return true;
	}
}

/************************************
* Checks if the tag of the selected line matches the input tag
* Inputs: int tag_in - tag of address coming in
* Outputs: bool - True: Tag Match, False: Tag does not match
*************************************/
bool checkMatch( int lCacheIndex, int lSetIndex, int tag_in)
{
	int linetag = getTag(lCacheIndex, lSetIndex);
	if (linetag == tag_in)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/************************************
* Prints the INDEX, WAY, MESIF STATE and Tag of the line
* Inputs: int Cache index and int - set index
* Outputs: None
*************************************/
void print(int lCacheIndex, int lSetIndex)
{
	char printState;
	if (getState(lCacheIndex,lSetIndex) == 0)
		printState = 'M';
	if (getState(lCacheIndex,lSetIndex) == 1)
		printState = 'E';
	if (getState(lCacheIndex,lSetIndex) == 2)
		printState = 'S';
	if (getState(lCacheIndex,lSetIndex) == 3)
		printState = 'I';
	if (getState(lCacheIndex,lSetIndex) == 4)
		printState = 'F';
	cout << "\t" << lCacheIndex << "\t\t" << lSetIndex  << "\t\t" << printState << "\t\t" << getTag(lCacheIndex,lSetIndex) << endl;
}

/************************************
* Sets the MESIF state to Invalid does write back if present state is M
* Inputs: int Cache index and int - set index
* Outputs: None
*************************************/
void invalidate(int lCacheIndex, int lSetIndex)
{
	if (getState(lCacheIndex,lSetIndex) == 0)
	{
		BusOperation (2);
		MessageToL1Cache (3);
	}
	updateState(lCacheIndex,lSetIndex,3);
}

/***************************************************************************************************
* Updates the state based on a read or write request from the L1 cache and snoop result
* Inputs: int Cache index, int - set index and char - mesifOperation
* Outputs: None
 **************************************************************************************************/
void update_proc (int lCacheIndex, int lSetIndex, char mesifOperation)
{
	int snoop_result = getSnoopResult();
	switch(getState(lCacheIndex,lSetIndex))
	{
	case 3:
		if ((mesifOperation == '0') || (mesifOperation == '2'))
		{
			if (snoop_result == 1 || snoop_result == 2)
			{
				BusOperation (1);
				updateState(lCacheIndex,lSetIndex,4);
			}
			else if (snoop_result == 0)
			{
				BusOperation (1);
				updateState(lCacheIndex,lSetIndex,1);
			}
		}
		else if (mesifOperation == '1')
		{
			BusOperation (4);
			updateState(lCacheIndex,lSetIndex,0);
		}
		break;
	case 4:
		if (mesifOperation == '1')
		{
			BusOperation (2);
			updateState(lCacheIndex,lSetIndex,0);
		}
		else if ((mesifOperation == '0') || (mesifOperation == '2'))
		{
			updateState(lCacheIndex,lSetIndex,4);
		}
		break;
	case 2:
		if (mesifOperation == '1')
		{
			BusOperation (3);
			updateState(lCacheIndex,lSetIndex,0);
		}
		else if ((mesifOperation == '0') || (mesifOperation == '2'))
		{
			updateState(lCacheIndex,lSetIndex,2);
		}
		break;
	case 1:
		if (mesifOperation == '1')
		{
			updateState(lCacheIndex,lSetIndex,0);
		}
		else if ((mesifOperation == '0') || (mesifOperation == '2'))
		{
			updateState(lCacheIndex,lSetIndex,1);
		}
		break;
	case 0:
		if ((mesifOperation == '1') || (mesifOperation == '0') || (mesifOperation == '2'))
		{
			updateState(lCacheIndex,lSetIndex,0);
		}
		break;
	default:
		break;
	}
	return;
}

/***************************************************************************************************
* Updates the state based on a snooped operation on the FSB
* Inputs: int Cache index, int - set index, char - mesifOperation, int - mesifPutSnoop
* Outputs: None
 **************************************************************************************************/
void updateSnoop (int lCacheIndex, int lSetIndex, char mesifOperation, int mesifPutSnoop)
{
	switch (getState(lCacheIndex,lSetIndex))
	{
	case 3:
		updateState(lCacheIndex,lSetIndex,3);
		break;
	case 4:
		if ((mesifOperation == '6') & (mesifPutSnoop == 1))
		{
			invalidate(lCacheIndex,lSetIndex);
		}
		else if ((mesifOperation == '4') & (mesifPutSnoop == 1))
		{
			BusOperation (2);
			updateState(lCacheIndex,lSetIndex,2);
		}
		break;
	case 2:
		if (mesifOperation == '6')
		{
			invalidate(lCacheIndex,lSetIndex);
		}
		else if (mesifOperation == '4')
		{
			updateState(lCacheIndex,lSetIndex,2);
		}
		break;
	case 1:
		if ((mesifOperation == '4') & (mesifPutSnoop == 1))
		{
			BusOperation (2);
			updateState(lCacheIndex,lSetIndex,2);
		}
		else if ((mesifOperation == '6') & (mesifPutSnoop == 1))
		{
			invalidate(lCacheIndex,lSetIndex);
		}
		break;
	case 0:
		if ((mesifOperation == '4') & (mesifPutSnoop == 2))
		{
			BusOperation (2);
			updateState(lCacheIndex,lSetIndex,2);
		}
		else if ((mesifOperation == '6') & (mesifPutSnoop == 2))
		{
			invalidate(lCacheIndex,lSetIndex);
		}
		break;
	default:
		break;
	}
	return;
}

/***************************************************************************************************
 * Check which function to call based on operation
 * Inputs: int Cache index, int - set index, int - lineTag, char - lineOperation, int - linePutSnoop
 * Outputs:	None
 **************************************************************************************************/
 
void myUpdate(int lCacheIndex, int lSetIndex, int lineTag, char lineOperation, int linePutSnoop)
{
	updateTag(lCacheIndex,lSetIndex,lineTag);
	if(lineOperation == '0' || lineOperation == '1' || lineOperation == '2')
	{
		update_proc(lCacheIndex,lSetIndex,lineOperation);
	}
	else if(lineOperation == '3' || lineOperation == '4' || lineOperation == '5' || lineOperation == '6')
	{
		if (lineOperation == '4' || lineOperation == '6')
		{
			updateSnoop(lCacheIndex,lSetIndex,lineOperation,linePutSnoop);
		}
		else if (lineOperation == '3' || lineOperation == '5')
		{
			invalidate(lCacheIndex,lSetIndex);
		}
	}
}

#endif

