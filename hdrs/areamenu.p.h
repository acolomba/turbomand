#if !defined    AREAMENU_P_H
        #define AREAMENU_P_H
/*------------------------------------------------------------------------*/
        #if !defined EXEC_TYPES_H
                #include <exec/types.h>
        #endif
        #if !defined GRAPHICS_RASTPORT_H
                #include <graphics/rastport.h>
        #endif
/*------------------------------------------------------------------------*/
        VOID    ZoomIN(VOID),
                ZoomOUT(VOID);
/*
*               Compute(VOID);
*/
        VOID    SelectArea(LONG *, LONG *, LONG *, LONG *),
                DrawRect(struct RastPort *,LONG,LONG,LONG,LONG),
                PushArea(BOOL),
                PopArea(VOID),
                AddArea(VOID),
                RmArea(VOID);
/*------------------------------------------------------------------------*/
#endif
