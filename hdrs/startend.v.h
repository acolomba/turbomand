#if !defined    STARTEND_V_H
        #define STARTEND_V_H
/*------------------------------------------------------------------------*/
        #if !defined GRAPHICS_GFXBASE_H
                #include <graphics/gfxbase.h>
        #endif
        #if !defined INTUITION_INTUITION_H
                #include <intuition/intuition.h>
        #endif
        #if !defined STARTEND_D_H
                #include <startend.d.h>
        #endif
/*------------------------------------------------------------------------*/
        GLOBAL struct Library           *MathIeeeDoubBasBase,
                                        *MathBase;

        GLOBAL struct GfxBase           *GfxBase;
        GLOBAL struct IntuitionBase     *IntuitionBase;

        IMPORT struct Screen    *scr,*xscr;
        IMPORT struct Window    *win;
        IMPORT struct RastPort  *rp,*xrp;

        IMPORT BOOL     menu_on;
        IMPORT WORD     width,height,depth,colors;
        IMPORT UWORD    colormap[];
        IMPORT VIEWMODE viewmode;
        IMPORT WORD     stdwidth,stdheight;
/*------------------------------------------------------------------------*/
#endif
