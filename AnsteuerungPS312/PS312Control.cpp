#include "stdafx.h"
#include "PS312.h"
#include "PS312Control.h"

CPS312Control::CPS312Control(void)
{
    m_byUnitAddr = 11;
}

CPS312Control::CPS312Control(byte byUnitAddr)
{
    m_byUnitAddr = byUnitAddr;
}

CPS312Control::~CPS312Control(void)
{
}

bool CPS312Control::SetUnitAddress(byte byUnitAddr)
{
    m_byUnitAddr = byUnitAddr;
    return true;
}

int CPS312Control::GetStatus(byte byLevel, int* piStatus)
{
    PARAM param;

    if ((byLevel != PS312_LEVEL_ALL) && (byLevel != PS312_LEVEL_AXIS1))
        return PS312_CTRL_INVALID_LEVEL;

    param.byLevel  = byLevel;
    param.byNumber = PS312_PARAM_STATUS;

    return ReadParam(m_byUnitAddr, param, piStatus);
}

int CPS312Control::GetParam(byte byLevel, int iParam, int* piVal)
{
    PARAM param;

#if 0  //?? wieder rein
    if ((byLevel != PS312_LEVEL_ALL) && (byLevel != PS312_LEVEL_AXIS1))
        return PS312_CTRL_INVALID_LEVEL;
#endif // 0
    if ((iParam > PS312_MAX_PARAMS) || (iParam < PS312_MIN_PARAM))
        return PS312_CTRL_INVALID_PARAM;

    param.byLevel  = byLevel;
    param.byNumber = iParam;

    return ReadParam(m_byUnitAddr, param, piVal);
}

int CPS312Control::SetParam(byte byLevel, int iParam, int iVal)
{
    PARAM param;

#if 0  //?? wieder rein
    if ((byLevel != PS312_LEVEL_ALL) && (byLevel != PS312_LEVEL_AXIS1))
        return PS312_CTRL_INVALID_LEVEL;
#endif // 0
    if ((iParam > PS312_MAX_PARAMS) || (iParam < PS312_MIN_PARAM))
        return PS312_CTRL_INVALID_PARAM;

    param.byLevel  = byLevel;
    param.byNumber = iParam;

    return SendParam(m_byUnitAddr, param, iVal);
}

int CPS312Control::SetCmd(byte byLevel, int iCmd)
{
    PARAM param;

    if ((byLevel != PS312_LEVEL_ALL) && (byLevel != PS312_LEVEL_AXIS1))
        return PS312_CTRL_INVALID_LEVEL;
    if (iCmd < PS312_CMD_MIN || iCmd > PS312_CMD_MAX)
        return PS312_CTRL_INVALID_CMD;

    param.byLevel  = byLevel;
    param.byNumber = PS312_PARAM_CMD;

    return SendParam(m_byUnitAddr, param, iCmd);
}

char* CPS312Control::GetErrorString(int iError)
{
    static _TCHAR* s_ErrorStrings[] = {
        { _T("Ungültiger Parameter!") },
        { _T("Ungültiger Befehl!") },
        { _T("Ungültige Ebene!") },
        { _T("Unbekannter Fehlercode!") }
    };

    if (iError < PS312_CTRL_ERR_CLASS)
    {
        return CPS312Protocol::GetErrorString(iError);
    }

//    if ((iError - PS312_CTRL_ERR_CLASS) > 
    return s_ErrorStrings[iError - PS312_CTRL_ERR_CLASS];
}

int CPS312Control::LoadProgram(int iProgNr, PS312_DATA_SET* pData, int iCount)
{
    int iRet, i, iRetSave = PS312_CTRL_NO_ERR;

    // Setzen der Programm Nr in P62 (Ebene All)
    iRet = SetParam(PS312_LEVEL_ALL, PS312_PARAM_ACTIVE_PROG_NR, iProgNr);
    if (iRet != PS312_PROTO_NO_ERR)
        return iRet;

    // Aktivieren des Programm Downloads
    iRet = SetCmd(PS312_LEVEL_ALL, PS312_CMD_START_PROG_LOAD);
    if (iRet != PS312_PROTO_NO_ERR)
        return iRet;

    for (i = 0; i < iCount; i++)
    {
        // Speichern des Datensatzes als Absolut(erster)- oder Kettenmass (fest)
        if (pData[i].mass == MASS_ABSOLUT)
            iRet = SetCmd(PS312_LEVEL_ALL, PS312_CMD_SET_ABS_MASS);
        else
            iRet = SetCmd(PS312_LEVEL_ALL, PS312_CMD_SET_KETT_MASS);
        if (iRet != PS312_PROTO_NO_ERR)
        {
            iRetSave = iRet;
            goto stop_prog_load;
        }
        
        // Senden des Sollwerts
        iRet = SetParam(PS312_LEVEL_SOLLW, 0x00, pData[i].iLength*10 /* in 1/10 mm */);
        if (iRet != PS312_PROTO_NO_ERR)
        {
            iRetSave = iRet;
            goto stop_prog_load;
        }

        // Senden der Stückzahl
        iRet = SetParam(PS312_LEVEL_COUNT, 0x00, pData[i].iCount);
        if (iRet != PS312_PROTO_NO_ERR)
        {
            iRetSave = iRet;
            goto stop_prog_load;
        }

        //?? Gerät entlasten
        if (i%10 == 0)
            Sleep(10);
    }

stop_prog_load:
    // Programm mit 0,0 beenden
    // Senden des Sollwerts 0
    iRet = SetParam(PS312_LEVEL_SOLLW, 0x00, 0);
//?? auf alle Fälle weitermachen    if (iRet != PS312_PROTO_NO_ERR)
//??        return iRet;

    // Senden der Stückzahl 0
    iRet = SetParam(PS312_LEVEL_COUNT, 0x00, 0);
//??    if (iRet != PS312_PROTO_NO_ERR)
//??        return iRet;

    // Rückfahranweisung, Parkposition
    iRet = SetCmd(PS312_LEVEL_ALL, PS312_CMD_SET_ABS_MASS);
    iRet = SetParam(PS312_LEVEL_SOLLW, 0x00, 60000);
//?? auf alle Fälle weitermachen    if (iRet != PS312_PROTO_NO_ERR)
//??        return iRet;

    // Senden der Stückzahl 0
    iRet = SetParam(PS312_LEVEL_COUNT, 0x00, 1);
//??    if (iRet != PS312_PROTO_NO_ERR)
//??        return iRet;

    // Beenden des Programm Downloads
    iRet = SetCmd(PS312_LEVEL_ALL, PS312_CMD_STOP_PROG_LOAD);
    if (iRet == PS312_PROTO_NO_ERR)
    {
    }
    else
    {
        if (iRetSave == PS312_CTRL_NO_ERR)
        {
            // beim Programmieren ist kein Fehler aufgetreten, diesen Fehler melden
            return iRet;
        }
    }

    return iRetSave;
}

int CPS312Control::ReadProgram(int iProgNr, PS312_DATA_SET* pData, int iMaxCount, int* piCount)
{
    int iRet, i, iRetSave = PS312_CTRL_NO_ERR, iVal;

    *piCount = 0;

    // Setzen der Programm Nr in P62 (Ebene All)
    iRet = SetParam(PS312_LEVEL_ALL, PS312_PARAM_ACTIVE_PROG_NR, iProgNr);
    if (iRet != PS312_PROTO_NO_ERR)
        return iRet;

    // Aktivieren des Programm Uploads
    iRet = SetCmd(PS312_LEVEL_ALL, PS312_CMD_START_PROG_LOAD);
    if (iRet != PS312_PROTO_NO_ERR)
        return iRet;

    i = 0;
    while (i < iMaxCount)
    {
        // Anfordern des Sollwertstatus (Absolut-, Kettenmass)
        iRet = GetParam(PS312_LEVEL_ALL, PS312_PARAM_MASS_STATUS, &iVal);
        if (iRet != PS312_PROTO_NO_ERR)
        {
            iRetSave = iRet;
            goto stop_prog_load;
        }
        if (iVal == 0)
        {
            // Absolutmass
            pData[i].mass = MASS_ABSOLUT;
        }
        else
        {
            pData[i].mass = MASS_KETTE;
        }
        
        // Anfordern des Sollwerts
        iRet = GetParam(PS312_LEVEL_SOLLW, 0x00, &pData[i].iLength);
        if (iRet != PS312_PROTO_NO_ERR)
        {
            iRetSave = iRet;
            goto stop_prog_load;
        }

        // Anfordern der Stückzahl
        iRet = GetParam(PS312_LEVEL_COUNT, 0x00, &pData[i].iCount);
        if (iRet != PS312_PROTO_NO_ERR)
        {
            iRetSave = iRet;
            goto stop_prog_load;
        }

        if ((pData[i].iCount == 0) && (pData[i].iLength == 0))
            break;
        //?? Gerät entlasten
        if (i%10 == 0)
            Sleep(10);
        i++;
    }

stop_prog_load:
    // Beenden des Programm Downloads
    iRet = SetCmd(PS312_LEVEL_ALL, PS312_CMD_STOP_PROG_LOAD);
    if (iRet == PS312_PROTO_NO_ERR)
    {
    }
    else
    {
        if (iRetSave == PS312_CTRL_NO_ERR)
        {
            // beim Programmieren ist kein Fehler aufgetreten, diesen Fehler melden
            return iRet;
        }
    }
   
    if (iRetSave != PS312_CTRL_NO_ERR)
        return iRetSave;

    *piCount = i;
    if (*piCount == iMaxCount)
        return PS312_CTRL_MORE_DATA_SETS;

    return PS312_CTRL_NO_ERR;
}

int CPS312Control::StartProgram(int iProgNr)
{
    int iRet;

    // Setzen der Programm Nr in P62 (Ebene All)
    iRet = SetParam(PS312_LEVEL_ALL, PS312_PARAM_ACTIVE_PROG_NR, iProgNr);
    if (iRet != PS312_PROTO_NO_ERR)
        return iRet;

    // Aktivieren des Programms
    iRet = SetCmd(PS312_LEVEL_ALL, PS312_CMD_START_PROG);
    if (iRet != PS312_PROTO_NO_ERR)
        return iRet;
   
    return PS312_CTRL_NO_ERR;
}

int CPS312Control::GetDataSetsPerProg(int* piCount)
{
    int iRet;

    *piCount = 0;
    // Anzahl Programme in Paramter P60 auslesen und durch Gesamtanzahl der Datensätze teilen
    iRet = GetParam(PS312_LEVEL_ALL, PS312_PARAM_COUNT_PROGS, piCount);
    if (iRet != PS312_PROTO_NO_ERR)
        return iRet;

    if (*piCount > 0)
        *piCount = PS312_DATA_SET_COUNT/(*piCount);

    if (*piCount > PS312_DATA_SET_COUNT_PER_PROG)
        *piCount = PS312_DATA_SET_COUNT_PER_PROG;

    if (*piCount > 0)
        *piCount -= 1;  // letzten Eintrag mit 0,0 für Endekennung abziehen

    if (*piCount > 0)
        *piCount -= 1;  // Anfangseintrag mit ABSOLUT_MASS

    if (*piCount > 0)
        *piCount -= 1;  // Lezter Datebnsatz enthält Rückfahranweisung

    return PS312_CTRL_NO_ERR;
}
