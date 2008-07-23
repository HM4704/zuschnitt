#pragma once
#include "ps312protocoll.h"
#include "ps312.h"

///////////////////////////////////////
// Fehlercodes
#define PS312_CTRL_ERR_CLASS        0x100
#define PS312_CTRL_NO_ERR           0x00
#define PS312_CTRL_INVALID_PARAM    (PS312_CTRL_ERR_CLASS | 0x00)
#define PS312_CTRL_INVALID_CMD      (PS312_CTRL_ERR_CLASS | 0x01)
#define PS312_CTRL_INVALID_LEVEL    (PS312_CTRL_ERR_CLASS | 0x02)
#define PS312_CTRL_MORE_DATA_SETS   (PS312_CTRL_ERR_CLASS | 0x03)       // es sind mehr Datensätze vorhanden


class CPS312Control :
    public CPS312Protocol
{
    byte m_byUnitAddr;
public:
    CPS312Control(void);
    CPS312Control(byte byUnitAddr);
public:
    ~CPS312Control(void);
    
    virtual char* GetErrorString(int iError);

    bool SetUnitAddress(byte byUnitAddr);

    int GetStatus(byte byLevel, int* piStatus);
    int GetParam(byte byLevel, int iParam, int* piVal);
    int SetParam(byte byLevel, int iParam, int iVal);
    int SetCmd(byte byLevel, int iCmd);
    int LoadProgram(int iProgNr, PS312_DATA_SET* pJobs, int iCount);
    int ReadProgram(int iProgNr, PS312_DATA_SET* pJobs, int iMaxCount, int* piCount);
    int StartProgram(int iProgNr);
    int GetDataSetsPerProg(int* piCount);       // liefert maximale Anzahl an Datensätzen pro Programm

//    char* GetStatus(byte byLevel);
};
