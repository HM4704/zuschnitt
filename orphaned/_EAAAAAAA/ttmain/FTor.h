#ifndef FTOR_H
#define FTOR_H

#include "Tordoor.h"

class CFTor : public CTorDoor
{
  public:
    CFTor(CTorDoor* ct);
    CFTor() {};
    virtual void drawTT(HDC dc, int x, int y, int breite, int hoehe, HFONT fntSmall);
    virtual void drawBesch(HDC hdc, int x, int y);
};

#endif
