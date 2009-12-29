#ifndef CTORN_H
#define CTORN_H

#include "Tordoor.h"
#include "GBitmap.h"


class CTorN : public CTorDoor
{
  public:
   CTorN() {};
   CTorN(CTorDoor* ct) : CTorDoor(ct) {};
   virtual void drawTT(HDC dc, int x, int y, int breite, int hoehe, HFONT fntSmall);
   virtual void drawBesch(HDC hdc, int x, int y);
};


class CSchiebeTor : public CTorN
{
    CGBitmap m_rollAppBmp;
  public:
   CSchiebeTor();
   CSchiebeTor(CTorDoor* ct);
   virtual void drawTT(HDC dc, int x, int y, int breite, int hoehe, HFONT fntSmall);
   virtual void drawBesch(HDC hdc, int x, int y);
};

#endif