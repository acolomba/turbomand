#include <exec/types.h>
#include "hdrs/packer.h"
/*------------------------------------------------------------------------*/
#define DUMP   0
#define RUN   1

#define MinRun 3   
#define MaxRun 128
#define MaxDat 128
/*------------------------------------------------------------------------*/
#define GetByte()   (*source++)
#define PutByte(c)   { *dest++ = (c);   ++putSize; }

#define OutDump(nn)   dest = PutDump(dest, nn)
#define OutRun(nn,cc) dest = PutRun(dest, nn, cc)
/*------------------------------------------------------------------------*/
LONG putSize;
TEXT buf[256];
/*------------------------------------------------------------------------*/
BYTE *PutDump(BYTE *dest, LONG nn)
{
        LONG i;

        PutByte(nn-1);
        for(i=0;i<nn;i++)
                PutByte(buf[i]);

        return(dest);
}

BYTE *PutRun(BYTE *dest, LONG nn, LONG cc)
{
        PutByte(-(nn-1));
        PutByte(cc);
        return(dest);
}

LONG PackRow(BYTE **pSource, BYTE **pDest, LONG rowSize)
{
        BYTE *source,*dest;
        TEXT c,lastc='\0';
        BOOL mode=DUMP;
        WORD nbuf=0;
        WORD rstart=0;

        source=*pSource;
        dest=*pDest;
        putSize=0;
        buf[0]=lastc=c=GetByte();
        nbuf=1;
        rowSize--;


        for(;rowSize;rowSize--){
                buf[nbuf++]=c=GetByte();
                switch(mode) {
                        case DUMP:
                                if(nbuf>MaxDat){
                                        OutDump(nbuf-1);
                                        buf[0]=c;
                                        nbuf=1;
                                        rstart=0;
                                        break;
                                }
                                if(c==lastc){
                                        if(nbuf-rstart>=MinRun){
                                                if(rstart>0)
                                                        OutDump(rstart);
                                                mode=RUN;
                                        }else if(rstart==0)
                                                mode=RUN;
                                }else rstart = nbuf-1;
                                break;
                        case RUN :
                                if((c!=lastc) || (nbuf-rstart>MaxRun)){
                                        OutRun(nbuf-1-rstart,lastc);
                                        buf[0]=c;
                                        nbuf=1;
                                        rstart=0;
                                        mode=DUMP;
                                }
                                break;
                }

                lastc = c;
        }

        switch(mode){
                case DUMP:
                        OutDump(nbuf);
                        break;
                case RUN:
                        OutRun(nbuf-rstart,lastc);
                        break;
        }

        *pSource=source;
        *pDest=dest;

        return(putSize);
}
