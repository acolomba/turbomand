#if !defined    AMAND_P_H
        #define AMAND_P_H
/*------------------------------------------------------------------------*/
        #if !defined EXEC_TYPES_H
                #include <exec/types.h>
        #endif
/*------------------------------------------------------------------------*/
        LONG __asm intmand(REGISTER __d6 LONG, REGISTER __d7 LONG);
/*------------------------------------------------------------------------*/
#endif
