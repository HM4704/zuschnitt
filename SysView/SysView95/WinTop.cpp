
#include "stdafx.h"
#include "Datatype.h"
#include "WinTop.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static DWORD   g_dwProcessId;
static PROCESS_TIME g_ptArrTimes[WINTOP_MAX_PT_ENTRIES];
static DWORD g_dwTickCount = 0;


HANDLE WintopOpenDevice(void)
{
    return CreateFile(strWintopVxd, 0, 0, NULL, 0, 
        0x4000000 /*???*/, NULL);
}


BOOL WintopInit(HANDLE hDevice)
{
    DWORD dwBytesRet;

    g_dwProcessId = 0;
    g_dwTickCount = GetTickCount();

    // Anmeldung bei Treiber
    // Treiber gibt Prozess-Id zurück, mit der dann XOR verschluesselt wird
    if (DeviceIoControl(hDevice, IOCTL_WINTOP_INIT, NULL, 0, &g_dwProcessId, 4,
        &dwBytesRet, NULL) == FALSE)
    {
        return FALSE;
    }

    DWORD dwCurrentProcess = GetCurrentProcessId();
    g_dwProcessId ^= dwCurrentProcess;

    return TRUE;
}


BOOL WintopGetProcessTimes(HANDLE hDevice, CPU_USAGE* pBuf, DWORD dwMaxEntries, 
                           DWORD &dwExistEntries, BOOL bProcessChange)
{
    dwExistEntries; // to avoid warning

    static BOOL s_bFirst = TRUE;
    static DWORD s_dwEntries = 0;

    DWORD dwBytesRet;
    PROCESS_TIME *pPtEnd, *pPtAct;
    BOOL bFound = FALSE;

    DWORD dwDeltaTC = g_dwTickCount;
    g_dwTickCount = GetTickCount();
    dwDeltaTC = g_dwTickCount - dwDeltaTC;

    if (DeviceIoControl(hDevice, IOCTL_WINTOP_GET_TICKCOUNTS, NULL, 0, 
        g_ptArrTimes, sizeof(g_ptArrTimes), &dwBytesRet, NULL) == FALSE)
    {
        return FALSE;
    }    	

    if (bProcessChange)
    {
        // reset: neu aufbauen
        memset(pBuf, 0, sizeof(CPU_USAGE)*dwMaxEntries);
        s_dwEntries = 0;
        s_bFirst = TRUE;
    }

    pPtAct = g_ptArrTimes;
    pPtEnd = pPtAct+dwBytesRet;

    while (pPtAct < pPtEnd)
    {
        if ((pPtAct->dwProcessId>>16))
        {
            // größer 0xffff, also Prozess
            if (!(pPtAct->dwProcessId&0x70000000))
                pPtAct->dwProcessId ^= g_dwProcessId;

            if ((pPtAct->dwUnkown1 == 0)/* || (pPtAct->byFlags[0] & 2)
                ** Idle-Thread ist im Kernel enthalten*/)
            {
                // hier wird Pool-Id eingetragen
                bFound = FALSE;
                for (DWORD i=0; i<dwMaxEntries; i++)
                {
                    if (pBuf[i].dwProcessId == pPtAct->dwProcessId)
                    {
//                        pBuf[i].dwMemPoolId = *((DWORD*)pPtAct->byFlags);
                        bFound = TRUE;
                        break;
                    }
                }
                if (bFound == FALSE)
                {
                    s_dwEntries++;

                    for (DWORD i=0; i<dwMaxEntries; i++)
                    {
                        if (pBuf[i].dwProcessId == 0)
                        {
                            pBuf[i].dwProcessId = pPtAct->dwProcessId;
                            pBuf[i].dwMemPoolId = *((DWORD*)pPtAct->byFlags);
                            break;
                        }
                    }
                }

                // warum???
                // vielleicht diesen Prozess nicht weiter verarbeiten
                pPtAct->dwProcessId = 0;
            }
            else
            {
                bFound = FALSE;
                // Zeiten eintragen
                for (DWORD i=0; i<dwMaxEntries; i++)
                {
                    if (pBuf[i].dwProcessId == pPtAct->dwProcessId)
                    {
                        pBuf[i].dwTickCount += pPtAct->dwTickCount;
                        bFound = TRUE;
                        break;
                    }
                }
            }
        }
        else
        {
            // kleiner, also Hwnd
            if (pPtAct->dwProcessId)
            {
                HWND hWnd;
                if ((hWnd = GetWindow((HWND)pPtAct->dwProcessId, GW_CHILD)) == NULL)
                {
                    hWnd = (HWND)pPtAct->dwProcessId;
                }
                else
                {
                }
                ::GetWindowThreadProcessId(hWnd, &(pPtAct->dwProcessId));
                bFound = FALSE;
                // Zeiten eintragen
                for (DWORD i=0; i<dwMaxEntries; i++)
                {
                    if (pBuf[i].dwProcessId == pPtAct->dwProcessId)
                    {
                        pBuf[i].dwTickCount += pPtAct->dwTickCount;
                        bFound = TRUE;
                        break;
                    }
                }
            }
        }
        pPtAct++;
    }

    if (s_bFirst)
    {
        for (DWORD i=0; i<s_dwEntries; i++)
        {
            pBuf[i].dwTickCountPrev = pBuf[i].dwTickCount;
            pBuf[i].dwTickCount = 0;
        }
        s_bFirst = FALSE;
    }
    else
    {
        DWORD dwDeltaTickCount;
        for (DWORD i=0; i<s_dwEntries; i++)
        {
            dwDeltaTickCount = pBuf[i].dwTickCount - pBuf[i].dwTickCountPrev;
            pBuf[i].dwTickCountPrev = pBuf[i].dwTickCount;
            pBuf[i].dwTickCount = 0;
            pBuf[i].dwCpuPercent = (DWORD)(rnd((double)(dwDeltaTickCount*100)/(double)dwDeltaTC));
        }
    }

    return TRUE;
}


CPU_USAGE* WintopInitCuBuffer(DWORD dwMaxEntries)
{
    CPU_USAGE *pCuBuf = new CPU_USAGE[dwMaxEntries];
    if (pCuBuf)
    {
        memset(pCuBuf, 0, sizeof(CPU_USAGE)*dwMaxEntries);
    }

    return pCuBuf;
}

BOOL WintopGetProcessMemoryUsage(HANDLE hDevice, DWORD dwPoolId, MEM_USAGE *pMemUsage)
{
    DWORD dwBytesRet;

    if (DeviceIoControl(hDevice, IOCTL_WINTOP_GET_MEM, &dwPoolId, sizeof(dwPoolId), 
        pMemUsage, sizeof(*pMemUsage), &dwBytesRet, NULL) == FALSE)
    {
        return FALSE;
    }    	
    if (!dwBytesRet)
        return FALSE;


    return TRUE;
}