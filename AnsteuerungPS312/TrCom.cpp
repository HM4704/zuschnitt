#include "stdafx.h"
#include "TrCom.h"


static char szHexDigits[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

CTrCom::CTrCom(void)
{
    m_pTraceWindow = NULL;
    m_bFirstRcv = true;
}

CTrCom::~CTrCom(void)
{
}

bool CTrCom::Send(byte* pbyData, int iCount)
{
    if (m_pTraceWindow != NULL)
    {
        static int s_iSendCount = 0;
        char szBuf[MAX_PATH], *pC;
        int i;
        if (m_bFirstRcv == false)
        {
            // es war vorher ein Recv, Zeilenumbruch einfügen
            m_bFirstRcv = true;
            sprintf(szBuf, "\r\nS(%03ld): ", s_iSendCount++);
        }
        else
            sprintf(szBuf, "S(%03ld): ", s_iSendCount++);
        pC = szBuf + strlen(szBuf);
        for (i = 0; i < iCount; i++)
        {
           *pC = szHexDigits[pbyData[i]>>4];
           pC++;
           *pC = szHexDigits[pbyData[i]&0x0f];
           pC++;
           *pC = ' ';
           pC++;
        }
        *pC = '\r';
        pC++;
        *pC = '\n';
        pC++;
        *pC = 0;
        m_pTraceWindow->ReplaceSel(szBuf, 0);
    }
    return CSerCom::Send(pbyData, iCount);
}

bool CTrCom::Recv(byte* pbyToken, int iTimeout)
{
    bool bRet;
    if ((bRet = CSerCom::Recv(pbyToken, iTimeout)) == true)
    {
        if (m_pTraceWindow != NULL)
        {
            static int s_iRecvCount = 0;
            char szBuf[MAX_PATH], *pC;
            int i;
            if (m_bFirstRcv == true)
            {
                m_bFirstRcv = false;
                sprintf(szBuf, "R(%03ld): ", s_iRecvCount++);
            }
            else
                strcpy(szBuf, " ");
            pC = szBuf + strlen(szBuf);
            *pC = szHexDigits[pbyToken[0]>>4];
            pC++;
            *pC = szHexDigits[pbyToken[0]&0x0f];
            pC++;
            *pC = 0;
            m_pTraceWindow->ReplaceSel(szBuf, 0);
        }
    }

    return bRet;
}
