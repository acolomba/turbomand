#if !defined    ATOF_P_H
        #define ATOF_P_H
/*------------------------------------------------------------------------*/
        #if !defined EXEC_TYPES_H
                #include <exec/types.h>
        #endif
/*------------------------------------------------------------------------*/
        DOUBLE  ieeeatof(STRPTR);
        FLOAT   ffpatof(STRPTR);

        LONG    ieee2int(DOUBLE);

/*      ***  Look in ffpatof.c for explanations ...  ***
*
*       LONG    ffp2int(FLOAT);
*/

/*------------------------------------------------------------------------*/
#endif
