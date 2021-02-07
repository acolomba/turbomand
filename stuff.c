#include <exec/types.h>
#include <intuition/screens.h>
#include <proto/graphics.h>
#include "hdrs/stuff.d.h"
#include "hdrs/stuff.p.h"
#include "hdrs/startend.v.h"
/*------------------------------------------------------------------------*/
UWORD workpalette[]={
        BLACK,GREY,
        BLUE,WHITE,
        0x000,0x000,0x000,0x000,
        0x000,0x000,0x000,0x000,
        BLACK,GREY,
        BLUE,WHITE,
        0x000,WHITE,BLACK,GREY,0x000,0x000,
        0x000,0x000,0x000,0x000,0x000,0x000,
        BLACK,GREY,
        BLUE,WHITE
};
/*------------------------------------------------------------------------*/
VOID SetWorkPalette(VOID)
{
        LoadRGB4(&scr->ViewPort,workpalette,colors);
}

VOID UnsetWorkPalette(VOID)
{
        LoadRGB4(&scr->ViewPort,colormap,colors);
}
