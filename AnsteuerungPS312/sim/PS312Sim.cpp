// PS312Sim.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"
#include <process.h>
#include "PS312Simul.h"

CSerCom com;
CPS312Sim sim;

void PS312SimThread(void* pDummy);
bool bLocal;

int _tmain(int argc, _TCHAR* argv[])
{
    DCB dcb;
    char szName[MAX_PATH];

    strncpy(szName, PS312_SIMUL_PIPE, sizeof(szName));

    bLocal = true;
    if (argc > 1)
    {
        if (strncmp((char*)argv[1], "-r", 2) == 0)
        {
            // ueber com gehen
            strncpy(szName, "COM1", sizeof(szName));
            bLocal = false;
        }
    }

    printf("PS312Sim started\n");
    printf("Connect to %s\n", szName);
    sim.SetCom(&com);
    if (bLocal == true)
        com.CreatePipe(szName);
    else
        com.AttachToDevice(szName);

    com.GetCommDcb(&dcb);
    // Einstellungen: 7 Bit, 9600 B, 1 Stop, 1 Start, 1 Parity(even), kein Handshake
    dcb.BaudRate   = CBR_9600;
    dcb.fParity    = TRUE;
    dcb.ByteSize   = 7;
    dcb.Parity     = EVENPARITY;
    dcb.StopBits   = ONESTOPBIT;
    dcb.fOutxCtsFlow = FALSE;
    dcb.fOutxDsrFlow = FALSE;
    dcb.fDtrControl  = DTR_CONTROL_DISABLE;
    dcb.fDsrSensitivity  = FALSE;
    dcb.fOutX            = FALSE;
    dcb.fInX             = FALSE;
    dcb.fRtsControl      = RTS_CONTROL_DISABLE;
    com.SetCommDcb(&dcb);
    printf("Connect done\n");

    _beginthread(PS312SimThread, 0x2000, NULL);

    Sleep(INFINITE);

	return 0;
}

void PS312SimThread(void* pDummy)
{
    sim.Run();
}