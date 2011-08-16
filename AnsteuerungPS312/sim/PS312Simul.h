#pragma once
#include "..\ps312.h"
#include "..\ps312protocoll.h"
#include "ps312param.h"
#include <iostream>
#include <fstream>
#include <cassert>

using namespace std;

#define PS312_SIMUL_PIPE TEXT("\\\\.\\pipe\\PS312SimulPipeV10")


typedef struct _PS312_PROGRAM
{
    bool  bValid;
    PS312_DATA_SET* pFirstDataSet;
    PS312_DATA_SET* pLastDataSet;
} PS312_PROGRAM;

class CPS312Sim :
    public CPS312Protocol
{
    byte m_byUnitAddr;
    CPS312Param*    m_param[PS312_NUM_LEVEL][PS312_MAX_PARAMS];

    PS312_DATA_SET  m_DataSets[PS312_DATA_SET_COUNT];
    PS312_PROGRAM   m_Programs[PS312_MAX_PROGRAMS];

    ofstream        m_file;    // Datei mit Profilen, die beim Start Prog beschrieben wird

public:
    CPS312Sim(void);
public:
    ~CPS312Sim(void);

protected:
    void WaitValidFrame(PARAM* para, byte* byMsg, int iMaxLen, int* iMsgLen, int iTimeout);
    void HandleSendParam(PARAM para, byte* pbyMsg, int iLen);
    void HandleReadParam(PARAM para, READ_MSG* pReadMsg);
    void SendReadAnsw(PARAM para, READ_MSG* pReadMsg, int iVal);
    void InitParams(void);
    void InitPrograms(void);

    void HandleCmd(PARAM para, SEND_MSG* pMsg, int iLen);
    void DoProgCmd(void);
    void DoStartProgCmd(void);

public:
    void Run(void);
};
