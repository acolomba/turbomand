#include <intuition/intuition.h>
#include "hdrs/menus.d.h"
#include "hdrs/font.d.h"
/*------------------------------------------------------------------------*/
#define PrjLEFT         0
#define PrjNAME         "Project"
#define PrjWIDTH        ((7+MENUGAP)*FONT_WIDTH)
#define PrjITMWIDTH     ((5+ITEMGAP)*FONT_WIDTH)

#define DrwLEFT         (PrjWIDTH)
#define DrwNAME         "Drawing"
#define DrwWIDTH        ((7+MENUGAP)*FONT_WIDTH)
#define DrwITMWIDTH     ((5+ITEMGAP)*FONT_WIDTH+CHECKWIDTH)

#define AreaLEFT        (DrwLEFT+DrwWIDTH)
#define AreaNAME        "Area"
#define AreaWIDTH       ((4+MENUGAP)*FONT_WIDTH)
#define AreaITMWIDTH    ((8+ITEMGAP)*FONT_WIDTH)

#define DspLEFT         (AreaLEFT+AreaWIDTH)
#define DspNAME         "Display"
#define DspWIDTH        ((7+MENUGAP)*FONT_WIDTH)
#define DspITMWIDTH     ((10+ITEMGAP)*FONT_WIDTH+CHECKWIDTH)

#define ColLEFT         (DspLEFT+DspWIDTH)
#define ColNAME         "Colors"
#define ColWIDTH        ((6+MENUGAP)*FONT_WIDTH)
#define ColITMWIDTH     ((7+ITEMGAP)*FONT_WIDTH)
/*------------------------------------------------------------------------*/
#define u(a,b)  a##b
#define INTUITXT(str,LEFT)      { 0,1,JAM1,LEFT+1,0,NULL,str,NULL }

#define ITEM(Menu,ITEM,flags,me,key,LEFT) {                                     \
        &u(Menu,Itm)[ITEM+1],                                           \
        LEFT*FONT_WIDTH,ITEM*FONT_HEIGHT,                               \
        u(Menu,ITMWIDTH)+COMMWIDTH,FONT_HEIGHT,                         \
        ITEMTEXT|COMMSEQ|ITEMENABLED|HIGHCOMP|flags,                    \
        me,(APTR)&u(Menu,Txt)[ITEM],                                    \
        NULL,                                                           \
        key                                                             \
}
#define LASTITEM        { NULL,0,0,0,0,NULL,0,(APTR)&NullTxt,NULL,0 }

#define MENU(MENU,menu) {                                               \
        &Menu[MENU+1],                                                  \
        u(menu,LEFT),0,u(menu,WIDTH),                                   \
        FONT_HEIGHT+1,MENUENABLED,                                      \
        u(menu,NAME),                                                   \
        u(menu,Itm)                                                     \
}
#define LASTMENU        { NULL,0,0,0,0,NULL,NULL,0,0 }
/*------------------------------------------------------------------------*/
struct IntuiText PrjTxt[]={
        INTUITXT("About",0),
        INTUITXT("New  ",0),
        INTUITXT("Load ",0),
        INTUITXT("Save ",0),
        INTUITXT("Quit ",0)
};
struct IntuiText DrwTxt[]={
        INTUITXT("Start",0),
        INTUITXT("Stop ",0),
        INTUITXT("IEEE ",CHECKWIDTH),
        INTUITXT("FFP  ",CHECKWIDTH),
        INTUITXT("INT  ",CHECKWIDTH),
        INTUITXT("FAST ",CHECKWIDTH)
};
struct IntuiText AreaTxt[]={
        INTUITXT("Zoom IN ",0),
        INTUITXT("Zoom OUT",0),
        INTUITXT("Compute ",0)
};
struct IntuiText DspTxt[]={
        INTUITXT("Hires     ",CHECKWIDTH),
        INTUITXT("Lace      ",CHECKWIDTH),
        INTUITXT("H Overscan",CHECKWIDTH),
        INTUITXT("V Overscan",CHECKWIDTH)
};

/*
*       struct IntuiText ColTxt[]={
*               INTUITXT("Palette",0),
*               INTUITXT("Assign ",0),
*       };
*/

struct IntuiText NullTxt={ 0,0,0,0,0,NULL,NULL,NULL };

struct MenuItem PrjItm[]={
        ITEM(Prj,ABOUT,NULL,NULL,'A',0),
        ITEM(Prj,NEW  ,NULL,NULL,'N',0),
        ITEM(Prj,LOAD ,NULL,NULL,'O',0),
        ITEM(Prj,SAVE ,NULL,NULL,'W',0),
        ITEM(Prj,QUIT ,NULL,NULL,'Q',0),
        LASTITEM
};
struct MenuItem DrwItm[]={
        ITEM(Drw,START,NULL,NULL,'S',0),
        ITEM(Drw,STOP ,NULL,NULL,'B',0),
        ITEM(Drw,IEEE ,CHECKIT|MENUTOGGLE        ,(1<<FFP )|(1<<INT),'1',0),
        ITEM(Drw,FFP  ,CHECKIT|MENUTOGGLE        ,(1<<IEEE)|(1<<INT),'2',0),
        ITEM(Drw,INT  ,CHECKIT|MENUTOGGLE|CHECKED,(1<<IEEE)|(1<<FFP),'3',0),
        ITEM(Drw,FAST ,CHECKIT|MENUTOGGLE|CHECKED,0,'9',0),
        LASTITEM
};
struct MenuItem AreaItm[]={
        ITEM(Area,ZOOM_IN ,NULL,NULL,'I',0),
        ITEM(Area,ZOOM_OUT,NULL,NULL,'U',0),
        ITEM(Area,COMPUTE ,NULL,NULL,'C',0),
        LASTITEM
};
struct MenuItem DspItm[]={
        ITEM(Dsp,HIRES_    ,CHECKIT|MENUTOGGLE,NULL,'H',-4),
        ITEM(Dsp,LACE_     ,CHECKIT|MENUTOGGLE,NULL,'L',-4),
        ITEM(Dsp,H_OVERSCAN,CHECKIT|MENUTOGGLE,NULL,'O',-4),
        ITEM(Dsp,V_OVERSCAN,CHECKIT|MENUTOGGLE,NULL,'V',-4),
        LASTITEM
};

/*
*       struct MenuItem ColItm[]={
*               ITEM(Col,PALETTE,NULL,NULL,'P',-4),
*               ITEM(Col,ASSIGN ,NULL,NULL,'G',-4),
*               LASTITEM
*       };
*/

struct Menu Menu[]={
        MENU(PRJ,Prj),
        MENU(DRW,Drw),
        MENU(_AREA,Area),
        MENU(DSP,Dsp),
/*
*       MENU(COL,Col),
*/
        LASTMENU
};
/*------------------------------------------------------------------------*/
struct Menu *MyMenu = Menu;
