#if !defined    STARTEND_P_H
        #define STARTEND_P_H
/*------------------------------------------------------------------------*/
        #if !defined EXEC_TYPES_H
                #include <exec/types.h>
        #endif
/*------------------------------------------------------------------------*/
        VOID    OpenMath(VOID),
                OpenArp(VOID),
                OpenScreens(VOID),
                        InitNewScreen(VOID),
                        OpenAuxScreen(VOID),
                        OpenDrawScreen(VOID),
                        OpenDrawWindow(VOID),
                        SetMenus(VOID);
        VOID    GoodBye(STRPTR),
                        DropAreas(VOID),
                        CloseMath(VOID),
                        CloseScreens(VOID),
                                CloseAuxScreen(VOID),
                                UnsetMenus(VOID),
                                CloseDrawWindow(VOID),
                                CloseDrawScreen(VOID),
                        CloseArp(VOID);
/*------------------------------------------------------------------------*/
#endif
