#if !defined    MENUHAND_D_H
        #define MENUHAND_D_H
/*------------------------------------------------------------------------*/
        #define WAIT    TRUE
        #define NOWAIT  FALSE
/*------------------------------------------------------------------------*/
        typedef
                enum {
                        NONE,
                        _ABOUT, _START, _ZOOM_IN , _HIRES     , _PALETTE,
                        _NEW  , _STOP , _ZOOM_OUT, _LACE      , _ASSIGN,
                        _LOAD , _IEEE , _COMPUTE ,
                        _SAVE , _FFP  , _H_OVERSCAN,
                        _QUIT , _INT  , _V_OVERSCAN
                }
                ITEM;
/*------------------------------------------------------------------------*/
#endif
