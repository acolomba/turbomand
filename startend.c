#include <stdlib.h>
#include <exec/types.h>
#include <graphics/gfxbase.h>
#include <intuition/intuition.h>
#include <libraries/arpbase.h>
#include <libraries/dosextens.h>
#include <proto/dos.h>
#include <proto/exec.h>
#include <proto/graphics.h>
#include <proto/intuition.h>
#include "hdrs/areamenu.p.h"
#include "hdrs/areamenu.v.h"
#include "hdrs/startend.d.h"
#include "hdrs/startend.p.h"
#include "hdrs/font.d.h"
#include "hdrs/mand.v.h"
#include "hdrs/menus.d.h"
#include "hdrs/menus.v.h"
/*------------------------------------------------------------------------*/
GLOBAL struct Library           *MathBase,*MathIeeeDoubBasBase;

GLOBAL struct GfxBase           *GfxBase;
GLOBAL struct IntuitionBase     *IntuitionBase;
struct ArpBase  *ArpBase;

struct Process  *myproc;
APTR temp;

struct Screen   *scr,*xscr;
struct Window   *win;
struct RastPort *rp,*xrp;


BOOL    menu_on=FALSE;
WORD    width,height,depth,colors;
VIEWMODE viewmode;
WORD    stdwidth,stdheight;
/*------------------------------------------------------------------------*/
UWORD colormap[]={
        0x000,0x122,0x232,0x243,0x244,0x255,0x265,0x376,
        0x387,0x498,0x5A9,0x6BB,0x8CC,0xADD,0xCEE,0xFFF,
        0xDEF,0xACF,0x8BD,0x88B,0x69B,0x689,0x678,0x567,
        0x566,0x677,0x787,0x899,0x9CA,0xBEC,0xDFE,0xFFF
};

struct NewScreen nscr={
        0,0,                            /* Left Edge , Top Edge         */
        NULL,NULL,                      /* Width ,Height                */
        NULL,                           /* BitPlanes                    */
        0,0,                            /* Block Pen , Detail Pen       */
        NULL,                           /* Display Mode                 */
        CUSTOMSCREEN,                   /* Screen Type                  */
        NULL,                           /* Pointer to Custom Font       */
        "TurboMand by Alessandro Colomba",/* Pointer to Title Text       */
        NULL,                           /* Pointer to Screen Gadgets    */
        NULL                            /* Pointer to Custom BitMap     */
};

struct NewWindow nwin={
        0,0,                            /* Left Edge , Top Edge         */
        NULL,NULL,                      /* Width , Height               */
        0,1,                            /* Detail Pen , Block Pen       */
        MY_IDCMP,                       /* IDCMP Flags                  */
        SMART_REFRESH | ACTIVATE   |    /*      Window..                */
        NOCAREREFRESH | BORDERLESS |    /*              .               */
        REPORTMOUSE,                    /*              ..Flags         */
        NULL,                           /* Pointer to First Gadget      */
        NULL,                           /* Pointer to Check Mark Image  */
        NULL,                           /* Pointer to Title Text        */
        NULL,                           /* Pointer to Custom Screen     */
        NULL,                           /* Pointer to Custom BitMap     */
        320,200,                        /* Minimum  Width,Height        */
        -1,-1,                          /* Maximum  Width,Height        */
        CUSTOMSCREEN                    /* Type of screen it resides on */
};
/*------------------------------------------------------------------------*/
VOID OpenMath(VOID)
{
        MathBase=OpenLibrary("mathffp.library",MINVERSION);
        if(MathBase==NULL) GoodBye("Can't open ffp math library !!");

        MathIeeeDoubBasBase=OpenLibrary("mathieeedoubbas.library",MINVERSION);
        if(MathIeeeDoubBasBase==NULL) GoodBye("Can't open ieee math library !!");
}

VOID OpenArp(VOID)
{
        ArpBase=(struct ArpBase *)OpenLibrary("arp.library",ArpVersion);
        if(ArpBase==NULL) GoodBye("Can't open arp.library");

        GfxBase=(struct GfxBase *)ArpBase->GfxBase;
        if(GfxBase==NULL) GoodBye("Can't open graphics.library !!");

        IntuitionBase=ArpBase->IntuiBase;
        if(IntuitionBase==NULL) GoodBye("Can't open intuition.library !!");
}

VOID OpenScreens(VOID)
{
        InitNewScreen();
        OpenAuxScreen();
        OpenDrawScreen();
        OpenDrawWindow();
        SetMenus();
}

VOID InitNewScreen(VOID)
{
        width =stdwidth =GfxBase->NormalDisplayColumns;
        height=stdheight=GfxBase->NormalDisplayRows;

        if(viewmode.hires){
                nscr.ViewModes=HIRES;
                depth=4;
                colors=16;
                DspItm[HIRES_].Flags|=CHECKED;
        }else{
                width>>=1;
                nscr.ViewModes=NULL;
                depth=5;
                colors=32;
                DspItm[HIRES_].Flags&=~CHECKED;
        }

        if(viewmode.lace){
                height+=height;
                nscr.ViewModes|=LACE;
                DspItm[LACE_].Flags|=CHECKED;
        }else   DspItm[LACE_].Flags&=~CHECKED;

        if(viewmode.h_overscan){
                width+=OVERSCAN_W;
                if(viewmode.hires)
                        width+=OVERSCAN_W;
                DspItm[H_OVERSCAN].Flags|=CHECKED;
        }else   DspItm[H_OVERSCAN].Flags&=~CHECKED;


        if(viewmode.v_overscan){
                height+=OVERSCAN_H;
                if(viewmode.lace)
                        height+=OVERSCAN_H;
                DspItm[V_OVERSCAN].Flags|=CHECKED;
        }else   DspItm[V_OVERSCAN].Flags&=~CHECKED;

        nwin.Width =nscr.Width =width ;
        nwin.Height=nscr.Height=height;
}

VOID OpenAuxScreen(VOID)
{
        nscr.Depth=1;
        xscr=OpenScreen(&nscr);
        if(xscr==NULL) GoodBye("Can't open auxiliary screen !!");
        xrp=&xscr->RastPort;
        SetAPen(xrp,1);
}

VOID OpenDrawScreen(VOID)
{
        nscr.Depth=depth;
        nscr.Height=height;
        scr=(struct Screen *)OpenScreen(&nscr);
        if(scr==NULL) GoodBye("Can't open main screen !!");
        LoadRGB4(&scr->ViewPort,colormap,colors);
}

VOID OpenDrawWindow(VOID)
{
        nwin.Screen=scr;
        win=(struct Window *)OpenWindow(&nwin);
        if(win==NULL) GoodBye("Can't open window !!");
        rp=win->RPort;

        myproc=(struct Process *)FindTask(0L);
        temp=myproc->pr_WindowPtr;
        myproc->pr_WindowPtr=(APTR)win;
}

VOID SetMenus(VOID)
{
        ModifyIDCMP(win,MY_IDCMP | MN_IDCMP);
        RMBtrapOFF(win);
        SetMenuStrip(win,MyMenu);
        menu_on=TRUE;
}
/*------------------------------------------------------------------------*/
VOID GoodBye(STRPTR message)
{
/*
**      printf("\n%s\nTurbo Mandelbrot 0.98a ? 1989 Public Domain by Alessandro Colomba\n\n\a",message);
*/
        DropAreas();
        CloseMath();
        CloseScreens();
        CloseArp();

        Exit(0);
}

VOID DropAreas(VOID)
{
        while(area->Prev)
                RmArea();
}

VOID CloseMath(VOID)
{
        if(MathBase)            CloseLibrary(MathBase);
        if(MathIeeeDoubBasBase) CloseLibrary(MathIeeeDoubBasBase);
}

VOID CloseScreens(VOID)
{
        CloseAuxScreen();
        UnsetMenus();
        CloseDrawWindow();
        CloseDrawScreen();

        if(ieeep){      free(ieeep);    ieeep=NULL;     }
        if(ieeeq){      free(ieeeq);    ieeeq=NULL;     }
        if(ffpp){       free(ffpp);     ffpp=NULL;      }
        if(ffpq){       free(ffpq);     ffpq=NULL;      }
        if(intp){       free(intp);     intp=NULL;      }
        if(intq){       free(intq);     intq=NULL;      }
}

VOID CloseAuxScreen(VOID)
{
        if(xscr) CloseScreen(xscr);
}

VOID UnsetMenus(VOID)
{
        if(menu_on){
                ClearMenuStrip(win);
                ModifyIDCMP(win,MY_IDCMP);
                RMBtrapON(win);
                menu_on=FALSE;
        }
}

VOID CloseDrawWindow(VOID)
{
        if(temp){
                myproc->pr_WindowPtr=temp;
        }

        if(win){
                CloseWindow(win);
        }
}

VOID CloseDrawScreen(VOID)
{
        if(scr) CloseScreen(scr);
}

VOID CloseArp(VOID)
{
        if(ArpBase) CloseLibrary((struct Library *)ArpBase);
}


