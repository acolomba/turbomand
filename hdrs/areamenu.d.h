#if !defined    AREAMENU_D_H
        #define AREAMENU_D_H
/*------------------------------------------------------------------------*/
        #if !defined EXEC_TYPES_H
                #include <exec/types.h>
        #endif
/*------------------------------------------------------------------------*/
        #define HOWEVER         1
        #define IFNECESSARY     0
/*------------------------------------------------------------------------*/
        typedef struct Area {
                struct Area     *Prev,
                                *Next;
                        DOUBLE  ieeeleft,ieeeright,
                                ieeebottom,ieeetop,
                                ieeer,ieees,
                                ieeeman;
                        FLOAT   ffpleft,ffpright,
                                ffpbottom,ffptop,
                                ffpr,ffps,
                                ffpman;
                        LONG    intleft,intright,
                                intbottom,inttop,
                                intr,ints;
                        LONG    iters,
                                it4cl;
                        LONG    data[20];
                }
                AREA;
/*------------------------------------------------------------------------*/
#endif
