#ifndef _TRAYICON_H
#define _TRAYICON_H


class CTrayIcon : public CObject
{
public:
	BOOL Close(void);
	BOOL Modify(HICON hIcon, char* strTip);
	BOOL Create(CWnd *pWnd, UINT uCallbMsg, UINT uId, HICON hIcon, char* strTip);
	CTrayIcon();
	virtual ~CTrayIcon();

private:
    NOTIFYICONDATA      m_notIconData;
    BOOL                m_bCreated;

};


#endif