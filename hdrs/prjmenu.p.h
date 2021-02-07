#if !defined    PRJMENU_P_H
        #define PRJMENU_P_H
/*------------------------------------------------------------------------*/
        #if !defined EXEC_TYPES_H
                #include <exec/types.h>
        #endif
/*------------------------------------------------------------------------*/
        BOOL    New(VOID),
                Load(VOID);
        VOID    About(VOID),
                Save(VOID);
        VOID    MyReqFunc(ULONG,CPTR),
                Params2Str(VOID),
                Str2Params(VOID);
/*------------------------------------------------------------------------*/
#endif
