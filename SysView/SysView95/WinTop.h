#ifndef WINTOP_H
#define WINTOP_H
////////////////////////////////////////////////////////////////
// Interface-Beschreibung zu Wintop.vxd
////////////////////////////////////////////////////////////////

// Treiber-name
const char strWintopVxd[] = "\\\\.\\Wintop.vxd";

#define WINTOP_MAX_PT_ENTRIES   0x400

// IOCTLs
#define  IOCTL_WINTOP_GET_TICKCOUNTS         1
#define  IOCTL_WINTOP_INIT                   2
#define  IOCTL_WINTOP_GET_MEM                3


// Struktur PROCESS_TIME wird von IOCTL_WINTOP_GET_TICKCOUNTS benuetzt
typedef struct tagPROCESS_TIME
{
    DWORD       dwProcessId;
    DWORD       dwUnkown1;
    DWORD       dwTickCount;
    BYTE        byFlags[4];
} PROCESS_TIME;

typedef struct tagCPU_USAGE
{
    DWORD   dwProcessId;
    DWORD   dwCpuPercent;
    DWORD   dwTickCount;
    DWORD   dwTickCountPrev;
    DWORD   dwMemPoolId;
} CPU_USAGE;
     
typedef struct tagMEM_USAGE
{
    DWORD   dwCodeAllocated;        // *4 in [kB]
    DWORD   dwCodeInMemory;
    DWORD   dwCodeXXX;               // in use ??
    DWORD   dwDataAllocated;
    DWORD   dwDataInMemory;
    DWORD   dwDataXXX;               // in use ??
    DWORD   dwPageTablesNotPresent;
} MEM_USAGE;

BOOL WintopInit(HANDLE hDevice);
HANDLE WintopOpenDevice(void);
CPU_USAGE* WintopInitCuBuffer(DWORD dwMaxEntries);
BOOL WintopGetProcessTimes(HANDLE hDevice, CPU_USAGE* pBuf, DWORD dwMaxEntries, 
                           DWORD &dwExistEntries, BOOL bProcessChange = FALSE);
BOOL WintopGetProcessMemoryUsage(HANDLE hDevice, DWORD dwPoolId, 
                                 MEM_USAGE *pMemUsage);


#endif