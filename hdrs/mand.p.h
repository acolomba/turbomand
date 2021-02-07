#if !defined    MAND_P_H
        #define MAND_P_H
/*------------------------------------------------------------------------*/
        #if !defined EXEC_TYPES_H
                #include <exec/types.h>
        #endif
        #if !defined MENUHAND_D_H
                #include "menuhand.d.h"
        #endif
/*------------------------------------------------------------------------*/
        VOID    SetMand(SHORT),
                InitDraw(BOOL);
        ITEM    __asm DrawMand(REGISTER __d0 LONG, REGISTER __d1 LONG,
                               REGISTER __d2 LONG, REGISTER __d3 LONG );
        BOOL    __asm sides(REGISTER __d0 LONG, REGISTER __d1 LONG,
                            REGISTER __d2 LONG, REGISTER __d3 LONG);

        VOID    InitIEEEDraw(VOID),
                InitFFPDraw(VOID),
                InitINTDraw(VOID);

        #if defined NO_LATTICE_BUGS
                LONG __asm IEEEMand(REGISTER __d0 LONG, REGISTER __d1 LONG);
                LONG __asm FFPMand(REGISTER __d0 LONG, REGISTER __d1 LONG);
                LONG __asm INTMand(REGISTER __d0 LONG, REGISTER __d1 LONG);
        #else
                LONG    IEEEMand(LONG, LONG),
                        FFPMand(LONG, LONG),
                        INTMand(LONG, LONG);
        #endif
/*------------------------------------------------------------------------*/
#endif
