#pragma once
#include "d:\user\hmo\projekte\wimmer\mfc\zuschnitt18\ansteuerungps312\sercom.h"

class CTrCom :
    public CSerCom
{
    CEdit* m_pTraceWindow;
    bool   m_bFirstRcv;
public:
    CTrCom(void);
public:
    ~CTrCom(void);

    void SetTraceWindow(CEdit* pEdit) { m_pTraceWindow = pEdit; };
    virtual bool Send(byte* pbyData, int iCount);
    virtual bool Recv(byte* pbyToken, int iTimeout);
//    bool Recv(byte* pbyToken, int* piCount, int iMaxCount, int iTimeout);
};
