#define IT_COMES_FROM_A_MAND_MODULE

#include <stdlib.h>
#include <exec/types.h>
#include <graphics/rastport.h>
#include <libraries/mathieeedp.h>
#include <proto/graphics.h>
#include <proto/mathieeedoubbas.h>
#include "hdrs/mand.p.h"
#include "hdrs/mand.v.h"
#include "hdrs/startend.v.h"
/*------------------------------------------------------------------------*/
DOUBLE  ieeeleft,ieeeright,ieeebottom,ieeetop,ieeer,ieees;
DOUBLE  newieeeleft  =-2.00,
        newieeeright = 0.50,
        newieeebottom=-1.25,
        newieeetop   = 1.25,
        ieeeman   = 2.00,
        newieeer     = 0.00,
        newieees     = 0.00;
DOUBLE  ieeexgap,ieeeygap,ieeeman2;
FLOAT   *ieeep,*ieeeq;
/*------------------------------------------------------------------------*/
VOID InitIEEEDraw()
{
        REGISTER i;

        ieeeleft  =newieeeleft;
        ieeeright =newieeeright;
        ieeebottom=newieeebottom;
        ieeetop   =newieeetop;
        ieeer     =newieeer;
        ieees     =newieees;

        ieeeman2=ieeeman*ieeeman;
        ieeexgap=fabs(ieeeright-ieeeleft)/width;
        ieeeygap=fabs(ieeetop-ieeebottom)/height;

        if(ieeep) free(ieeep);
        ieeep=(FLOAT *)malloc(width*sizeof(FLOAT));
        for(i=0;i<width;i++)
                ieeep[i]=ieeeleft+i*ieeexgap;

        if(ieeeq) free(ieeeq);
        ieeeq=(FLOAT *)malloc(height*sizeof(FLOAT));
        for(i=0;i<height;i++)
                ieeeq[i]=ieeebottom+i*ieeeygap;
}

#if defined NO_LATTICE_BUGS
        LONG __asm IEEEMand(REGISTER __d0 LONG i, REGISTER __d1 LONG j)
#else
        LONG IEEEMand(LONG i, LONG j)
#endif
{
        REGISTER LONG k;
        DOUBLE p,q,x,y,x2,y2;

        if(ReadPixel(xrp,i,j))
                return(ReadPixel(rp,i,j));

        x=ieeer; p=ieeep[i];
        y=ieees; q=ieeeq[j];
        for(k=iters ; (x2=x*x)+(y2=y*y)<=ieeeman2 && k; k--){
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
