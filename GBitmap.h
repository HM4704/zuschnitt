// GBitmap.h: interface for the CGBitmap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GBITMAP_H__6B2FA4AB_2B42_40C8_8702_095D13C22B25__INCLUDED_)
#define AFX_GBITMAP_H__6B2FA4AB_2B42_40C8_8702_095D13C22B25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGBitmap : public CBitmap  
{
    PBITMAPINFO m_pBmi;
    HBITMAP     m_hBmp;
    HDC         m_cDc;

    PBITMAPINFO CreateBitmapInfoStruct(void);
public:
	CGBitmap();
	virtual ~CGBitmap();

    BOOL LoadBitmap( UINT nIDResource );
    HDC  GetCompatibleDC(HDC hdc);
};

#endif // !defined(AFX_GBITMAP_H__6B2FA4AB_2B42_40C8_8702_095D13C22B25__INCLUDED_)
