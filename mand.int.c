#define IT_COMES_FROM_A_MAND_MODULE

#include <stdlib.h>
#include <exec/types.h>
#include <graphics/rastport.h>
#include <proto/graphics.h>
#include "hdrs/mand.p.h"
#include "hdrs/mand.v.h"
#include "hdrs/mand.int.d.h"
#include "hdrs/startend.v.h"
#include "hdrs/intmand.p.h"
/*------------------------------------------------------------------------*/
LONG    intleft,intright,intbottom,inttop,intr,ints;
LONG    newintleft  =-FUDGE<<1,
        newintright = FUDGE>>1,
        newintbottom=-FUDGE-FUDGE>>2,
        newinttop   = FUDGE+FUDGE>>2,
        newintr     = 0,
        newints     = 0;
LONG    intxgap,intygap;
LONG    *intp,*intq;
/*------------------------------------------------------------------------*/
VOID InitINTDraw()
{
        REGISTER i;
        IMPORT DOUBLE *ieeep,*ieeeq;

        intleft  =newintleft;
        intright =newintright;
        intbottom=newintbottom;
        inttop   =newinttop;
        intr     =newintr;
        ints     =newints;

        intxgap=iabs(intright-intleft)/width;
        intygap=iabs(inttop-intbottom)/height;

        if(intp) free(intp);
        intp=(LONG *)malloc(width*sizeof(LONG));
        for(i=0;i<width;i++)
                intp[i]=ieee2int(ieeep[i]);     /* intleft+i*intxgap; */

        if(intq) free(intq);
        intq=(LONG *)malloc(height*sizeof(LONG));
        for(i=0;i<height;i++)
                intq[i]=ieee2int(ieeeq[i]);     /* intbottom+i*intygap; */
}


#if defined NO_LATTICE_BUGS
        LONG __asm INTMand(REGISTER __d0 LONG i, REGISTER __d1 LONG j)
#else
        LONG INTMand(LONG i, LONG j)
#endif
{
        REGISTER LONG k;

        if(ReadPixel(xrp,i,j))
                return(ReadPixel(rp,i,j));

        if(k=intmand(intp[i],intq[j])){
                k=iters-k;
                k/=it4cl;
                k%=colmod;
                k++;
                SetAPen(rp,k);
                WritePixel(rp,i,j);
        }
        WritePixel(xrp,i,j);
        return(k);
}
