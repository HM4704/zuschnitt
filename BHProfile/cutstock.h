#pragma once

#include <list>
#include <vector>
#include <map>
//#include <ProfileCutElem.h>

using namespace std;


typedef struct _CUT_LIST_ELEM
{
    int iCount;
    int iLength;
}CUT_LIST_ELEM;

typedef map <int, int, greater<int>> InputMap;
typedef InputMap::iterator MIIter;
//typedef InputMap::reverse_iterator MRIter; 
typedef list <int> ProfileCutList;
typedef pair <int, int> Int_Pair;
typedef vector <ProfileCutList*>::iterator CLIter;


typedef struct _CUTSTOCK_INPUT
{
    InputMap* pInpuMap;
    int       iSrcLength;
    int       iSrcCost;
} CUTSTOCK_INPUT;

typedef struct _CUTSTOCK_OUTPUT
{
    vector <ProfileCutList*>* pCutList;
} CUTSTOCK_OUTPUT;


void cutstock_main(CUTSTOCK_INPUT* pInput, CUTSTOCK_OUTPUT* pOutput);
