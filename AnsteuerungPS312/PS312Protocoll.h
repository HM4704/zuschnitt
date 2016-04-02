#pragma once

#include "sercom.h"

////////////////////////////////////////
// Implementierung des PS312 Protokolls


///////////////////////////////////////
// Datentypen
typedef unsigned char  byte;


////////////////////////////////////////
// Msg defines

#define CC_EOT      0x04        // Control Character End of Transmit
#define CC_STX      0x02        // Start of Text
#define CC_ETX      0x03        // End Of Text
#define CC_ENQ      0x05        // Enquiry
#define CC_NAK      0x15        // Not acknowledge
#define CC_ACK      0x06        // Acknowledge

#define MAX_MSG_LEN             30
#define PS312_MAX_VAL_LEN       (MAX_MSG_LEN - sizeof(MSG_ADDR) - 2 /* ETX + BCC */)



///////////////////////////////////////
// Fehlercodes
#define PS312_PROTO_NO_ERR         0x00
#define PS312_PROTO_TIMEOUT        0x01
#define PS312_PROTO_CHKSUM_ERR     0x02     // BCC Fehler
#define PS312_PROTO_TOO_LARGE      0x03     // zu viele Daten übergeben
#define PS312_PROTO_NO_COM         0x04     // kein Com Objekt
#define PS312_PROTO_FRAME_ERR      0x05     // Fehler im Frame Aufbau
#define PS312_PROTO_SEND_ERR       0x06     // Fehler beim Senden
#define PS312_PROTO_NAK_ERR        0x07     // Gerät hat mit NAK geantwortet
#define PS312_PROTO_INVALID_PARAM  0x08     // ungültiger oder unbekannter Parameter


///////////////////////////////////////
// Msg Strukturen
typedef struct _MSG_ADDR
{
    byte eot;
    byte ad1;
    byte ad2;
    byte stx;
    byte c1;
    byte c2;
    byte c3;
    byte c4;
} MSG_ADDR;

typedef struct _READ_MSG
{
    MSG_ADDR a;
    byte enq;
} READ_MSG;

typedef struct _READ_ANSW_MSG
{
    byte stx;
    byte c1;
    byte c2;
    byte c3;
    byte c4;
    /* ???xxx */
    union
    {
        byte val[1];
        byte eot;      // fuer ablehnende Antwort
    };
    byte etx;
//?? wird angehängt    byte bcc;
} READ_ANSW_MSG;

typedef struct _SEND_MSG
{
    MSG_ADDR a;
    /* ??? xxx */
    byte val[1];
    byte etx;
//?? wird angehängt    byte bcc;
} SEND_MSG;

#define PAYLOAD_SEND_MSG(iMsgLen)                   \
    (iMsgLen - sizeof(MSG_ADDR) - sizeof(byte))

// Parameter Code
typedef union _PARAM
{
    struct
    {
        byte byLevel;           // Ebene
        byte byNumber;
    };
    unsigned short param;
} PARAM;


class CPS312Protocol
{
    int        m_iRecvTimeout;
protected:
    CSerCom*   m_pCom;

public:

    CPS312Protocol(void);
public:
    ~CPS312Protocol(void);

    void SetCom(CSerCom* pCom)  { m_pCom = pCom; };
    void SetTimeout(int iTimeout) { m_iRecvTimeout = iTimeout; };

    virtual char* GetErrorString(int iError);

protected:
    int ReadParam(byte byUnitAddr, PARAM param, int* iParam);
    int SendParam(byte byUnitAddr, PARAM param, int iParam);
    int  SendData(byte* bpyData, int iCount);
    int  RecvData(byte* pbyData, int* piCount, int iMaxCount, int iTimeout);
    inline bool Int2Ascii(int iVal, char* szVal, int iMaxLen, int* piLen = NULL);
    inline bool Ascii2Int(char* szVal, int iLen, int* piVal);

private:
    bool TestChecksum(byte* pbyData, int iCount);
    int  ComputeChecksum(byte* pbyData, int iCount);
    inline void InitMsgAddr(byte byUnitAddr, PARAM param, MSG_ADDR& msg);
};
