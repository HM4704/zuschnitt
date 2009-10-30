#pragma once

//////////////////////////////////////////////////////////////////////
// Klasse für die Ansteuerung der seriellen Schnittstelle oder einer Pipe


#pragma warning(disable:4786)
#include <string>

using namespace std;

typedef unsigned char byte;


//////////////////////////////////////////////////
//	Strukturen
//


class CSerCom
{
    HANDLE		m_hDevice;		    	// Handle auf das Schnittstellendevice (COM-Schnittstelle oder Pipe)
    string      m_strDevName;           // Name der Schnittstelle
    bool        m_bIsPipe;              // 
    bool        m_bIsServer;            // Server bei Pipe
    OVERLAPPED  m_olRecv;               // fuer Empfang

    bool ReconnectPipe(void);
public:
    CSerCom(void);
public:
    ~CSerCom(void);

    bool AttachToDevice(char* szDev);
    bool CreatePipe(char* szPipe);       // um Weg ueber Pipe zu erzwingen
    bool GetCommDcb(DCB *pDcb);
	bool SetCommDcb(DCB *pDcb);

    virtual bool Send(byte* pbyData, int iCount);
    virtual bool Recv(byte* pbyToken, int iTimeout);
    bool Recv(byte* pbyToken, int* piCount, int iMaxCount, int iTimeout);
    bool ClearDevice(void);
    bool ClearRecv(void);
    void DetachDevice(void);
};
