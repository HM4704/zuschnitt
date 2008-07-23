#include "stdafx.h"
#include <stddef.h>
#include "PS312.h"
#include "PS312Protocoll.h"



#define PS312_CHKSUM_MIN        0x20


CPS312Protocol::CPS312Protocol(void)
{
    m_pCom = NULL;
    m_iRecvTimeout = 1500;  //?? 1 Sekunde
}

CPS312Protocol::~CPS312Protocol(void)
{
}

// Helper
// macht aus einem Int einen Ascii String ohne 0
bool CPS312Protocol::Int2Ascii(int iVal, char* szVal, int iMaxLen, int* piLen)
{
    char szBuf[20];  // max 20 Stellen
    int iCount;

//    _itoa(iVal, szBuf, 10);
    if (iMaxLen == 2)
        sprintf(szBuf, "%02d", iVal);
    else
    if (iMaxLen == 4)
        sprintf(szBuf, "%04d", iVal);
    else
        sprintf(szBuf, "%d", iVal);

    iCount = strlen(szBuf);
    if (iCount > iMaxLen)
    {
        return false;
    }
    // ohne 0 kopieren
    memcpy(szVal, szBuf, iCount);
    if (piLen)
        *piLen = iCount;

    return true;
}

bool CPS312Protocol::Ascii2Int(char* szVal, int iLen, int* piVal)
{
    char szBuf[20];  // max 20 Stellen

    if (iLen > (sizeof(szBuf)-1))
        return false;

    memcpy(szBuf, szVal, iLen);
    szBuf[iLen] = 0;

    *piVal = atoi(szBuf);

    return true;
}

inline void CPS312Protocol::InitMsgAddr(byte byUnitAddr, PARAM param, MSG_ADDR& msg)
{
    msg.eot = CC_EOT;
    Int2Ascii(byUnitAddr, (char*)&msg.ad1, 2);
    Int2Ascii((int)param.byLevel, (char*)&msg.c1, 2);
    Int2Ascii((int)param.byNumber, (char*)&msg.c3, 2);
//    msg.enq = CC_ENQ;
}

int CPS312Protocol::ReadParam(byte byUnitAddr, PARAM param, int* iParam)
{
    // Anforderung senden
    // Frame Aufbau: EOT AD1 AD2 STX C1 C2 C3 C4 ENQ
       
    READ_MSG msg;
    byte byMsg[MAX_MSG_LEN];
    int iRet, iRcvd, iFirst;

    InitMsgAddr(byUnitAddr, param, msg.a);
    msg.enq = CC_ENQ;
    msg.a.stx = CC_STX;

    if ((iRet = SendData((byte*)&msg, sizeof(msg))) != PS312_PROTO_NO_ERR)
    {
        return iRet;
    }

    if ((iRet = RecvData(byMsg, &iRcvd, sizeof(byMsg), m_iRecvTimeout /*iTimeout*/)) != PS312_PROTO_NO_ERR)
    {
        return iRet;
    }
    
    // Antwort auswerten
    if (byMsg[0] == CC_STX)
    {
        //
        if (byMsg[iRcvd - 1] == CC_ETX)
        {
            // ok, Parameter empfangen
            // Laenge bestimmen
            iFirst = offsetof(READ_ANSW_MSG, c4);
            iFirst++;                     // auf ersten Text zeigen
            Ascii2Int((char*)&byMsg[iFirst], iRcvd - iFirst, iParam);
            // Fertig
        }
        else if (byMsg[iRcvd - 1] == CC_EOT)
        {
            // ungültiger oder unbekannter Parameter
            return PS312_PROTO_INVALID_PARAM;
        }
    }
    else if (byMsg[0] == CC_NAK)
    {
        return PS312_PROTO_NAK_ERR;
    }
    else
    {
        // unbekanntes Format
        return PS312_PROTO_FRAME_ERR;
    }

    return PS312_PROTO_NO_ERR;
}

int CPS312Protocol::SendParam(byte byUnitAddr, PARAM param, int iParam)
{
    // Parameter senden
    // Frame Aufbau: EOT AD1 AD2 STX C1 C2 C3 C4 XXXXXXXX ETX BCC
    SEND_MSG* pMsg;
    byte byMsg[MAX_MSG_LEN];
    int iParamLen, iMsgLen, iRcvd, iRet;

    pMsg = (SEND_MSG*)byMsg;

    InitMsgAddr(byUnitAddr, param, pMsg->a);
    pMsg->a.stx = CC_STX;
    
    if (Int2Ascii(iParam, (char*)&pMsg->val[0], MAX_MSG_LEN - sizeof(MSG_ADDR) - 1 /*etx*/ , &iParamLen) == false)
    {
        return PS312_PROTO_TOO_LARGE;
    }

    iMsgLen = sizeof(*pMsg) - sizeof(pMsg->val) + iParamLen;
    byMsg[iMsgLen-1] = CC_ETX;   //??

    if ((iRet = SendData(byMsg, iMsgLen)) != PS312_PROTO_NO_ERR)
    {
        return iRet;
    }

    if ((iRet = RecvData(byMsg, &iRcvd, sizeof(byMsg), m_iRecvTimeout /*iTimeout*/)) != PS312_PROTO_NO_ERR)
    {
        return iRet;
    }

    if (byMsg[0] == CC_ACK)
    {
        // ok
        return PS312_PROTO_NO_ERR;
    }
    if (byMsg[0] == CC_NAK)
    {
        // Gerät hat abgelehnt
        return PS312_PROTO_NAK_ERR;
    }

    // alles andere macht hier keinen Sinn
    return PS312_PROTO_FRAME_ERR;
}

int CPS312Protocol::SendData(byte* pbyData, int iCount)
{
    byte byMsg[MAX_MSG_LEN];

    if (m_pCom == NULL)
        return PS312_PROTO_NO_COM;

    if (iCount >= MAX_MSG_LEN)
        // zu viele Daten
        return PS312_PROTO_TOO_LARGE;


    // immer erst alle anstehen Bytes verwerfen
    m_pCom->ClearRecv();

    // umkopieren
    memcpy(byMsg, pbyData, iCount);
    byMsg[iCount] = ComputeChecksum(byMsg, iCount);
    if (byMsg[iCount] >= PS312_CHKSUM_MIN)
        // Checksumme wurde berechnet, 1 byte mehr senden
        iCount++;
    else
        if (byMsg[iCount] != 0)
            // Fehler erkannt, Abbruch
            return byMsg[iCount];

    if (m_pCom->Send(byMsg, iCount) == false)
        return PS312_PROTO_SEND_ERR;

    return PS312_PROTO_NO_ERR;
}

// ACHTUNG: timeout muss groesser iMaxCount sein!!!
int CPS312Protocol::RecvData(byte* pbyData, int* piCount, int iMaxCount, int iTimeout)
{
    int iCount = 0, iInterval, iRetry = 0;
    byte byMsg[MAX_MSG_LEN];
    bool bEndRcv = false;

    if (m_pCom == NULL)
        return PS312_PROTO_NO_COM;

    if (iMaxCount > MAX_MSG_LEN)
        // zu viele Daten
        return PS312_PROTO_TOO_LARGE;

    if (iTimeout == -1)
    {
        // INFINITE
        iInterval = INFINITE;
    }
    else        
    {
        iInterval = iTimeout/iMaxCount;
    }

    *piCount = 0;
    while ((iCount < iMaxCount) && (iRetry < iMaxCount))
    {
        if (m_pCom->Recv(&byMsg[iCount], iInterval) == true)
        {
            // byte empfangen, auswerten
            if ((byMsg[iCount] == CC_ACK) || (byMsg[iCount] == CC_ACK)
                || (byMsg[iCount] == CC_ENQ))
            {
                // bei ACK oder NACK oder ENQ fertig
                bEndRcv = true;
            }
            if (iCount > 1)
            {
                if (byMsg[iCount-1] == CC_ETX)
                {
                    // end of text, Checksumme pruefen
                    if (TestChecksum(byMsg, iCount) == true)
                    {
                        // ok, ohne checksumme nach oben
                        iCount -= 1;
                        bEndRcv = true;
                    }
                    else
                    {
                        // checksumme nicht ok, Fehler melden
                        return PS312_PROTO_CHKSUM_ERR;
                    }
                }
                else
                if (byMsg[iCount] == CC_EOT)
                {
                    // Abweisung einer Anfrage, Ende
                    bEndRcv = true;
                }
            }
            if (bEndRcv == true)
            {
                // ok, fertig.
                iCount++;
                *piCount = iCount;
                memcpy(pbyData, byMsg, *piCount);
                return PS312_PROTO_NO_ERR;
            }
            iCount++;
        }
        else
        {
            // timeout, retry
            iRetry++;
        }
    }
    if (iCount > 0)
    {
        // alle empfangenen bytes zurückgeben
        memcpy(pbyData, byMsg, iCount);
        *piCount = iCount;
    }
    else
        return PS312_PROTO_TIMEOUT;

    return PS312_PROTO_NO_ERR;
}

// Checksumme ist das xor von C1 (1 nach STX) bis CC_ETX inklusive
bool CPS312Protocol::TestChecksum(byte* pbyData, int iCount)
{
    int i = 0;
    byte byChksum = 0;

    while (i < iCount)
    {
        if (pbyData[i] == CC_STX)
        {
            // STX gefunden, ab dem nächsten Checksumme bis ETX
            i++;
            while (i < iCount)
            {
                byChksum ^= pbyData[i];
                if (pbyData[i] == CC_ETX)
                {
                    // ende
                    if (byChksum < PS312_CHKSUM_MIN)
                        byChksum += PS312_CHKSUM_MIN;
                    i++;  // i auf BCC setzen
                    if (pbyData[i] != byChksum)
                        return false;
                    else
                        return true;
                }
                i++;
            }
            // stx gefunden aber kein etx, kommt bei Anforderung oder negativer Antwort vor
            if ((pbyData[i-1] == CC_ENQ) || (pbyData[i-1] == CC_EOT))
                return true;
            else
                return false;
        }
        i++;
    }

    return true;
}

// berechnet BCC, Rückgabe == 0: keine BCC für den FRame nötig
// < 0x20: Fehler
int  CPS312Protocol::ComputeChecksum(byte* pbyData, int iCount)
{
    int i = 0;
    byte byChksum = 0;

    while (i < iCount)
    {
        if (pbyData[i] == CC_STX)
        {
            // STX gefunden, ab dem nächsten Checksumme bis ETX
            i++;
            while (i < iCount)
            {
                byChksum ^= pbyData[i];
                if (pbyData[i] == CC_ETX)
                {
                    // ende
                    if (byChksum < PS312_CHKSUM_MIN)
                        byChksum += PS312_CHKSUM_MIN;

                    return byChksum;

                }
                i++;
            }
            // stx gefunden aber kein etx, kommt bei Anforderung oder negativer Antwort vor
            if ((pbyData[i-1] == CC_ENQ) || (pbyData[i-1] == CC_EOT))
                return 0;
            else
                return PS312_PROTO_FRAME_ERR;
        }
        i++;
    }

    return 0;
}

char* CPS312Protocol::GetErrorString(int iError)
{
    // Achtung: gleiche Reihenfolge wie Fehlercodes in PS312Protocoll.h
    static _TCHAR* s_ErrorStrings[] = {
        { _T("Kein Fehler!") },
        { _T("Gerät antwortet nicht!") },
        { _T("Checksummen Fehler!") },
        { _T("Datenteil zu lang!") },
        { _T("keine Schnittstelle gefunden!") },
        { _T("Fehler in einem Frame gefunden!") },
        { _T("Fehler beim Senden!") },
        { _T("Gerät lehnt Anforrderung ab!") },
        { _T("Gerät kennt Parameter nicht!") }
    };

    return s_ErrorStrings[iError];
}
