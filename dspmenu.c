#include <exec/types.h>
#include "hdrs/dspmenu.p.h"
#include "hdrs/mand.d.h"
#include "hdrs/mand.p.h"
#include "hdrs/startend.d.h"
#include "hdrs/startend.p.h"
#include "hdrs/startend.v.h"
/*------------------------------------------------------------------------*/
VOID Hires(VOID)
{
        CloseScreens();
        viewmode.hires=~viewmode.hires;
        OpenScreens();

        InitDraw(NOCLS);
}

VOID Lace(VOID)
{
        CloseScreens();
        viewmode.lace=~viewmode.lace;
        OpenScreens();

        InitDraw(NOCLS);
}

VOID H_Overscan(VOID)
{
        CloseScreens();
        viewmode.h_overscan=~viewmode.h_overscan;
        OpenScreens();

        InitDraw(NOCLS);
}

VOID V_Overscan(VOID)
{
        CloseScreens();
        viewmode.v_overscan=~viewmode.v_overscan;
        OpenScreens();

        InitDraw(NOCLS);
}
