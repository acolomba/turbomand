#include <intuition/intuition.h>
#include <proto/exec.h>
#include <proto/graphics.h>
#include <proto/intuition.h>
#include "hdrs/mand.d.h"
#include "hdrs/mand.p.h"
#include "hdrs/areamenu.p.h"
#include "hdrs/intuimsgs.p.h"
#include "hdrs/menuhand.d.h"
#include "hdrs/menuhand.p.h"
#include "hdrs/menus.d.h"
#include "hdrs/menus.v.h"
#include "hdrs/startend.p.h"
#include "hdrs/startend.v.h"
#include "hdrs/prjmenu.p.h"
#include "hdrs/stuff.p.h"
/*------------------------------------------------------------------------*/
ITEM HandleMenus(VOID)
{
        ITEM item=NONE;
        ULONG class;
        UWORD code;
        UWORD mousex,mousey;

        while(GetIMsg(win,&class,&code,&mousex,&mousey)){
                switch(class){
                        case MENUPICK:{
                                if(code!=MENUNULL){
                                        switch(MENUNUM(code)){
                                                case PRJ: {
                                                        UnsetMenus();
                                                        item=HandlePrj(code);
                                                        SetMenus();
                                                        break;
                                                }
                                                case DRW: {
                                                        UnsetMenus();
                                                        item=HandleDrw(code);
                                                        SetMenus();
                                                        break;
                                                }
                                                case _AREA: {
                                                        item=HandleArea(code);
                                                        break;
                                                }
                                                case DSP: {
                                                        item=HandleDsp(code);
                                                        break;
                                                }
/*
*                                                       case COL: {
*                                                               item=HandleCol(code);
*                                                               break;
*                                                       }
*/
                                        }
                                }
                                else    UnsetWorkPalette();
                                break;
                        }
                        case MENUVERIFY: {
                                WaitPort(win->UserPort);
                                item=HandleMenus();
                                break;
                        }
                }
        }

        return(item);
}
/*-------------------------------------------------------------------------*/
ITEM HandlePrj(UWORD code)
{
        ITEM item=NONE;

        switch(ITEMNUM(code)){
                case ABOUT: {
                                About();
                                UnsetWorkPalette();
                                break;
                }
                case NEW  : {
                                New();
                                UnsetWorkPalette();
                                break;
                }
                case LOAD : {
                                UnsetWorkPalette();
                                if(Load()) item=_STOP;
                                break;
                }
                case SAVE : {
                                UnsetWorkPalette();
                                Save();
                                break;
                }
                case QUIT : {
                                item=_QUIT;
                                UnsetWorkPalette();
                                break;
                }
        }

        return(item);
}

ITEM HandleDrw(UWORD code)
{
        ITEM item=NONE;

        UnsetWorkPalette();

        switch(ITEMNUM(code)){
                case START: { item=_START;      break;  }
                case STOP : { item=_STOP;       break;  }
                case IEEE : { SetMand(IEEEMATH);break;  }
                case FFP  : { SetMand(FFPMATH); break;  }
                case INT  : { SetMand(INTMATH); break;  }
                case FAST : { SetMand(FASTMAND);break;  }
        }

        return(item);
}

ITEM HandleArea(UWORD code)
{
        ITEM item=NONE;

        UnsetWorkPalette();

        switch(ITEMNUM(code)){
                case ZOOM_IN : { UnsetMenus(); ZoomIN(); SetMenus(); break; }
                case ZOOM_OUT: { item=_ZOOM_OUT; break; }
                case COMPUTE : {
                        LONG x1,y1,x2,y2;

                        UnsetMenus();
                        SelectArea(&x1,&y1,&x2,&y2);

                        SetAPen(rp,0);
                        RectFill(rp,x1,y1,x2,y2);

                        SetAPen(xrp,0);
                        RectFill(xrp,x1,y1,x2,y2);
                        SetAPen(xrp,1);

                        SetMenus();

                        item=DrawMand(x1,y1,x2,y2);

                        if(item==_STOP)
                                item=NONE;
                        else    RectFill(xrp,x1,y1,x2,y2);

                        break;
                }
        }

        return(item);
}

ITEM HandleDsp(UWORD code)
{
        ITEM item=NONE;

        UnsetWorkPalette();
        switch(ITEMNUM(code)){
                case HIRES_    : { item=_HIRES;         break;  }
                case LACE_     : { item=_LACE;          break;  }
                case H_OVERSCAN: { item=_H_OVERSCAN;    break;  }
                case V_OVERSCAN: { item=_V_OVERSCAN;    break;  }
        }

        return(item);
}

/*
*       ITEM HandleCol(UWORD code)
*       {
*               ITEM item=NONE;
*
*               UnsetWorkPalette();
*               switch(ITEMNUM(code)){
*                       case PALETTE: break;
*                       case ASSIGN : break;
*               }
*
*               return(item);
*       }
*/
