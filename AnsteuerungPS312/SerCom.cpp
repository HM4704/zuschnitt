#include "stdafx.h"
#include "SerCom.h"

CSerCom::CSerCom(void)
{
    m_hDevice = INVALID_HANDLE_VALUE;
    m_bIsPipe = false;
    m_bIsServer = false;
    memset(&m_olRecv, 0, sizeof(m_olRecv));
}

CSerCom::~CSerCom(void)
{
    DetachDevice();
}


bool CSerCom::AttachToDevice(char* szDev)
{
    //'CreateFile' liefert Handle auf Schnittstelle
	m_hDevice= CreateFile(					
						szDev,
						GENERIC_READ|GENERIC_WRITE,
						0,
						NULL,
						OPEN_EXISTING,
						FILE_FLAG_OVERLAPPED,
						NULL
						);

	if (m_hDevice == INVALID_HANDLE_VALUE)
    {
		//----- ungültigen Handle löschen
		m_hDevice = INVALID_HANDLE_VALUE;
		return(FALSE);
    }

    m_strDevName.assign(szDev);

    if (strlen(szDev) > 16)
        m_bIsPipe = true;


    /*
    HANDLE CreateEvent(
      LPSECURITY_ATTRIBUTES lpEventAttributes,
      BOOL bManualReset,
      BOOL bInitialState,
      LPCTSTR lpName
    );
    */
    m_olRecv.hEvent = CreateEvent(NULL, TRUE /* bManualReset */, FALSE /* bInitialState */, NULL);
    if (m_olRecv.hEvent == INVALID_HANDLE_VALUE)
        return false;

    return true;
}

void CSerCom::DetachDevice(void)
{
    ClearDevice();
    CloseHandle(m_hDevice);
    m_hDevice = INVALID_HANDLE_VALUE;
    CloseHandle(m_olRecv.hEvent);
}

bool CSerCom::CreatePipe(char* szPipe)       // um Weg ueber Pipe zu erzwingen
{
#define BUFSIZE 0x2000
     BOOL fConnected; 

     m_hDevice = CreateNamedPipe( 
          szPipe,                   // pipe name 
     PIPE_ACCESS_DUPLEX |     // read/write access 
         FILE_FLAG_OVERLAPPED,    // overlapped mode 
         PIPE_TYPE_BYTE |         // byte-type pipe 
         PIPE_READMODE_BYTE |     // message-read mode 
         PIPE_WAIT,               // blocking mode 
         /*INSTANCES*/3,               // number of instances 
         BUFSIZE*sizeof(TCHAR),   // output buffer size 
         BUFSIZE*sizeof(TCHAR),   // input buffer size 
         10000,                   // client time-out 
         NULL);                   // default security attributes 

      if (m_hDevice == INVALID_HANDLE_VALUE) 
      {
          return 0;
      }
 
      // Wait for the client to connect; if it succeeds, 
      // the function returns a nonzero value. If the function
      // returns zero, GetLastError returns ERROR_PIPE_CONNECTED. 
 
      fConnected = ConnectNamedPipe(m_hDevice, NULL) ? 
         TRUE : (GetLastError() == ERROR_PIPE_CONNECTED); 
 
      if (fConnected) 
      { 
          // ok
      } 
      else 
      {
        // The client could not connect, so close the pipe. 
        CloseHandle(m_hDevice); 
        m_hDevice = INVALID_HANDLE_VALUE;
        return false;
      }

    m_bIsPipe = true;
    m_bIsServer = true;
    m_strDevName.assign(szPipe);

    m_olRecv.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (m_olRecv.hEvent == INVALID_HANDLE_VALUE)
        return false;

    return true;
}

bool CSerCom::GetCommDcb(DCB *pDcb)
{
    if (m_bIsPipe == true)
        return true;

	//Handle auf Schnittstelle gültig?
	if(m_hDevice != INVALID_HANDLE_VALUE)
	{
		bool bRet;
		// Aktuelle Schnittstellenparameter auslesen
        pDcb->DCBlength = sizeof(DCB);
		bRet = GetCommState(m_hDevice, pDcb);
		
		return bRet;
	}
	else
		return FALSE;
}

bool CSerCom::SetCommDcb(DCB *pDcb)
{
    if (m_bIsPipe == true)
        return true;

	if(m_hDevice != INVALID_HANDLE_VALUE)
	{
		// Aktuelle Schnittstellenparameter setzen
		return(SetCommState(m_hDevice, pDcb));
	}
	else
		return FALSE;
}

bool CSerCom::ReconnectPipe(void)
{
    CloseHandle(m_hDevice);
    CloseHandle(m_olRecv.hEvent);
    return CreatePipe((char*)m_strDevName.c_str());
}

bool CSerCom::Send(byte* pbyData, int iCount)
{
    DWORD dwWritten;

    if (!WriteFile(m_hDevice, pbyData, iCount, &dwWritten, &m_olRecv ))
    {
        if (GetLastError() == ERROR_IO_PENDING)
        {
            switch (WaitForSingleObject(m_olRecv.hEvent, 5*1000))
            {
                case WAIT_OBJECT_0:
                {
			        if (!GetOverlappedResult(m_hDevice, &m_olRecv, &dwWritten, FALSE))
			        {
				        // Error in communication
                        return false;
			        }
                    break;
                }
                case WAIT_TIMEOUT:
                    return false;
                default:
                    return false;
            }
        }
        else
            return false;
    }
    if (iCount != dwWritten)
        return false;

    if (m_bIsPipe == true)
        Sleep(iCount*1);  //?? 9600 Baud simulieren
    return true;
}

bool CSerCom::Recv(byte* pbyToken, int iTimeout)
{
    DWORD dwRead;
    LPOVERLAPPED pOvlpd;

read_again:
    if (iTimeout == INFINITE)
    {
#if 0
        if (m_bIsPipe == true)
            pOvlpd = NULL;
        else
            // Fall: COM1 und INFINITE Timeout
#endif // 0
            pOvlpd = &m_olRecv;
    }
    else
        pOvlpd = &m_olRecv;
	if(!ReadFile( m_hDevice, pbyToken, 1, &dwRead, pOvlpd) )
    {

        if (GetLastError() == ERROR_MORE_DATA)
        {
            // es sind noch weitere Daten da
            return true;
        }
        if ((GetLastError() == ERROR_BROKEN_PIPE)  && (m_bIsPipe == true) && (m_bIsServer == true))
        {
            ReconnectPipe();
            goto read_again;
        }
		if (GetLastError() != ERROR_IO_PENDING)
		{
            return false;
		}
        switch (WaitForSingleObject(m_olRecv.hEvent, iTimeout))
        {
            case WAIT_OBJECT_0:
            {
			    if (!GetOverlappedResult(m_hDevice, &m_olRecv, &dwRead, FALSE))
			    {
				    // Error in communication
                    return false;
			    }
                if (dwRead != 1)
                {
                    return false;
                }
                break;
            }
            case WAIT_TIMEOUT:
                CancelIo(m_hDevice);
                return false;
            default:
                return false;
        }

	}// if ReadFile    

    return true;
}

// empfängt bis zu iMaxCount Daten, gibt die Daten zurück die bis zum Timeout empfangen wurden
bool CSerCom::Recv(byte* pbyToken, int* piCount, int iMaxCount, int iTimeout)
{
    DWORD dwRead;

read_again:
	if(!ReadFile( m_hDevice, pbyToken, iMaxCount, &dwRead, &m_olRecv) )
    {

		if (GetLastError() != ERROR_IO_PENDING)
		{
            if ((GetLastError() == ERROR_INVALID_HANDLE) && (m_bIsPipe == true) && (m_bIsServer == true))
            {
                ReconnectPipe();
                goto read_again;
            }
            else
                return false;
		}
        // Daten werden noch gelesen

        switch (WaitForSingleObject(m_olRecv.hEvent, iTimeout))
        {
            case WAIT_OBJECT_0:
            case WAIT_TIMEOUT:
            {
			    if (!GetOverlappedResult(m_hDevice, &m_olRecv, &dwRead, FALSE))
			    {
				    // Error in communication
                    return false;
			    }
                *piCount = (int)dwRead;
                break;
            }
            default:
                return false;
        }

	}// if ReadFile    

    return true;
}

bool CSerCom::ClearRecv(void)
{
    byte byDummy;

#if 0
    if (m_bIsPipe == true)
        return true;
#endif // 0

    while (Recv(&byDummy, 1) == true)
    {}
    return true;
}

bool CSerCom::ClearDevice(void)
{
    if (m_bIsPipe == true)
        return true;

	return ((bool)PurgeComm(m_hDevice, PURGE_RXABORT|PURGE_RXCLEAR|PURGE_TXABORT|PURGE_TXCLEAR));
}
