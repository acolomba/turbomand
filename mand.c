#include <stdlib.h>
#include <exec/types.h>
#include <graphics/rastport.h>
#include <proto/graphics.h>
#include "hdrs/mand.d.h"
#include "hdrs/mand.p.h"
#include "hdrs/menuhand.d.h"
#include "hdrs/menuhand.p.h"
#include "hdrs/startend.v.h"
#include "hdrs/areamenu.d.h"
#include "hdrs/areamenu.p.h"
/*------------------------------------------------------------------------*/
LONG pixels;
WORD colmod;
LONG iters=_1stITERS,
     it4cl=_1stIT4CL;
WORD math;
BOOL slow=TRUE;

#if defined NO_LATTICE_BUG
        LONG __asm (*Mand)(REGISTER __d0 LONG, REGISTER __d1 LONG);
#else
        LONG (* Mand)(LONG, LONG);
#endif
/*------------------------------------------------------------------------*/
VOID SetMand(SHORT mathy)
{
        switch(mathy){
                case IEEEMATH:{ Mand=IEEEMand;  break; }
                case FFPMATH :{ Mand=FFPMand ;  break; }
                case INTMATH :{ Mand=INTMand ;  break; }
                case FASTMAND:{ slow=!slow;     break; }
        }

        math=mathy;
        SetRast(xrp,0);
}

VOID InitDraw(BOOL cls)
{
        PushArea(IFNECESSARY);
        if(cls==CLS){
                SetRast(xrp,0);
                SetRast(rp,0);
        }
        pixels=0;
        colmod=colors;
        colmod--;

        InitIEEEDraw();
        InitFFPDraw();
        InitINTDraw();
}
/*------------------------------------------------------------------------*/
ITEM __asm DrawMand(REGISTER __d0 LONG xl, REGISTER __d1 LONG yl,
                    REGISTER __d2 LONG xu, REGISTER __d3 LONG yu)
{
        ITEM item=NONE;

        item=HandleMenus();
        if(item!=NONE)
                return(item);

        if(!sides(xl,yl,xu,yu)){
                if(xu-xl<4 || yu-yl<4){
                        REGISTER LONG i,j;

                        for(i=xl;i<=xu;i++)
                                for(j=yl;j<=yu;j++)
                                        (VOID)Mand(i,j);
                }else{
                        ITEM item;
                        REGISTER LONG   mx=(xl+xu)>>1,
                                        my=(yl+yu)>>1;

                        item=   DrawMand(xl,yl,mx,my);
                         if(item!=NONE) return(item);
                        item=   DrawMand(mx,yl,xu,my);
                         if(item!=NONE) return(item);
                        item=   DrawMand(xl,my,mx,yu);
                         if(item!=NONE) return(item);
                        item=   DrawMand(mx,my,xu,yu);
                         if(item!=NONE) return(item);
        }       }
        return(item);
}
/*------------------------------------------------------------------------*/
BOOL __asm sides(REGISTER __d0 LONG xl, REGISTER __d1 LONG yl,
                 REGISTER __d2 LONG xu, REGISTER __d3 LONG yu)
{
        REGISTER LONG x,y;
        LONG k;

        if((k=Mand(xl,yl))!=Mand(xu,yu) || Mand(xl,yu)!=k || Mand(xu,yl)!=k)
                return(FALSE);

        if(slow){
                for(x=xl,x++;x<xu;x++)
                        if(Mand(x,yl)!=k || Mand(x,yu)!=k)
                                return(FALSE);
                for(y=yl,y++;y<yu;y++)
                        if(Mand(xl,y)!=k || Mand(xu,y)!=k)
                                return(FALSE);
        }else   DrawRect(xrp,xl,yl,xu,yu);


        if(k){
                SetAPen(rp,k);
                RectFill(rp,xl,yl,xu,yu);
        }
        return(TRUE);
}
