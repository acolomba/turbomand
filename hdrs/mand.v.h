#if !defined    MAND_V_H
        #define MAND_V_H
/*------------------------------------------------------------------------*/
        #if !defined EXEC_TYPES_H
                #include <exec/types.h>
        #endif
/*------------------------------------------------------------------------*/
#if !defined IT_COMES_FROM_A_MAND_MODULE
        IMPORT DOUBLE ieeeleft,ieeeright,ieeebottom,ieeetop,ieeeman;
        IMPORT FLOAT  ffpleft,ffpright,ffpbottom,ffptop,ffpman;
        IMPORT LONG   intleft,intright,intbottom,inttop;

        IMPORT DOUBLE ieeer,ieees;
        IMPORT FLOAT  ffpr,ffps;
        IMPORT LONG   intr,ints;

        IMPORT DOUBLE ieeexgap,ieeeygap,ieeeman2;
        IMPORT FLOAT  ffpxgap,ffpygap,ffpman2;
        IMPORT LONG   intxgap,intygap,intman2;

        IMPORT DOUBLE   newieeeleft,newieeeright,newieeebottom,newieeetop,
                        newieeer,newieees;
        IMPORT FLOAT    newffpleft,newffpright,newffpbottom,newffptop,
                        newffpr,newffps;
        IMPORT LONG     newintleft,newintright,newintbottom,newinttop,
                        newintr,newints;

        IMPORT DOUBLE   *ieeep,*ieeeq;
        IMPORT FLOAT    *ffpp,*ffpq;
        IMPORT LONG     *intp,*intq;

        #if defined NO_LATTICE_BUG
                IMPORT LONG __asm (*Mand)(REGISTER __d0 LONG, REGISTER __d1 LONG);
        #else
                IMPORT LONG (* Mand)(LONG, LONG);
        #endif
#endif

        IMPORT LONG iters,it4cl;
        IMPORT WORD colmod;
        IMPORT WORD math;
/*------------------------------------------------------------------------*/
#endif
