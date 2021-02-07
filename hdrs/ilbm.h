#if !defined ILBM_H
        #define ILBM_H

        #if !defined EXEC_TYPES_H
                #include <exec/types.h>
        #endif

        #if !defined GRAPHICS_GFX_H
                #include <graphics/gfx.h>
        #endif

        #include "iff.h"

        #define ID_ILBM MakeID('I','L','B','M')
        #define ID_BMHD MakeID('B','M','H','D')
        #define ID_CMAP MakeID('C','M','A','P')
        #define ID_GRAB MakeID('G','R','A','B')
        #define ID_DEST MakeID('D','E','S','T')
        #define ID_SPRT MakeID('S','P','R','T')
        #define ID_CAMG MakeID('C','A','M','G')
        #define ID_BODY MakeID('B','O','D','Y')

        #define ID_MAND MakeID('M','A','N','D') /* He,he,he... */

        typedef UBYTE Masking;
        #define mskNone                 0
        #define mskHasMask              1
        #define mskHasTransparentColor  2
        #define mskLasso                3

        typedef UBYTE Compression;
        #define cmpNone         0
        #define cmpByteRun1     1

        #define x320x200Aspect  10
        #define y320x200Aspect  10
        #define x320x400Aspect  20
        #define y320x400Aspect  10
        #define x640x200Aspect  5
        #define y640x200Aspect  10
        #define x640x400Aspect  10
        #define y640x400Aspect  10

        typedef struct {
                        UWORD   w,h;
                        WORD    x,y;
                        UBYTE   nPlanes;
                        Masking masking;
                        Compression compression;
                        UBYTE   pad1;
                        UWORD   transparentColor;
                        UBYTE   xAspect,yAspect;
                        WORD    pageWidth,pageHeight;
                }
                BitMapHeader;

        #define RowBytes(w)     (((w)+15)>>4<<1)

        typedef struct {
                        UBYTE red,green,blue;
                }
                ColorRegister;

        #define sizeofColorRegister 3

        typedef WORD Color4;

        #define MaxAmDepth 6

        typedef struct {
                        WORD x,y;
                }
                Point2D;

        typedef struct {
                        UBYTE depth;
                        UBYTE pad1;
                        UWORD planePick;
                        UWORD planeOnOff;
                        UWORD planeMask;
                }
                DestMerge;

        typedef UWORD SpritePrecedence;

        #if 0
                typedef UWORD ViewportMode;
        #endif

        #define PutBMHD(context, bmHdr)         \
                PutCk(context, ID_BMHD, sizeof(BitMapHeader), (BYTE *)bmHdr)
        #define PutGRAB(context, point2D)       \
                PutCk(context, ID_GRAB, sizeof(Point2D), (BYTE *)point2D)
        #define PutDEST(context, destMerge)     \
                PutCk(context, ID_DEST, sizeof(DestMerge), (BYTE *)destMerge)
        #define PutSPRT(context, spritePrec)    \
                PutCk(context, ID_SPRT, sizeof(SpritePrecedence), (BYTE *)spritePrec)

        extern IFFP     InitBMHdr(/*BitMapHeader *, struct BitMap *, bmHdr,
                                  bitmap, int, int, int, int, int*/),
                        PutCMAP(GroupContext *, WORD *,   UBYTE),
                        PutBODY(GroupContext *, struct BitMap *, BYTE *,
                                BitMapHeader *, BYTE *, LONG);

        #define GetBMHD(context, bmHdr)         \
                IFFReadBytes(context, (BYTE *)bmHdr, sizeof(BitMapHeader))
        #define GetGRAB(context, point2D)       \
                IFFReadBytes(context, (BYTE *)point2D, sizeof(Point2D))
        #define GetDEST(context, destMerge)     \
                IFFReadBytes(context, (BYTE *)destMerge, sizeof(DestMerge))
        #define GetSPRT(context, spritePrec)    \
                IFFReadBytes(context, (BYTE *)spritePrec, sizeof(SpritePrecedence))

        extern IFFP GetCMAP(GroupContext *, WORD *,   UBYTE *);

        #define MaxSrcPlanes 16+1

        extern IFFP GetBODY(GroupContext *, struct BitMap *, BYTE *,
                            BitMapHeader *, BYTE *, LONG);
#endif
