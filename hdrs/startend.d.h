#if !defined    STARTEND_D_H
        #define STARTEND_D_H
/*------------------------------------------------------------------------*/
        #define MINVERSION      33L
/*------------------------------------------------------------------------*/
#define MY_IDCMP        (MOUSEMOVE | MOUSEBUTTONS)
#define MN_IDCMP        (MENUPICK  | MENUVERIFY  )

#define RMBtrapON(w)    ((w)->Flags|= RMBTRAP)
#define RMBtrapOFF(w)   ((w)->Flags&=~RMBTRAP)
/*------------------------------------------------------------------------*/
        typedef
                struct {
                        unsigned hires      :1;
                        unsigned lace       :1;
                        unsigned h_overscan :1;
                        unsigned v_overscan :1;
                }
                VIEWMODE;

        #if !defined TRUE
                #define TRUE    1
                #define FALSE   0
        #endif
/*------------------------------------------------------------------------*/
        #define OVERSCAN_W      32
        #define OVERSCAN_H      26
/*------------------------------------------------------------------------*/
#endif
