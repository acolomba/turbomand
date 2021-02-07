#include <exec/types.h>
#include <exec/memory.h>
#include <graphics/gfxmacros.h>
#include <intuition/intuition.h>
#include <proto/exec.h>
#include <proto/graphics.h>
#include <proto/mathffp.h>
#include <proto/mathieeedoubbas.h>
#include "hdrs/areamenu.d.h"
#include "hdrs/areamenu.p.h"
#include "hdrs/intuimsgs.p.h"
#include "hdrs/mand.p.h"
#include "hdrs/mand.v.h"
#include "hdrs/menuhand.d.h"
#include "hdrs/startend.v.h"
/*------------------------------------------------------------------------*/
AREA    dummy,
        *area=&dummy;
/*------------------------------------------------------------------------*/
VOID ZoomIN(VOID)
{
        LONG x1,y1,x2,y2;

        SelectArea(&x1,&y1,&x2,&y2);

        newieeeleft  =ieeep[x1];
        newieeeright =ieeep[x2];
        newieeebottom=ieeeq[y1];
        newieeetop   =ieeeq[y2];

        newffpleft  =ffpp[x1];
        newffpright =ffpp[x2];
        newffpbottom=ffpq[y1];
        newffptop   =ffpq[y2];

        newintleft  =intp[x1];
        newintright =intp[x2];
        newintbottom=intq[y1];
        newinttop   =intq[y2];
}

VOID ZoomOUT(VOID)
{
        PopArea();
}

/* A Lattice (I think) bug has obliged me to put it in menuhand.c
*
*       VOID Compute(VOID)
*       {
*               LONG x1,y1,x2,y2;
*
*               SelectArea(&x1,&y1,&x2,&y2);
*               (VOID)DrawMand(x1,y1,x2,y2);
*       }
*/

/*-------------------------------------------------------------------------*/
VOID SelectArea(LONG *x1, LONG *y1, LONG *x2, LONG *y2)
{
        ULONG class;
        UWORD code;
        UWORD mousex,mousey;
        BOOL  _1stpair=FALSE;

        *x1=win->MouseX;
        *y1=win->MouseY;

        do{
                WaitPort(win->UserPort);
                (VOID)GetIMsg(win,&class,&code,&mousex,&mousey);

                switch(class){
                        case MOUSEBUTTONS:{
                                switch(code){
                                        case SELECTDOWN: _1stpair=TRUE;
                                }
                                break;
                        }
                        case MOUSEMOVE:{
                                *x1=mousex;
                                *y1=mousey;
                                break;
                        }
                }
        }while(!_1stpair);

        *x2=*x1;*y2=*y1;

        SetAPen(rp,0x1F);
        SetDrMd(rp,JAM1|COMPLEMENT);
        WritePixel(rp,*x1,*y1);

        for(;;){
                WaitPort(win->UserPort);
                GetIMsg(win,&class,&code,&mousex,&mousey);

                switch(class){
                        case MOUSEBUTTONS:{
                                switch(code){
                                        case SELECTUP:{
                                                DrawRect(rp,*x1,*y1,*x2,*y2);
                                                SetDrMd(rp,JAM1);
                                                return;
                                        }
                                }
                                break;
                        }
                        case MOUSEMOVE:{
                                DrawRect(rp,*x1,*y1,*x2,*y2);
                                *x2=mousex<*x1+2 ? *x1+2 : mousex;
                                *y2=mousey<*y1+2 ? *y1+2 : mousey;
                                DrawRect(rp,*x1,*y1,*x2,*y2);
                                break;
                        }
                }
        }
}


VOID DrawRect(struct RastPort *rp, LONG x1, LONG y1, LONG x2, LONG y2)
{
        Move(rp,x1,y1);
        Draw(rp,x2,y1);
        Draw(rp,x2,y2);
        Draw(rp,x1,y2);
        Draw(rp,x1,y1);
}

VOID PushArea(BOOL cond)
{
        if(cond==HOWEVER
        || cond==IFNECESSARY && (  ieeeleft  !=newieeeleft
                                || ieeeright !=newieeeright
                                || ieeebottom!=newieeebottom
                                || ieeetop   !=newieeetop)
        ){
                AddArea();

                area->ieeeleft  =ieeeleft;
                area->ieeeright =ieeeright;
                area->ieeebottom=ieeebottom;
                area->ieeetop   =ieeetop;
                area->ieeer=ieeer;
                area->ieees=ieees;
                area->ieeeman=ieeeman;

                area->ffpleft  =ffpleft;
                area->ffpright =ffpright;
                area->ffpbottom=ffpbottom;
                area->ffptop   =ffptop;
                area->ffpr=ffpr;
                area->ffps=ffps;
                area->ffpman=ffpman;

                area->intleft  =intleft;
                area->intright =intright;
                area->intbottom=intbottom;
                area->inttop   =inttop;
                area->intr=intr;
                area->ints=ints;

                area->iters=iters;
                area->it4cl=it4cl;
        }
}

VOID PopArea(VOID)
{
        newieeeleft  =ieeeleft  =area->ieeeleft;
        newieeeright =ieeeright =area->ieeeright;
        newieeebottom=ieeebottom=area->ieeebottom;
        newieeetop   =ieeetop   =area->ieeetop;
        ieeer=area->ieeer;
        ieees=area->ieees;
        ieeeman=area->ieeeman;

        newffpleft  =ffpleft  =area->ffpleft;
        newffpright =ffpright =area->ffpright;
        newffpbottom=ffpbottom=area->ffpbottom;
        newffptop   =ffptop   =area->ffptop;
        ffpr=area->ffpr;
        ffps=area->ffps;
        ffpman=area->ffpman;

        newintleft  =intleft  =area->intleft;
        newintright =intright =area->intright;
        newintbottom=intbottom=area->intbottom;
        newinttop   =inttop   =area->inttop;
        intr=area->intr;
        ints=area->ints;

        iters=area->iters;
        it4cl=area->it4cl;

        RmArea();
}

VOID AddArea(VOID)
{
        area->Next=AllocMem(sizeof(AREA),MEMF_CLEAR);
        area->Next->Prev=area;
        area=area->Next;
}

VOID RmArea(VOID)
{
        if(area=area->Prev){
                FreeMem(area->Next,sizeof(AREA));
                area->Next=NULL;
        }
}
