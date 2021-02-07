#include <exec/types.h>
#include <proto/exec.h>
#include "hdrs/areamenu.p.h"
#include "hdrs/atof.p.h"
#include "hdrs/mand.d.h"
#include "hdrs/mand.p.h"
#include "hdrs/mand.v.h"
#include "hdrs/menuhand.d.h"
#include "hdrs/menuhand.p.h"
#include "hdrs/prjmenu.p.h"
#include "hdrs/startend.d.h"
#include "hdrs/startend.p.h"
#include "hdrs/startend.v.h"
#include "hdrs/stuff.p.h"
#include "hdrs/dspmenu.p.h"
/*------------------------------------------------------------------------*/
VOID main()
{
        ITEM item;

        OpenMath();
        OpenArp();

        OpenScreens();
        UnsetMenus();
        About();
        SetMenus();
        SetMand(INTMATH);
        SetMand(FASTMAND);
        InitDraw(NOCLS);

        for(;;){
                do{
                        WaitPort(win->UserPort);
                }while((item=HandleMenus())==NONE);
blea:;
                switch(item){
                        case _QUIT:GoodBye("Okay. I hope you have liked me...");

                        case _START : {
                                InitDraw(CLS);
                                item=DrawMand(0,0,width-1,height-1);
                                if(item==_QUIT) GoodBye("Already tired ? Try buying a Cray XM-P... (!!!)");
                                if(item!=NONE) goto blea;
                                break;
                        }

                        case _ZOOM_OUT : { ZoomOUT();   break; }

                        case _HIRES     : { Hires();            break;  }
                        case _LACE      : { Lace();             break;  }
                        case _H_OVERSCAN: { H_Overscan();       break;  }
                        case _V_OVERSCAN: { V_Overscan();       break;  }
                }
        }
}
