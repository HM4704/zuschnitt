#pragma once


enum eCutAction { caNew /* Neues Profil, verwurf Rest */, caCut /* vorne wegschneiden */, caLength /* Profillaenge */ };

class CProfileCutElem
{
public:
    eCutAction m_eAction;
    union 
    {
        unsigned long  m_dwLength;     // fuer vorne Wegschneiden
        unsigned long  m_dwRest;       // fuer Rest Verwurf
    };
public:
    CProfileCutElem(void);
public:
    ~CProfileCutElem(void);
};
