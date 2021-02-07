#include <exec/types.h>
#include <intuition/screens.h>
#include <libraries/dos.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include "hdrs/ilbm.h"
#include "hdrs/saveiff.p.h"
#include "hdrs/startend.p.h"
#include "hdrs/startend.v.h"
/*------------------------------------------------------------------------*/
#define bufSize 512
/*------------------------------------------------------------------------*/
VOID SaveIFF(STRPTR filename)
{
        LONG file;
        IFFP iffp = NO_FILE;

        file=Open(filename,MODE_NEWFILE);

        if(file){
                Write(file,"x",1);

                iffp=PutPicture(file,rp->BitMap,colormap);

                Close(file);
        }
}
/*------------------------------------------------------------------------*/
Point2D nullPoint = {0, 0};
/*------------------------------------------------------------------------*/
IFFP PutPicture(LONG file, struct BitMap *bitmap, WORD *colorMap)
{
        BYTE buffer[bufSize];

        return( PutAnILBM(file,bitmap,NULL,colorMap,bitmap->Depth,
                          &nullPoint,buffer,bufSize) );
}
/*------------------------------------------------------------------------*/
#define CkErr(expression)       { if(ifferr==IFF_OKAY) ifferr=(expression); }
/*------------------------------------------------------------------------*/
IFFP PutAnILBM(LONG file, struct BitMap *bitmap, BYTE *mask, WORD *colorMap,
               UBYTE depth, Point2D *xy, BYTE *buffer, LONG bufsize )
{
        BitMapHeader bmHdr;
        GroupContext fileContext, formContext;
        IFFP ifferr;
        WORD pageWidth, pageHeight;

        pageWidth =(bitmap->BytesPerRow) << 3;
        pageHeight=bitmap->Rows;

        ifferr=InitBMHdr(&bmHdr,bitmap,mskNone,cmpByteRun1,
                         NULL,pageWidth,pageHeight);

        bmHdr.nPlanes=depth;

        if(mask) bmHdr.masking=mskHasMask;
        bmHdr.x=xy->x;
        bmHdr.y=xy->y;

        CkErr(OpenWIFF(file,&fileContext,szNotYetKnown));
        CkErr(StartWGroup(&fileContext,FORM,szNotYetKnown,
                          ID_ILBM,&formContext));

        CkErr(PutBMHD(&formContext,&bmHdr));
        CkErr(PutCMAP(&formContext,colorMap,depth));
        CkErr(PutBODY(&formContext,bitmap,mask,&bmHdr,buffer,bufsize));
        CkErr(PutMAND(&formContext));

        CkErr(EndWGroup(&formContext));
        CkErr(CloseWGroup(&fileContext));

        return(ifferr);
}
