#include <exec/types.h>
#include "hdrs/packer.h"
/*------------------------------------------------------------------------*/
#define UGetByte()   (*source++)
#define UPutByte(c)   (*dest++ = (c))
/*------------------------------------------------------------------------*/
BOOL UnPackRow(BYTE **pSource, BYTE **pDest, WORD srcBytes0, WORD dstBytes0)
{
        REGISTER BYTE *source=*pSource;
        REGISTER BYTE *dest  =*pDest;
        REGISTER WORD n;
        REGISTER BYTE c;
        REGISTER WORD srcBytes=srcBytes0,
                      dstBytes=dstBytes0;
        BOOL error=TRUE;
        WORD minus128=-128;

        while(dstBytes>0){
                if((srcBytes-=1)<0)
                        goto ErrorExit;

                n=UGetByte();

                if(n>=0){
                        n+=1;

                        if((srcBytes-=n)<0)
                                goto ErrorExit;
                        if((dstBytes-=n)<0)
                                goto ErrorExit;
                        do {
                                UPutByte(UGetByte());
                        } while(--n>0);
                }else if(n!=minus128){
                        n=-n+1;
                        if((srcBytes-=1)<0)
                                goto ErrorExit;
                        if((dstBytes-=n)<0)
                                goto ErrorExit;
                        c=UGetByte();

                        do {
                                UPutByte(c);
                        } while(--n>0);
                }
        }

        error=FALSE;

ErrorExit:
        *pSource=source;
        *pDest  =dest;

        return(error);
}
