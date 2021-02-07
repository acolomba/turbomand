#if !defined    INTUIMSGS_H
        #define INTUIMSGS_H
/*------------------------------------------------------------------------*/
        #if !defined EXEC_TYPES_H
                #include <exec/types.h>
        #endif
        #if !defined INTUITION_INTUITION_H
                #include <intuition/intuition.h>
        #endif
/*------------------------------------------------------------------------*/
        BOOL    GetIMsg(struct Window *,ULONG *,UWORD *,UWORD *,UWORD *);
/*------------------------------------------------------------------------*/
#endif
