#ifndef CTUERN_H
#define CTUERN_H

#include "Tordoor.h"

#define SEHNENHOEHE  18

class CTuerN : public CTorDoor
{
  public:
   CTuerN() {};
   CTuerN(CTorDoor* ct) : CTorDoor(ct) {};
   virtual void drawTT(HDC dc, int x, int y, int breite, int hoehe);
   virtual void drawBesch(HDC hdc, int x, int y);
   virtual float calculateScale(int breite, int hoehe);
};

class CSchiebeTuer : public CTuerN
{
  public:
   CSchiebeTuer() {};
   CSchiebeTuer(CTorDoor* ct) : CTuerN(ct) {};
   virtual void drawTT(HDC dc, int x, int y, int breite, int hoehe);
   virtual void drawBesch(HDC hdc, int x, int y);
};

#endif