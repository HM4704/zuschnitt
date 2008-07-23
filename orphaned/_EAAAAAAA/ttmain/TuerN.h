#ifndef CTUERN_H
#define CTUERN_H

#include "Tordoor.h"

#define SEHNENHOEHE  18

class CTuerN : public CTorDoor
{
  public:
   CTuerN(CTorDoor* ct) : CTorDoor(ct) {};
   virtual void drawTT(HDC dc, int x, int y, int breite, int hoehe);
   virtual void drawBesch(HDC hdc, int x, int y);
   virtual float calculateScale(int breite, int hoehe);
};

#endif