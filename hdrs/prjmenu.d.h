#if !defined    PRJMENU_D_H
        #define PRJMENU_D_H
/*------------------------------------------------------------------------*/
        #if !defined FONT_D_H
                #include "hdrs/font.d.h"
        #endif
        #if !defined MAND_D_H
                #include "hdrs/mand.d.h"
        #endif
/*------------------------------------------------------------------------*/
        #define FF      "%1.8lg"
/*------------------------------------------------------------------------*/
        #define XGAP            6
        #define YGAP            6
        #define BARGAP          13

        #define TXTW            12
        #define FLOAT_CHARS     12
        #define FLOAT_WIDTH     ((FLOAT_CHARS+1)*FONT_WIDTH)

        #define LEFT_GAD        0
        #define RIGHT_GAD       1
        #define BOTTOM_GAD      2
        #define TOP_GAD         3
        #define R_GAD           4
        #define S_GAD           5
        #define M_GAD           6

        #define ITERS_GAD       7
        #define ITERS_CHARS     5
        #define ITERS_TOP       (BARGAP+ITERS_GAD*(FONT_HEIGHT+YGAP))
        #define ITERS_WIDTH     ((ITERS_CHARS+1)*FONT_WIDTH)

        #define IT4CL_GAD       8
        #define IT4CL_CHARS     5
        #define IT4CL_TOP       (BARGAP+IT4CL_GAD*(FONT_HEIGHT+YGAP))
        #define IT4CL_WIDTH     ((IT4CL_CHARS+1)*FONT_WIDTH)

        #define NWIN_W          (FLOAT_WIDTH+TXTW*FONT_WIDTH+2*XGAP)
        #define NWIN_H          (BARGAP+1+(IT4CL_GAD+1)*(FONT_HEIGHT+YGAP))
/*------------------------------------------------------------------------*/
        #define GADTEXT(NAME,str) {                                     \
                                        3,0,                            \
                                        JAM1,                           \
                                        -TXTW*FONT_WIDTH,0,             \
                                        NULL,                           \
                                        str,                            \
                                        NULL                            \
                                }

        #define STRING(NAME)    {                                       \
                                        Gadbuf[NAME],                   \
                                        NULL,                           \
                                        FLOAT_CHARS,                    \
                                        FLOAT_CHARS+1,                  \
                                        0                               \
                                }


        #define STRGAD(NAME)    {                                       \
                                        &MyGad[NAME+1],                 \
                                        XGAP+TXTW*FONT_WIDTH,           \
                                        BARGAP+NAME*(FONT_HEIGHT+YGAP)+1,\
                                        FLOAT_WIDTH,FONT_HEIGHT,        \
                                        GADGHCOMP,STRINGRIGHT|RELVERIFY,\
                                        STRGADGET,                      \
                                        (APTR)&GadBorder,NULL,          \
                                        &GadText[NAME],                 \
                                        NULL,                           \
                                        (APTR)&GadInfo[NAME],           \
                                        NAME,                           \
                                        NULL                            \
                                }
/*------------------------------------------------------------------------*/
        typedef struct {
                        LONG    row,
                                pen;
                        STRPTR  text;
                }
                MYTEXT;

        #define Y2CH(y) (BARGAP+y*FONT_HEIGHT)
/*------------------------------------------------------------------------*/

#endif
