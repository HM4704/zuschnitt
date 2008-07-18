// GBitmap.cpp: implementation of the CGBitmap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Macros.h"
#include "GBitmap.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGBitmap::CGBitmap()
{
    m_pBmi = NULL;
    m_cDc  = NULL;
    m_hBmp = NULL;
}

CGBitmap::~CGBitmap()
{
    SAFE_DELETE(m_pBmi);
}

PBITMAPINFO CGBitmap::CreateBitmapInfoStruct(void)
{
    BITMAP bmp; 
    PBITMAPINFO pbmi; 
    WORD    cClrBits; 

    // Retrieve the bitmap color format, width, and height. 
    if (!::GetObject(GetSafeHandle(), sizeof(BITMAP), (LPSTR)&bmp)) 
        //errhandler("GetObject", hwnd); 
        return NULL;

    // Convert the color format to a count of bits. 
    cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel); 
    if (cClrBits == 1) 
        cClrBits = 1; 
    else if (cClrBits <= 4) 
        cClrBits = 4; 
    else if (cClrBits <= 8) 
        cClrBits = 8; 
    else if (cClrBits <= 16) 
        cClrBits = 16; 
    else if (cClrBits <= 24) 
        cClrBits = 24; 
    else cClrBits = 32; 

    // Allocate memory for the BITMAPINFO structure. (This structure 
    // contains a BITMAPINFOHEADER structure and an array of RGBQUAD 
    // data structures.) 

     if (cClrBits != 24) 
         pbmi = (PBITMAPINFO) new unsigned char[
                    sizeof(BITMAPINFOHEADER) + 
                    sizeof(RGBQUAD) * (1<< cClrBits)]; 

     // There is no RGBQUAD array for the 24-bit-per-pixel format. 

     else 
         pbmi = (PBITMAPINFO) new unsigned char[ 
                    sizeof(BITMAPINFOHEADER)]; 

    // Initialize the fields in the BITMAPINFO structure. 

    pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER); 
    pbmi->bmiHeader.biWidth = bmp.bmWidth; 
    pbmi->bmiHeader.biHeight = bmp.bmHeight; 
    pbmi->bmiHeader.biPlanes = bmp.bmPlanes; 
    pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel; 
    if (cClrBits < 24) 
        pbmi->bmiHeader.biClrUsed = (1<<cClrBits); 

    // If the bitmap is not compressed, set the BI_RGB flag. 
    pbmi->bmiHeader.biCompression = BI_RGB; 

    // Compute the number of bytes in the array of color 
    // indices and store the result in biSizeImage. 
    // For Windows NT, the width must be DWORD aligned unless 
    // the bitmap is RLE compressed. This example shows this. 
    // For Windows 95/98/Me, the width must be WORD aligned unless the 
    // bitmap is RLE compressed.
    pbmi->bmiHeader.biSizeImage = ((pbmi->bmiHeader.biWidth * cClrBits +31) & ~31) /8
                                  * pbmi->bmiHeader.biHeight; 
    // Set biClrImportant to 0, indicating that all of the 
    // device colors are important. 
     pbmi->bmiHeader.biClrImportant = 0; 
     return pbmi; 
}


BOOL CGBitmap::LoadBitmap( UINT nIDResource )
{
    BOOL bRet;
    bRet = CBitmap::LoadBitmap(nIDResource);
    if (bRet == FALSE)
        return bRet;

    SAFE_DELETE(m_pBmi);
    m_pBmi = CreateBitmapInfoStruct();
    if (m_pBmi == NULL)
        return FALSE;

    return TRUE;
}

HDC CGBitmap::GetCompatibleDC(HDC hdc)
{
    if (m_pBmi != NULL)
    {
        // Bmp ist geladen
        if (m_cDc == NULL)
        {
            // DC ist noch nicht vorbereitet
            unsigned char* pBits = new unsigned char[m_pBmi->bmiHeader.biSizeImage];
            GetBitmapBits(m_pBmi->bmiHeader.biSizeImage, pBits);


            m_cDc = ::CreateCompatibleDC(hdc);
            HBITMAP m_hBmp = ::CreateCompatibleBitmap(m_cDc, 
		            (WORD)m_pBmi->bmiHeader.biWidth, (WORD)m_pBmi->bmiHeader.biHeight);


            ::SetDIBits(m_cDc, m_hBmp, 0, (WORD)m_pBmi->bmiHeader.biHeight, 
			            (LPSTR)pBits, 
			            (LPBITMAPINFO)m_pBmi, DIB_RGB_COLORS);


            HGDIOBJ pOld = ::SelectObject(m_cDc, m_hBmp);

            delete pBits;
        }
        return m_cDc;
    }

    return NULL;
}
