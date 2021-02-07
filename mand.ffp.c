#define IT_COMES_FROM_A_MAND_MODULE

#include <stdlib.h>
#include <exec/types.h>
#include <graphics/rastport.h>
#include <libraries/mathffp.h>
#include <proto/graphics.h>
#include <proto/mathffp.h>
#include "hdrs/mand.p.h"
#include "hdrs/mand.v.h"
#include "hdrs/startend.v.h"
/*------------------------------------------------------------------------*/
FLOAT   ffpleft,ffpright,ffpbottom,ffptop,ffpr,ffps;
FLOAT   newffpleft  =-2.00,
        newffpright = 0.50,
        newffpbottom=-1.25,
        newffptop   = 1.25,
        ffpman      = 2.00,
        newffpr     = 0.00,
        newffps     = 0.00;
FLOAT   ffpxgap,ffpygap,ffpman2;
FLOAT   *ffpp,*ffpq;
/*------------------------------------------------------------------------*/
VOID InitFFPDraw()
{
        REGISTER i;

        ffpleft  =newffpleft;
        ffpright =newffpright;
        ffpbottom=newffpbottom;
        ffptop   =newffptop;
        ffpr     =newffpr;
        ffps     =newffps;

        ffpman2=ffpman*ffpman;
        ffpxgap=fabs(ffpright-ffpleft)/width;
        ffpygap=fabs(ffptop-ffpbottom)/height;

        if(ffpp) free(ffpp);
        ffpp=(FLOAT *)malloc(width*sizeof(FLOAT));
        for(i=0;i<width;i++)
                ffpp[i]=ffpleft+i*ffpxgap;

        if(ffpq) free(ffpq);
        ffpq=(FLOAT *)malloc(height*sizeof(FLOAT));
        for(i=0;i<height;i++)
                ffpq[i]=ffpbottom+i*ffpygap;
}

#if defined NO_LATTICE_BUGS
        LONG __asm FFPMand(REGISTER __d0 LONG i, REGISTER __d1 LONG j)
#else
        LONG FFPMand(LONG i, LONG j)
#endif
{
        REGISTER LONG k;
        FLOAT x,y,p,q,x2,y2;

        if(ReadPixel(xrp,i,j))
                return(ReadPixel(rp,i,j));

        x=ffpr; p=ffpp[i];
        y=ffps; q=ffpq[j];
        for(k=iters ; (x2=x*x)+(y2=y*y)<=ffpman2 && k ; k--){
                y=(x+x)*y+q;
                x=x2-y2+p;
        }

        WritePixel(xrp,i,j);

        if(k){
                k=iters-k;
                k/=it4cl;
                k%=colmod;
                k++;
                SetAPen(rp,k);
                WritePixel(rp,i,j);
        }
        return(k);
}
