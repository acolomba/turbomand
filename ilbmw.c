#include <exec/types.h>
#include "hdrs/areamenu.d.h"
#include "hdrs/mand.v.h"
#include "hdrs/packer.h"
#include "hdrs/ilbm.h"
#include "hdrs/startend.v.h"
/*------------------------------------------------------------------------*/
IFFP InitBMHdr(BitMapHeader *bmHdr0, struct BitMap *bitmap, LONG masking,
               LONG compression, LONG transparentColor, LONG pageWidth,
               LONG pageHeight)
{
    REGISTER BitMapHeader *bmHdr = bmHdr0;
    REGISTER WORD rowBytes = bitmap->BytesPerRow;

        bmHdr->w=rowBytes << 3;
        bmHdr->h=bitmap->Rows;
        bmHdr->x=bmHdr->y=0;
        bmHdr->nPlanes=bitmap->Depth;
        bmHdr->masking=masking;
        bmHdr->compression=compression;
        bmHdr->pad1=0;
        bmHdr->transparentColor=transparentColor;
        bmHdr->xAspect=bmHdr->yAspect=1;
        bmHdr->pageWidth=pageWidth;
        bmHdr->pageHeight=pageHeight;

        if(!viewmode.hires){
                if(!viewmode.lace){
                        bmHdr->xAspect=x320x200Aspect;
                        bmHdr->yAspect=y320x200Aspect;
                }else {
                        bmHdr->xAspect=x320x400Aspect;
                        bmHdr->yAspect=y320x400Aspect;
                }
        }else {
                if(!viewmode.lace){
                        bmHdr->xAspect=x640x200Aspect;
                        bmHdr->yAspect=y640x200Aspect;
                }else {
                        bmHdr->xAspect=x640x400Aspect;
                        bmHdr->yAspect=y640x400Aspect;
                }
        }

        return(IS_ODD(rowBytes) ? CLIENT_ERROR : IFF_OKAY );
}

IFFP PutCMAP(GroupContext *context, WORD *colorMap, UBYTE depth)
{
        REGISTER LONG nColorRegs;   
        IFFP iffp;
        ColorRegister colorReg;

        if(depth>MaxAmDepth)
                depth=MaxAmDepth;
        nColorRegs=1<<depth;

        iffp=PutCkHdr(context,ID_CMAP,nColorRegs*sizeofColorRegister);
        CheckIFFP();

        for(;nColorRegs;nColorRegs--){
                colorReg.red  =(*colorMap>>4 )&0xf0;
                colorReg.green=(*colorMap    )&0xf0;
                colorReg.blue =(*colorMap<<4 )&0xf0;
                iffp=IFFWriteBytes(context,(BYTE *)&colorReg,
                                   sizeofColorRegister);
                CheckIFFP();
                colorMap++;
        }

        iffp=PutCkEnd(context);
        return(iffp);
}

IFFP PutBODY(GroupContext *context, struct BitMap *bitmap, BYTE *mask,
             BitMapHeader *bmHdr, BYTE *buffer, LONG bufsize)
{
        IFFP iffp;
        LONG rowBytes=bitmap->BytesPerRow;
        LONG dstDepth=bmHdr->nPlanes;
        Compression compression=bmHdr->compression;
        LONG planeCnt;
        REGISTER LONG iPlane,iRow;
        REGISTER LONG packedRowBytes;
        BYTE *buf;
        BYTE *planes[MaxAmDepth + 1];

        if(bufsize<MaxPackedSize(rowBytes) || compression>cmpByteRun1
        || bitmap->Rows!=bmHdr->h || rowBytes!=RowBytes(bmHdr->w)
        || bitmap->Depth<dstDepth || dstDepth>MaxAmDepth)
                return(CLIENT_ERROR);

        planeCnt=dstDepth+(mask==NULL ? 0 : 1);

        for(iPlane=0;iPlane<dstDepth;iPlane++)
                planes[iPlane]=(BYTE *)bitmap->Planes[iPlane];

        if(mask) planes[dstDepth]=mask;

        iffp=PutCkHdr(context,ID_BODY,szNotYetKnown);
        CheckIFFP();

        for(iRow=bmHdr->h;iRow>0;iRow--){
                for(iPlane=0;iPlane<planeCnt;iPlane++){
                        if(compression==cmpNone){
                                iffp=IFFWriteBytes(context,planes[iPlane],
                                                   rowBytes);
                                planes[iPlane]+=rowBytes;
                        }else {
                                buf=buffer;
                                packedRowBytes=PackRow(&planes[iPlane],
                                                       &buf, rowBytes);
                                iffp=IFFWriteBytes(context,buffer,
                                                   packedRowBytes);
                        }

                        CheckIFFP();
                }
        }

        iffp=PutCkEnd(context);

        return(iffp);
}

IFFP PutMAND(GroupContext *context)
{
        IFFP iffp;
        AREA hope;

        iffp=PutCkHdr(context,ID_MAND,sizeof(AREA));
        CheckIFFP();

        hope.ieeeleft  =ieeeleft;
        hope.ieeeright =ieeeright;
        hope.ieeebottom=ieeebottom;
        hope.ieeetop   =ieeetop;
        hope.ieeer=ieeer;
        hope.ieees=ieees;
        hope.ieeeman=ieeeman;

        hope.ffpleft  =ffpleft;
        hope.ffpright =ffpright;
        hope.ffpbottom=ffpbottom;
        hope.ffptop   =ffptop;
        hope.ffpr=ffpr;
        hope.ffps=ffps;
        hope.ffpman=ffpman;

        hope.intleft  =intleft;
        hope.intright =intright;
        hope.intbottom=intbottom;
        hope.inttop   =inttop;
        hope.intr=intr;
        hope.ints=ints;

        hope.iters=iters;
        hope.it4cl=it4cl;


        iffp=IFFWriteBytes(context,(BYTE *)&hope,sizeof(AREA));
        CheckIFFP();
        iffp=PutCkEnd(context);

        return(iffp);
}
