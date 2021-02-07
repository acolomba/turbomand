#if !defined    MENUHAND_P_H
        #define MENUHAND_P_H
/*------------------------------------------------------------------------*/
        #if !defined EXEC_TYPES_H
                #include <exec/types.h>
        #endif
        #if !defined MENUHAND_D_H
                #include "menuhand.d.h"
        #endif
/*------------------------------------------------------------------------*/
        ITEM    HandleMenus(VOID),
                HandlePrj(UWORD),
                HandleDrw(UWORD),
                HandleArea(UWORD),
                HandleDsp(UWORD),
                HandleCol(UWORD);
/*------------------------------------------------------------------------*/
#endif
