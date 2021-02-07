#include <ctype.h>
#include <exec/types.h>
#include <proto/mathffp.h>
#include "hdrs/atof.p.h"
#include "hdrs/mand.int.d.h"
/*------------------------------------------------------------------------*/
FLOAT ffpatof(STRPTR str)
{
        BOOL minus=FALSE;
        FLOAT ffp=0.0;

        for(;*str==' ';str++);

        switch(*str){
                case '-': minus=TRUE;
                case '+': str++;
        }

        for(;isdigit(*str);str++){
                ffp*=10.0;
                ffp+=(FLOAT)(*str-'0');
        }

        if(*str=='.'){
                FLOAT div;

                str++;
                for(div=10.0;isdigit(*str);div*=10.0,str++)
                        ffp+=(FLOAT)(*str-'0')/div;
        }

        if(minus) ffp=-ffp;

        return(ffp);
}

/*      ***  Lattice crashes when it compile the following lines...  ***
*
*       LONG ffp2int(FLOAT ffp)
*       {
*               return((LONG)(ffp*(FLOAT)FUDGE);
*       }
*/
