#include "StdAfx.h"
#include "..\PS312.h"
#include "PS312Simul.h"
#include <map>

const char cNAK = CC_NAK;
const char cACK = CC_ACK;
extern  bool bLocal;

static void PrintFrame(byte* pbyMsg, int iLen)
{
    int i;
    char szBuf[MAX_PATH];

    for (i = 0; i < iLen; i++)
    {
        printf("%d ", pbyMsg[i]);
    }
    memcpy(szBuf, pbyMsg, iLen);
    szBuf[iLen] = 0;
    printf(" : %s", szBuf);
    printf("\n");
}
CPS312Sim::CPS312Sim(void)
{
    m_byUnitAddr = 11;      //?? fest
    memset(m_param, 0, sizeof(m_param));
    memset(m_DataSets, 0, sizeof(m_DataSets));
    memset(m_Programs, 0, sizeof(m_Programs));

    InitParams();
    InitPrograms();
}

CPS312Sim::~CPS312Sim(void)
{
    int iLevel, iNr;
    for (iLevel = 0; iLevel < 2; iLevel++)
    {
        for (iNr = 0; iNr < PS312_MAX_PARAMS; iNr++)
        {
            if (m_param[iLevel][iNr] != NULL)
                delete m_param[iLevel][iNr];
        }
    }
}

void CPS312Sim::WaitValidFrame(PARAM* para, byte* byMsg, int iMaxLen, int* iMsgLen, int iTimeout)
{
    int iRet, iUnitAddr, iVal;
    MSG_ADDR* pMsg = (MSG_ADDR*)byMsg;

    while (TRUE)
    {
        iRet = RecvData(byMsg, iMsgLen, iMaxLen, iTimeout);
        if (iRet != PS312_PROTO_NO_ERR)
        {
            // Fehler
            printf("PS312Sim: ERROR (%ld) in RecvData()\n", iRet);
            continue;
        }
        PrintFrame(byMsg, *iMsgLen);
        if (pMsg->eot == CC_EOT)
        {
            // Unit Addr überprüfen
            Ascii2Int((char*)&pMsg->ad1, 2, &iUnitAddr);
            if ((byte)iUnitAddr != m_byUnitAddr)
            {
                // ungültige Addresse, keine Antwort
                printf("CPS312Sim - unkown unit: ");
                PrintFrame(byMsg, *iMsgLen);
                continue;
            }
            // Param Block in Wert wandeln
            Ascii2Int((char*)&pMsg->c1, 4, &iVal);
            para->byLevel  = iVal/100;
            para->byNumber = iVal%100;
            break;
        }
        else
        {
            // unbekannter Frame
            printf("CPS312Sim - rcvd unkown msg: ");
            PrintFrame(byMsg, *iMsgLen);
            if ((iRet = SendData((byte*)&cNAK, 1)) != PS312_PROTO_NO_ERR)
            {
                printf("CPS312Sim: ERROR(%ld) in SendData()\n", iRet);
            }
        }
    }
}

void CPS312Sim::Run(void)
{
    int iRcvd;
    byte byMsg[MAX_MSG_LEN];
    MSG_ADDR* pMsg = (MSG_ADDR*)byMsg;
    PARAM para;

    printf("PS312Sim started!\n\n");
    while (TRUE)
    {
        if (bLocal == true)
            WaitValidFrame(&para, byMsg, sizeof(byMsg), &iRcvd, -1);
        else
            WaitValidFrame(&para, byMsg, sizeof(byMsg), &iRcvd, 0x0f000000);

        if (byMsg[iRcvd-1] == CC_ETX)
        {
            // send param 
            HandleSendParam(para, byMsg, iRcvd);
        }
        else
        if (byMsg[iRcvd-1] == CC_ENQ)
        {
            // read param
            HandleReadParam(para, (READ_MSG*)byMsg);
        }
        else
        {
            // Fehler unbekannte Msg
            printf("CPS312Sim - rcvd unkown msg: ");
            PrintFrame(byMsg, iRcvd);
        }
    }
}

void CPS312Sim::HandleSendParam(PARAM para, byte* pbyMsg, int iLen)
{
    int iRet;
    SEND_MSG* pMsg = (SEND_MSG*)pbyMsg;
    CPS312Param* pParam;

    // Param Block prüfen
    if ((para.byLevel == PS312_LEVEL_ALL) || (para.byLevel == PS312_LEVEL_AXIS1))
    {
        if (para.byNumber == PS312_PARAM_CMD)
        {
            // Cmd 
            return HandleCmd(para, pMsg, iLen);
        }

        // ok, abspeichern
        if ((pParam = m_param[para.byLevel-PS312_LEVEL_ALL][para.byNumber]) == NULL)
        {
            printf("CPS312Sim: parameter not valid. Frame:\n");
            PrintFrame((byte*)pbyMsg, iLen);
            // sende NAK
            if ((iRet = SendData((byte*)&cNAK, 1)) != PS312_PROTO_NO_ERR)
            {
                printf("CPS312Sim: ERROR(%ld) in SendData()\n", iRet);
            }
            return;
        }
        else
        {
            if (para.byNumber == PS312_PARAM_ACTIVE_PROG_NR)
            {
                // überprüfen, ob sich Programm geäendert hat, wenn ja Datei schliessen
                int  iLength;
                char szOldVal[100];

                m_param[para.byLevel-PS312_LEVEL_ALL][para.byNumber]->GetValueString(szOldVal, sizeof(szOldVal), &iLength);
                szOldVal[iLen] = 0;
                if (strncmp(szOldVal, (char*)pMsg->val, PAYLOAD_SEND_MSG(iLen)) != 0)
                {
                    // Wert hat sich geaendert, Datei schliessen
                    if (m_file.is_open())
                        m_file.close();
                }
            }
            if (m_param[para.byLevel-PS312_LEVEL_ALL][para.byNumber]->SaveValue((char*)pMsg->val, PAYLOAD_SEND_MSG(iLen)) == false)
            {
                // sende NAK
                if ((iRet = SendData((byte*)&cNAK, 1)) != PS312_PROTO_NO_ERR)
                {
                    printf("CPS312Sim: ERROR(%ld) in SendData()\n", iRet);
                }
                return;
            }
        }

        // ok, sende ACK
        if ((iRet = SendData((byte*)&cACK, 1)) != PS312_PROTO_NO_ERR)
        {
            printf("CPS312Sim: ERROR(%ld) in SendData()\n", iRet);
        }
    }
    else
    {
        // unbekannte Ebene, antworte mit NAK
        if ((iRet = SendData((byte*)&cNAK, 1)) != PS312_PROTO_NO_ERR)
        {
            printf("CPS312Sim: ERROR(%ld) in SendData()\n", iRet);
        }
    }
}

void CPS312Sim::SendReadAnsw(PARAM para, READ_MSG* pReadMsg, int iVal)
{
    int iRet, iLen;
    byte byMsg[MAX_MSG_LEN];
    READ_ANSW_MSG* pMsg = (READ_ANSW_MSG*)byMsg;

    // ok, Ebene stimmt
    // Antwort Msg aufbauen
    pMsg->stx = CC_STX;
    memcpy(&pMsg->c1, &pReadMsg->a.c1, 4);

    if (Int2Ascii(iVal, (char*)pMsg->val, MAX_MSG_LEN - sizeof(MSG_ADDR) - 1 /*etx*/, &iLen) == false)
        return;

    iLen += offsetof(READ_ANSW_MSG, val);
    byMsg[iLen] = CC_ETX;
    iLen++;
    if ((iRet = SendData((byte*)pMsg, iLen)) != PS312_PROTO_NO_ERR)
    {
        printf("CPS312Sim: ERROR(%ld) in SendData()\n", iRet);
    }
}

void CPS312Sim::HandleReadParam(PARAM para, READ_MSG* pReadMsg)
{
    int iRet, iLen;
    CPS312Param* pParam;
    byte byMsg[MAX_MSG_LEN];
    READ_ANSW_MSG* pMsg = (READ_ANSW_MSG*)byMsg;

#if 0
    //?? Fehler wieder raus
    byMsg[0] = 0x36;
    m_pCom->Send(byMsg, 1);
    return;
#endif // 0

    // Param Block prüfen
    if ((para.byLevel == PS312_LEVEL_ALL) || (para.byLevel == PS312_LEVEL_AXIS1))
    {
        // ok, Ebene stimmt
        // Antwort Msg aufbauen
        pMsg->stx = CC_STX;
        memcpy(&pMsg->c1, &pReadMsg->a.c1, 4);

        // Wert holen
        if ((pParam = m_param[para.byLevel-PS312_LEVEL_ALL][para.byNumber]) == NULL)
        {
            printf("CPS312Sim: parameter not valid. Frame:\n");
            PrintFrame((byte*)pReadMsg, sizeof(*pReadMsg));
            // kein Parameter zum Auslesen, antworte mit: STX C1 C2 C3 C4 EOT
            pMsg->eot = CC_EOT;
            if ((iRet = SendData((byte*)pMsg, 6)) != PS312_PROTO_NO_ERR)
            {
                printf("CPS312Sim: ERROR(%ld) in SendData()\n", iRet);
            }
            return;
        }

        pParam->GetValueString((char*)&pMsg->val[0], PS312_MAX_VAL_LEN, &iLen);
        iLen += offsetof(READ_ANSW_MSG, val);
        byMsg[iLen] = CC_ETX;
        iLen++;
        if ((iRet = SendData((byte*)pMsg, iLen)) != PS312_PROTO_NO_ERR)
        {
            printf("CPS312Sim: ERROR(%ld) in SendData()\n", iRet);
        }
    }
    else
    {
        // unbekannte Ebene, antworte mit NAK
        if ((iRet = SendData((byte*)&cNAK, 1)) != PS312_PROTO_NO_ERR)
        {
            printf("CPS312Sim: ERROR(%ld) in SendData()\n", iRet);
        }
    }

}

static void InsertParam(CPS312Param** ppSave, char* szName, int iVal, int iMin, int iMax, bool bReadOnly)
{
    CPS312ParamInt* pIParam;

    pIParam = new CPS312ParamInt(szName, iVal, iMin, iMax, bReadOnly);
    *ppSave = pIParam;
}

static void InsertParam(CPS312Param** ppSave, char* szName, float fVal, float fMin, float fMax, bool bReadOnly)
{
    CPS312ParamFloat* pFParam;

    pFParam = new CPS312ParamFloat(szName, fVal, fMin, fMax, bReadOnly);
    *ppSave = pFParam;
}

void CPS312Sim::InitParams(void)
{
    int iLevel = 0;

    //Parameter Ebene 
    InsertParam(&m_param[iLevel][0], "Achsenanzahl", 1, 1, 3, false);
    InsertParam(&m_param[iLevel][3], "Kundennummer", 1007, 0, 10000, true /* readonly */);
    InsertParam(&m_param[iLevel][5], "Referenzfahrt Freigabe", 0, 0, 1, false);
    InsertParam(&m_param[iLevel][6], "Betriebsmode", 1, 0, 1, true);
    InsertParam(&m_param[iLevel][29], "Logik Schalteingänge", 61, 0, 255, false);
    InsertParam(&m_param[iLevel][30], "Logik Schalteingänge 2", 8, 0, 15, false);
    InsertParam(&m_param[iLevel][33], "Schaltverhalten Encodereingänge", 0, 0, 1, false);
    InsertParam(&m_param[iLevel][34], "Schaltverhalten Schalteingänge", 1, 0, 1, false);
    InsertParam(&m_param[iLevel][35], "Funktion Taste M", 1, 0, 3, false);
    //...
    InsertParam(&m_param[iLevel][60], "Anzahl der Programme", 50/*20*/, 1, 99, false);        //??
    InsertParam(&m_param[iLevel][61], "Programmablauf", 0, 0, 3, false);
    InsertParam(&m_param[iLevel][62], "aktuelle Programm Nr.", 5, 0, 99, false);        //??
    InsertParam(&m_param[iLevel][63], "Status Sollwert", 0, 0, 1, true);                //??
    InsertParam(&m_param[iLevel][64], "Endlagenüberwachung", 0, 0, 1, false);
    InsertParam(&m_param[iLevel][67], "Gesamtstückzähler", 0, 0, 1, false);
    InsertParam(&m_param[iLevel][90], "Geräteadress", 11, 11, 99, false);
    InsertParam(&m_param[iLevel][91], "Baudrate", 0, 0, 3, false);
    InsertParam(&m_param[iLevel][94], "Maske für Ausgänge", 0, 0, 255, false);
    InsertParam(&m_param[iLevel][95], "Ausgänge über seriell", 0, 0, 255, false);

    //Parameter Achs
    iLevel++;
    InsertParam(&m_param[iLevel][0], "Istwert", 0, -999999, 999999, false);
    InsertParam(&m_param[iLevel][1], "Sollwert", 0, -999999, 999999, false);
    InsertParam(&m_param[iLevel][2], "Eichwert", 0, -999999, 999999, false);
    InsertParam(&m_param[iLevel][3], "Softwareendschalter unten", 0, -999999, 999999, false);
    InsertParam(&m_param[iLevel][4], "Softwareendschalter oben", 0, -999999, 999999, false);
    //...
    InsertParam(&m_param[iLevel][41], "Displayhelligkeit", 0, 0, 15, false);
    InsertParam(&m_param[iLevel][42], "Positioniermode", 0, 0, 1, false);
    InsertParam(&m_param[iLevel][43], "Kettenmassfunktion", 0, 0, 3, false);
    InsertParam(&m_param[iLevel][44], "Sägeblattstärke", 3, -999999, 999999, false);
    InsertParam(&m_param[iLevel][47], "Offset Parkposition", 0, -999999, 999999, false);
    InsertParam(&m_param[iLevel][80], "Status", 5, 0, 256, true);

}

void CPS312Sim::InitPrograms(void)
{
    int i, iProgNum, iDataSetNum;

    // Anzahl Programm abfragen (Ebene All, P60)
    CPS312ParamInt* pParam = (CPS312ParamInt*)m_param[0][PS312_PARAM_COUNT_PROGS];
    iProgNum = pParam->GetValueInt();

    // Anzahl Datensätze pro Programm berechnen
    iDataSetNum = PS312_DATA_SET_COUNT/iProgNum;

    for (i = 0; i < iProgNum; i++)
    {
        m_Programs[i].bValid = true;
        m_Programs[i].pFirstDataSet = &m_DataSets[i*iDataSetNum];
        m_Programs[i].pLastDataSet  = &m_DataSets[i*iDataSetNum+(iDataSetNum-1)];
    }
}

void CPS312Sim::HandleCmd(PARAM para, SEND_MSG* pMsg, int iLen)
{
    char szCmd[30];
    int iCmd, iPLoadLen, iRet;

    iPLoadLen = PAYLOAD_SEND_MSG(iLen);
    memcpy(szCmd, pMsg->val, iPLoadLen);
    szCmd[iPLoadLen] = 0;

    iCmd = atoi(szCmd);

    if ((iCmd < PS312_CMD_MIN) || (iCmd > PS312_CMD_MAX))
    {
        // ungültiges Cmd, send NAK
        if ((iRet = SendData((byte*)&cNAK, 1)) != PS312_PROTO_NO_ERR)
        {
            printf("CPS312Sim: ERROR(%ld) in SendData()\n", iRet);
        }
        return;
    }

    switch (iCmd)
    {
        case PS312_CMD_START_PROG_LOAD:
            DoProgCmd();
            break;
        case PS312_CMD_START_PROG:
            DoStartProgCmd();
            break;
        default:
            // alles andere einfach ACK senden
            if ((iRet = SendData((byte*)&cACK, 1)) != PS312_PROTO_NO_ERR)
            {
                printf("CPS312Sim: ERROR(%ld) in SendData()\n", iRet);
            }
            break;
    }
}

void CPS312Sim::DoStartProgCmd(void)
{
    typedef map <int, int, greater<int>>::iterator MIIter;
    typedef pair <int, int> Int_Pair;

    int iRet, iProgNum;
    PS312_DATA_SET* pActDataSet;
    map <int, int, greater<int>> InputMap;
    pair< MIIter, bool > pr;
    MIIter mIter;
    char strFile[MAX_PATH];

    // sende ACK
    if ((iRet = SendData((byte*)&cACK, 1)) != PS312_PROTO_NO_ERR)
    {
        printf("CPS312Sim: ERROR(%ld) in SendData()\n", iRet);
    }

    // get active prog number (Ebene All, P62)
    CPS312ParamInt* pParam = (CPS312ParamInt*)m_param[0][PS312_PARAM_ACTIVE_PROG_NR];
    iProgNum = pParam->GetValueInt();

    if (m_file.is_open() == false)
    {
        sprintf(strFile, "PS312CutListP%d.txt", iProgNum);
        m_file.open(strFile, ios_base::out | ios_base::trunc);    

        if (m_file.fail())
        {
            printf("CPS312Sim: ERROR in opening file\n");
            return;
        }
    }

    // erster Datensatz
    pActDataSet = m_Programs[iProgNum].pFirstDataSet;
   

    while (pActDataSet != m_Programs[iProgNum].pLastDataSet)
    {
#if 0
        pr = InputMap.insert ( Int_Pair ( pActDataSet->iLength, pActDataSet->iCount ) );
        if( pr.second == true ) 
        {
            // The element was inserted successfully
        }
        else 
        {
            // Key number already exists, add it
            ( pr.first )->second += pActDataSet->iCount;
        }
#else
        if ((pActDataSet->iCount != 0) && (pActDataSet->iLength != 0))
            m_file << pActDataSet->iCount << ',' << (pActDataSet->iLength/10) << endl;
        else
            break;
#endif //
        pActDataSet++;
    }

#if 0
    for (mIter = InputMap.begin(); mIter != InputMap.end(); mIter++)
    {
        if ((mIter->second != 0) && (mIter->first != 0))
            m_file << mIter->second << ',' << (mIter->first/10) << endl;
    }
#endif
    m_file << flush;
}

void CPS312Sim::DoProgCmd(void)
{
    bool bAck;
    int iRcvd, iProgNum, iVal, iPLoadLen,iRet;
    PS312_DATA_SET* pActDataSet;
    byte byMsg[MAX_MSG_LEN];
    MSG_ADDR* pMsg = (MSG_ADDR*)byMsg;
    SEND_MSG* pSendMsg = (SEND_MSG*)byMsg;
    PARAM para;
    char szVal[30];
    eMass statMass = MASS_ABSOLUT;

    // sende ACK
    if ((iRet = SendData((byte*)&cACK, 1)) != PS312_PROTO_NO_ERR)
    {
        printf("CPS312Sim: ERROR(%ld) in SendData()\n", iRet);
    }

    // get active prog number (Ebene All, P62)
    CPS312ParamInt* pParam = (CPS312ParamInt*)m_param[0][PS312_PARAM_ACTIVE_PROG_NR];
    iProgNum = pParam->GetValueInt();
    
    pActDataSet = m_Programs[iProgNum].pFirstDataSet;

    while (TRUE)
    {
        WaitValidFrame(&para, byMsg, sizeof(byMsg), &iRcvd, -1);
        if (byMsg[iRcvd-1] == CC_ETX)
        {
            iPLoadLen = PAYLOAD_SEND_MSG(iRcvd);
            memcpy(szVal, pSendMsg->val, iPLoadLen);
            szVal[iPLoadLen] = 0;
            iVal = atoi(szVal);

            if (para.byLevel == PS312_LEVEL_AXIS1)
            {
                HandleSendParam(para, byMsg, iRcvd);
                continue;
            }
            // abspeichern
            bAck = false;
            if (para.byNumber == PS312_PARAM_CMD)
            {
                // Befehl
                if (iVal == PS312_CMD_SET_ABS_MASS)
                {
                    // Absolutmass
                    statMass = MASS_ABSOLUT;
                    bAck = true;
                }
                if (iVal == PS312_CMD_SET_KETT_MASS)
                {
                    // Absolutmass
                    statMass = MASS_KETTE;
                    bAck = true;
                }
                if (iVal == PS312_CMD_STOP_PROG_LOAD)
                {
                    // Program Ende
                    break;
                }
                // bei allen anderen Cmds NAK senden, da zur Zeit nicht moeglich
            }
            else
            {
                if (para.byLevel == PS312_LEVEL_SOLLW)
                {
                    // Sollwert speichern
                    pActDataSet->iLength= iVal;
                    bAck = true;
                }
                else if (para.byLevel == PS312_LEVEL_COUNT)
                {
                    // Stückzahl abspeichern und auf nächsten Datensatz schalten
                    pActDataSet->iCount = iVal;
                    pActDataSet->mass = statMass;
                    pActDataSet++;
                    if (pActDataSet == m_Programs[iProgNum].pLastDataSet)
                    {
                        //?? Überlauf, von vorne beginnen
                        pActDataSet = m_Programs[iProgNum].pFirstDataSet;
                    }
                    bAck = true;
                }
                else
                {
                    // send param 
                    HandleSendParam(para, byMsg, iRcvd);
                    continue;
                }
            }
            if (bAck)
            {
                if ((iRet = SendData((byte*)&cACK, 1)) != PS312_PROTO_NO_ERR)
                {
                    printf("CPS312Sim: ERROR(%ld) in SendData()\n", iRet);
                }
            }
            else
            {
                if ((iRet = SendData((byte*)&cNAK, 1)) != PS312_PROTO_NO_ERR)
                {
                    printf("CPS312Sim: ERROR(%ld) in SendData()\n", iRet);
                }
            }
        }
        else
        if (byMsg[iRcvd-1] == CC_ENQ)
        {
            if (para.byLevel == PS312_LEVEL_AXIS1)
            {
                // read param
                HandleReadParam(para, (READ_MSG*)byMsg);
                continue;
            }
            if (para.byNumber == PS312_PARAM_MASS_STATUS)
            {
                iVal = (int)pActDataSet->mass;
            }
            else if (para.byLevel == PS312_LEVEL_SOLLW)
            {
                iVal = pActDataSet->iLength/10 /* in 1/10 mm */;
            }
            else if (para.byLevel == PS312_LEVEL_COUNT)
            {
                iVal = pActDataSet->iCount;
                // auf nächsten Datensatz schalten
                pActDataSet++;
                if (pActDataSet == m_Programs[iProgNum].pLastDataSet)
                {
                    //?? Überlauf, von vorne beginnen
                    pActDataSet = m_Programs[iProgNum].pFirstDataSet;
                }
            }
            else
            {
                // read param
                HandleReadParam(para, (READ_MSG*)byMsg);
                continue;
            }
            // Antwort senden
            SendReadAnsw(para, (READ_MSG*)byMsg, iVal);
            continue;
        }
        else
        {
            // Fehler unbekannte Msg
            printf("CPS312Sim - rcvd unkown msg: ");
            PrintFrame(byMsg, iRcvd);
        }
    }    

    // sende ACK
    if ((iRet = SendData((byte*)&cACK, 1)) != PS312_PROTO_NO_ERR)
    {
        printf("CPS312Sim: ERROR(%ld) in SendData()\n", iRet);
    }
}
