#ifndef CTORN_H
#define CTORN_H

#include "Tordoor.h"

class CTorN : public CTorDoor
{
  public:
   CTorN() {};
   CTorN(CTorDoor* ct) : CTorDoor(ct) {};
   virtual void drawTT(HDC dc, int x, int y, int breite, int hoehe);
   virtual void drawBesch(HDC hdc, int x, int y);
};


class CSchiebeTor : public CTorN
{
  public:
   CSchiebeTor() {};
   CSchiebeTor(CTorDoor* ct) : CTorN(ct) {};
   virtual void drawTT(HDC dc, int x, int y, int breite, int hoehe);
   virtual void drawBesch(HDC hdc, int x, int y);
};

#endif