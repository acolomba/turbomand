#include <stdlib.h>
#include <exec/types.h>
#include "hdrs/areamenu.d.h"
#include "hdrs/areamenu.p.h"
#include "hdrs/mand.v.h"
#include "hdrs/packer.h"
#include "hdrs/ilbm.h"
/*------------------------------------------------------------------------*/
IFFP GetCMAP(GroupContext *ilbmContext, WORD *colorMap, UBYTE *pNColorRegs)
{
        REGISTER LONG nColorRegs;   
        REGISTER IFFP iffp;
        ColorRegister colorReg;

        nColorRegs=ilbmContext->ckHdr.ckSize/sizeofColorRegister;
        if(*pNColorRegs<nColorRegs)
                nColorRegs=*pNColorRegs;
        *pNColorRegs=nColorRegs;

        for(;nColorRegs>0;nColorRegs--){
                iffp=IFFReadBytes(ilbmContext,(BYTE *)&colorReg,
                                  sizeofColorRegister);
                CheckIFFP();

                *colorMap++=((colorReg.red  >>4)<<8)
                           |((colorReg.green>>4)<<4)
                           |((colorReg.blue >>4)   );
        }

        return(IFF_OKAY);
}

IFFP GetBODY(GroupContext *context, struct BitMap *bitmap, BYTE *mask,
             BitMapHeader *bmHdr, BYTE *buffer, LONG bufsize)
{
        REGISTER IFFP iffp;
        UBYTE srcPlaneCnt=bmHdr->nPlanes;
        LONG srcRowBytes=RowBytes(bmHdr->w);
        LONG bufRowBytes=MaxPackedSize(srcRowBytes);
        LONG nRows=bmHdr->h;
        Compression compression=bmHdr->compression;
        REGISTER LONG iPlane,iRow,nEmpty,nFilled;
        BYTE *buf,*nullDest,*nullBuf,**pDest;
        BYTE *planes[MaxSrcPlanes];

        if(compression>cmpByteRun1)
                return(CLIENT_ERROR);

        if(srcRowBytes!=bitmap->BytesPerRow || bufsize<bufRowBytes * 2
        || srcPlaneCnt>MaxSrcPlanes)
                return(CLIENT_ERROR);

        if(nRows>bitmap->Rows)
                nRows=bitmap->Rows;

        for(iPlane=0;iPlane<bitmap->Depth;iPlane++)
                planes[iPlane]=(BYTE *)bitmap->Planes[iPlane];
        for(; iPlane<MaxSrcPlanes;iPlane++)
                planes[iPlane] = NULL;

        if(bmHdr->masking==mskHasMask){
                if(mask!=NULL)  planes[srcPlaneCnt]=mask;
                else            planes[srcPlaneCnt]=NULL;

                srcPlaneCnt++;
        }

        nullDest =buffer;
        buffer  +=srcRowBytes;
        bufsize -=srcRowBytes;

        buf=buffer+bufsize;
        for(iRow=nRows;iRow>0;iRow--){
                for(iPlane=0;iPlane<srcPlaneCnt;iPlane++){

                        pDest=&planes[iPlane];

                        if(*pDest==NULL){
                                nullBuf=nullDest;
                                pDest=&nullBuf;
                        }

                        nEmpty =buf-buffer;
                        nFilled=bufsize-nEmpty;

                        if(nFilled<bufRowBytes){
                                movmem(buf,buffer,nFilled);

                                if(nEmpty>ChunkMoreBytes(context)){
                                        nEmpty=ChunkMoreBytes(context);
                                        bufsize=nFilled+nEmpty;
                                }

                                iffp=IFFReadBytes(context,&buffer[nFilled],
                                                  nEmpty);
                                CheckIFFP();

                                buf    =buffer;
                                nFilled=bufsize;
                                nEmpty =0;
                        }

                        if(compression==cmpNone){
                                if(nFilled<srcRowBytes)
                                        return(BAD_FORM);
                                movmem(buf,*pDest,srcRowBytes);
                                buf   +=srcRowBytes;
                                *pDest+=srcRowBytes;
                        }else if(UnPackRow(&buf,pDest,nFilled,srcRowBytes))
                                return(BAD_FORM);
                }
        }

        return(IFF_OKAY);
}

IFFP GetMAND(GroupContext *ilbmContext)
{
        REGISTER IFFP iffp;
        AREA hope;

        iffp=IFFReadBytes(ilbmContext,(BYTE *)&hope,sizeof(AREA));

        PushArea(HOWEVER);

        newieeeleft  =ieeeleft  =hope.ieeeleft;
        newieeeright =ieeeright =hope.ieeeright;
        newieeebottom=ieeebottom=hope.ieeebottom;
        newieeetop   =ieeetop   =hope.ieeetop;
        newieeer=ieeer=hope.ieeer;
        newieees=ieees=hope.ieees;
        ieeeman=hope.ieeeman;

        new