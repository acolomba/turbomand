#define INTUITION_PREFERENCES_H         /* A little trick in order */
                                        /* to avoid including it   */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <exec/types.h>
#include <intuition/intuition.h>
#include <libraries/arpbase.h>
#include <proto/arp.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/graphics.h>
#include <proto/intuition.h>
#include "hdrs/prjmenu.d.h"
#include "hdrs/prjmenu.p.h"
#include "hdrs/atof.p.h"
#include "hdrs/intuimsgs.p.h"
#include "hdrs/loadiff.p.h"
#include "hdrs/saveiff.p.h"
#include "hdrs/mand.p.h"
#include "hdrs/mand.v.h"
#include "hdrs/startend.d.h"
#include "hdrs/startend.v.h"
#include "hdrs/stuff.d.h"
#include "hdrs/stuff.p.h"
#include "hdrs/areamenu.p.h"
/*------------------------------------------------------------------------*/
GLOBAL struct ArpBase *ArpBase;
/*------------------------------------------------------------------------*/
TEXT filename[FCHARS+1];
TEXT dirname[DSIZE+1];
STRPTR loadstring="Select a file";
STRPTR savestring="Enter name of file to save";

struct FileRequester filereq={
        NULL,                   /* Hailing text                 */
        filename,               /* Filename array (FCHARS * N)  */
        dirname,                /* Directory array (DSIZE + 1)  */
        NULL,                   /* Window requesting files      */
        FRF_NewWindFunc,        /* Flags                        */
        NULL,                   /* Reserved !!                  */
        NULL,                   /* Special function             */
        NULL                    /* Reserved !!                  */
};

VOID ReqFunc(ULONG mask,CPTR *obj)
{
        switch(mask){
                case FRF_NewWindFunc:{
                        if(!viewmode.hires)
                                ((struct NewWindow *)obj)->LeftEdge=10;
                        break;
                }
        }
        SetWorkPalette();
}
/*------------------------------------------------------------------------*/
TEXT    Gadbuf[IT4CL_GAD][FLOAT_CHARS],
        itersbuf[ITERS_CHARS],
        it4clbuf[IT4CL_CHARS];
/*------------------------------------------------------------------------*/
BOOL Load(VOID)
{
        filereq.fr_Hail=loadstring;
        filereq.fr_FuncFlags&=~FRF_DoColor;
        filereq.fr_Window=win;
        filereq.fr_Function=(VOID(*)())ReqFunc;

        if(FileRequest(&filereq)){
                TEXT file[DSIZE+FCHARS+2];

                strcpy(file,dirname);
                TackOn(file,filename);

                UnsetWorkPalette();

                LoadIFF(file);
                InitDraw(NOCLS);

                return(TRUE);
        }

        UnsetWorkPalette();
        return(FALSE);
}

VOID Save(VOID)
{
        filereq.fr_Hail=savestring;
        filereq.fr_FuncFlags|=FRF_DoColor;
        filereq.fr_Window=win;
        filereq.fr_Function=(VOID(*)())ReqFunc;

        if(FileRequest(&filereq)){
                TEXT file[DSIZE+FCHARS+2];

                strcpy(file,dirname);
                TackOn(file,filename);

                UnsetWorkPalette();

                SaveIFF(file);
        }

        UnsetWorkPalette();
}
/*------------------------------------------------------------------------*/
WORD GadXY[]={
                0,              0 ,
    FLOAT_WIDTH+2,              0 ,
    FLOAT_WIDTH+2,FONT_HEIGHT+2*2 ,
                0,FONT_HEIGHT+2*2 ,
                0,              0 
};

WORD ItersXY[]={
                0,              0 ,
    ITERS_WIDTH+2,              0 ,
    ITERS_WIDTH+2,FONT_HEIGHT+2*2 ,
                0,FONT_HEIGHT+2*2 ,
                0,              0 
};

WORD It4clXY[]={
                0,              0 ,
    IT4CL_WIDTH+2,              0 ,
    IT4CL_WIDTH+2,FONT_HEIGHT+2*2 ,
                0,FONT_HEIGHT+2*2 ,
                0,              0 
};

struct Border GadBorder={
        -1,-2,          /* Left Edge, Top Edge  */
        2,0,            /* Front Pen, Back Pen  */
        JAM1,           /* Draw Mode            */
        5,              /* Number of vertices+1 */
        GadXY,          /* Pointer to vertices  */
        NULL            /* Pointer to next strct*/
};

struct Border ItersBorder={
        -1,-2,
        2,0,
        JAM1,
        5,
        ItersXY,
        NULL
};

struct Border It4clBorder={
        -1,-2,
        2,0,
        JAM1,
        5,
        It4clXY,
        NULL
};

struct IntuiText GadText[]={
        GADTEXT(LEFT_GAD  ,"Left      : "),
        GADTEXT(RIGHT_GAD ,"Right     : "),
        GADTEXT(BOTTOM_GAD,"Bottom    : "),
        GADTEXT(TOP_GAD   ,"Top       : "),
        GADTEXT(R_GAD     ,"Const R   : "),
        GADTEXT(S_GAD     ,"Const I   : "),
        GADTEXT(M_GAD     ,"Mand      : "),
        GADTEXT(ITERS_GAD ,"Iters     : "),
        GADTEXT(IT4CL_GAD ,"Iters/Col : ")
};

struct StringInfo GadInfo[]={
        STRING(LEFT_GAD),
        STRING(RIGHT_GAD),
        STRING(BOTTOM_GAD),
        STRING(TOP_GAD),
        STRING(R_GAD),
        STRING(S_GAD),
        STRING(M_GAD),
        {
                itersbuf,               /* Pointer to Input String      */
                NULL,                   /* Pointer to Undo String       */
                3,                      /* Cursor Initial Position      */
                ITERS_CHARS+1,          /* 65535 is the max iters number*/
                0                       /* Pos of First Displayed Char  */
        },
        {
                it4clbuf,               /* Pointer to Input String      */
                NULL,                   /* Pointer to Undo String       */
                3,                      /* Cursor Initial Position      */
                IT4CL_CHARS+1,          /* 65535 is the max iters number*/
                0                       /* Pos of First Displayed Char  */
        }
};

struct Gadget MyGad[]={
        STRGAD(LEFT_GAD),
        STRGAD(RIGHT_GAD),
        STRGAD(BOTTOM_GAD),
        STRGAD(TOP_GAD),
        STRGAD(R_GAD),
        STRGAD(S_GAD),
        STRGAD(M_GAD),
        {
                &MyGad[ITERS_GAD+1],    /* Pointer to Next Gadget       */
                XGAP+TXTW*FONT_WIDTH,ITERS_TOP, /* Left Edge , Top Edge */
                ITERS_WIDTH,FONT_HEIGHT,/* Width , Height               */
                GADGHCOMP,              /* Flags                        */
                STRINGRIGHT|RELVERIFY,  /* Activation                   */
                STRGADGET,              /* Gadget Type                  */
                (APTR)&ItersBorder,     /* Pointer to Gadget Render     */
                NULL,                   /* Pointer to Select Render     */
                &GadText[ITERS_GAD],    /* Pointer to Gadget IntuiText  */
                NULL,                   /* Mutual Exclude               */
                (APTR)&GadInfo[ITERS_GAD],/* Special Info               */
                ITERS_GAD,              /* Gadget ID                    */
                NULL                    /* Pointer to Special Data      */
        },
        {
                NULL,                   /* Pointer to Next Gadget       */
                XGAP+TXTW*FONT_WIDTH,IT4CL_TOP, /* Left Edge , Top Edge */
                IT4CL_WIDTH,FONT_HEIGHT,/* Width , Height               */
                GADGHCOMP,              /* Flags                        */
                STRINGRIGHT|RELVERIFY,  /* Activation                   */
                STRGADGET,              /* Gadget Type                  */
                (APTR)&It4clBorder,     /* Pointer to Gadget Render     */
                NULL,                   /* Pointer to Select Render     */
                &GadText[IT4CL_GAD],    /* Pointer to Gadget IntuiText  */
                NULL,                   /* Mutual Exclude               */
                (APTR)&GadInfo[IT4CL_GAD],/* Special Info               */
                IT4CL_GAD,              /* Gadget ID                    */
                NULL                    /* Pointer to Special Data      */
        }
};

struct NewWindow newnwin={
        10,10,                          /* Left Edge , Top Edge         */
        NWIN_W,NWIN_H,                  /* Width , Height               */
        0,1,                            /* Detail Pen , Block Pen       */
        GADGETUP | CLOSEWINDOW,         /*      IDCMP Flags             */
        WINDOWDRAG    | ACTIVATE      | /*      Window..                */
        SMART_REFRESH | NOCAREREFRESH | /*              ..              */
        WINDOWCLOSE   ,                 /*                ..Flags       */
        MyGad,                          /* Pointer to First Gadget      */
        NULL,                           /* Pointer to Check Mark Image  */
        "Insert new parameters",        /* Pointer to Title Text        */
        NULL,                           /* Pointer to Custom Screen     */
        NULL,                           /* Pointer to Custom BitMap     */
        NWIN_W,NWIN_H,                  /* Minimum  Width,Height        */
        NWIN_W,NWIN_H,                  /* Maximum  Width,Height        */
        CUSTOMSCREEN                    /* Type of screen it resides on */
};
/*------------------------------------------------------------------------*/
BOOL New(VOID)
{
        struct Window *nwin;
        ULONG class;
        UWORD code;
        UWORD mousex,mousey;

        DOUBLE  oldieeeman=ieeeman;
        LONG    olditers=iters,
                oldit4cl=it4cl;

        Params2Str();

        SetWorkPalette();

        newnwin.Screen=scr;
        nwin=OpenWindow(&newnwin);
        if(nwin==NULL) return(FALSE);

        for(;;){
                WaitPort(nwin->UserPort);
                GetIMsg(nwin,&class,&code,&mousex,&mousey);

                switch(class){
                        case CLOSEWINDOW:{
                                Str2Params();

                                if(ieeer!=newieeer || ieees!=newieees
                                || ieeeman!=oldieeeman
                                || iters!=olditers || it4cl!=oldit4cl)
                                        SetRast(xrp,0);

                                CloseWindow(nwin);
                                UnsetWorkPalette();

                                return(TRUE);
                                break;
                        }
                }
        }
}
/*------------------------------------------------------------------------*/
VOID Params2Str(VOID)
{
        sprintf(Gadbuf[LEFT_GAD]  ,FF,ieeeleft);
        sprintf(Gadbuf[RIGHT_GAD] ,FF,ieeeright);
        sprintf(Gadbuf[BOTTOM_GAD],FF,ieeebottom);
        sprintf(Gadbuf[TOP_GAD]   ,FF,ieeetop);
        sprintf(Gadbuf[R_GAD]     ,FF,ieeer);
        sprintf(Gadbuf[S_GAD]     ,FF,ieees);
        sprintf(Gadbuf[M_GAD]     ,FF,ieeeman);
        sprintf(itersbuf,"%d"     ,iters);
        sprintf(it4clbuf,"%d"     ,it4cl);
}

VOID Str2Params(VOID)
{
        newieeeleft  =atof(Gadbuf[LEFT_GAD]);
        newieeeright =atof(Gadbuf[RIGHT_GAD]);
        newieeebottom=atof(Gadbuf[BOTTOM_GAD]);
        newieeetop   =atof(Gadbuf[TOP_GAD]);
        newieeer     =atof(Gadbuf[R_GAD]);
        newieees     =atof(Gadbuf[S_GAD]);
        ieeeman      =atof(Gadbuf[M_GAD]);

        newffpleft  =ffpatof(Gadbuf[LEFT_GAD]);
        newffpright =ffpatof(Gadbuf[RIGHT_GAD]);
        newffpbottom=ffpatof(Gadbuf[BOTTOM_GAD]);
        newffptop   =ffpatof(Gadbuf[TOP_GAD]);
        newffpr     =ffpatof(Gadbuf[R_GAD]);
        newffps     =ffpatof(Gadbuf[S_GAD]);
        ffpman      =ffpatof(Gadbuf[M_GAD]);

        newintleft  =ieee2int(newieeeleft);
        newintright =ieee2int(newieeeright);
        newintbottom=ieee2int(newieeebottom);
        newinttop   =ieee2int(newieeetop);
        newintr     =ieee2int(newieeer);
        newints     =ieee2int(newieees);

        iters=atoi(itersbuf);
        it4cl=atoi(it4clbuf);

}
/*------------------------------------------------------------------------*/
#define AT_LINES        15
#define REAL_LINES      16
#define ABOUTW_W        ((32+2)*FONT_WIDTH)
#define ABOUTW_H        (BARGAP+REAL_LINES*FONT_HEIGHT)

struct NewWindow aboutnwin={
        NULL,NULL,                      /* Left Edge , Top Edge         */
        ABOUTW_W,ABOUTW_H,              /* Width , Height               */
        0,0,                            /* Detail Pen , Block Pen       */
        MOUSEBUTTONS,                   /*      IDCMP Flags             */
        ACTIVATE      | SMART_REFRESH | /*      Window..                */
        NOCAREREFRESH ,                 /*                ..Flags       */
        NULL,                           /* Pointer to First Gadget      */
        NULL,                           /* Pointer to Check Mark Image  */
        NULL,                           /* Pointer to Title Text        */
        NULL,                           /* Pointer to Custom Screen     */
        NULL,                           /* Pointer to Custom BitMap     */
        20,20,                          /* Minimum  Width,Height        */
        -1,-1,                          /* Maximum  Width,Height        */
        CUSTOMSCREEN                    /* Type of screen it resides on */
};

MYTEXT AboutText[AT_LINES]={
        {  1, 2, " Turbo Mandelbrot Set Generator "     },
        {  2, 3, "          Version  "                  },
        {  2, 2, "                  1.00"               },
        {  3, 3, " (C)1988-1992 "                               },
        {  3, 2, "              Alessandro Colomba"     },
        {  5, 2, "  Integer mathematics due to:"        },
        {  6, 3, "    Hannu \"The DM\" Helminen"        },
        {  8, 3, " Moral support: "                     },
        {  8, 2, "                Fabrizio Labate"      },
        {  9, 2, "                Enzo Marra"           },
        { 11, 3, "This is a               program."     },
        { 11, 2, "          Public Domain"              },
        { 13, 3, " E-mail:"                             },
        { 13, 2, "          flynn@147.162.2.101"        },
        { 14, 3, "         (alessia.dei.unipd.it)"      }
};
/*------------------------------------------------------------------------*/
VOID About(VOID)
{
        struct Window *aboutwin;
        struct RastPort *arp;
        struct IntuiMessage *message;

        aboutnwin.LeftEdge=(width -ABOUTW_W) >> 1;
        aboutnwin.TopEdge =(height-ABOUTW_H) >> 1;
        aboutnwin.Screen=scr;

        SetWorkPalette();
        aboutwin=OpenWindow(&aboutnwin);

        if(aboutwin){
                REGISTER LONG i;

                arp=aboutwin->RPort;
                SetRast(arp,1);
                SetAPen(arp,0);
                DrawRect(arp,2,2,aboutwin->Width-3,aboutwin->Height-3);
                DrawRect(arp,4,4,aboutwin->Width-5,aboutwin->Height-5);
                SetDrMd(arp,JAM1);

                for(i=0;i<AT_LINES;i++){
                        Move(arp,FONT_WIDTH,Y2CH(AboutText[i].row));
                        SetAPen(arp,AboutText[i].pen);
                        Text(arp,AboutText[i].text,strlen(AboutText[i].text));
                }

                do {
                        WaitPort(aboutwin->UserPort);
                        message=(struct IntuiMessage *)
                                GetMsg(aboutwin->UserPort);
                } while(message->Class!=MOUSEBUTTONS);

                CloseWindow(aboutwin);
        }

        UnsetWorkPalette();
}
