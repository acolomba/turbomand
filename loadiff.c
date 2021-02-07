#include <exec/types.h>
#include <libraries/dos.h>
#include <graphics/gfxbase.h>
#include <graphics/rastport.h>
#include <graphics/gfx.h>
#include <graphics/view.h>
#include "hdrs/ilbm.h"
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/graphics.h>
#include "hdrs/loadiff.p.h"
#include "hdrs/startend.d.h"
#include "hdrs/startend.p.h"
#include "hdrs/startend.v.h"
/*------------------------------------------------------------------------*/
#define EXDepth 5
#define maxColorReg (1<<EXDepth)
#define MIN(a,b) ((a)<(b)?(a):(b))
#define SafeFreeMem(p,q) {if(p)FreeMem(p,q);}
#define bufSz 512
/*------------------------------------------------------------------------*/
typedef struct {
                ClientFrame clientFrame;
                UBYTE foundBMHD;
                UBYTE nColorRegs;
                BitMapHeader bmHdr;
                Color4 colorMap[maxColorReg];
        }
        ILBMFrame;
/*------------------------------------------------------------------------*/
VOID DisplayPic(ILBMFrame *ptilbmFrame)
{
        CloseScreens();

        width =ptilbmFrame->bmHdr.pageWidth;
        height=ptilbmFrame->bmHdr.pageHeight;

        if (width > stdwidth/2+OVERSCAN_W) {
                viewmode.hires=TRUE;
                if(width > stdwidth)
                        viewmode.h_overscan=TRUE;
                else    viewmode.h_overscan=FALSE;
        }else{
                viewmode.hires=FALSE;
                if(width > stdwidth/2)
                        viewmode.h_overscan=TRUE;
                else    viewmode.h_overscan=FALSE;
        }

        if (height > stdheight+OVERSCAN_H) {
                viewmode.lace=TRUE;
                if(height > stdheight*2)
                        viewmode.v_overscan=TRUE;
                else    viewmode.v_overscan=FALSE;
        }else{
                viewmode.lace=FALSE;
                if(height > stdheight)
                        viewmode.v_overscan=TRUE;
                else    viewmode.v_overscan=FALSE;
        }

        OpenScreens();
}

IFFP GetFoILBM(GroupContext *parent)
{
        IFFP iffp;
        GroupContext formContext;
        ILBMFrame ilbmFrame;
        LONG plsize;
        BYTE buffer[bufSz];

        if(parent->subtype!=ID_ILBM)
                return(IFF_OKAY);

        ilbmFrame=*(ILBMFrame *)parent->clientFrame;
        iffp=OpenRGroup(parent,&formContext);
        CheckIFFP();

        do
                switch(iffp=GetFChunkHdr(&formContext)) {
                        case ID_BMHD: {
                                ilbmFrame.foundBMHD = TRUE;
                                iffp=GetBMHD(&formContext, &ilbmFrame.bmHdr);
                                break;
                        }
                        case ID_CMAP: {
                                ilbmFrame.nColorRegs = maxColorReg;
                                iffp=GetCMAP(&formContext,
                                             colormap,
                                             &ilbmFrame.nColorRegs);
                                LoadRGB4(&scr->ViewPort,colormap,
                                         ilbmFrame.nColorRegs);

                                break;
                        }
                        case ID_BODY: {
                                if(!ilbmFrame.foundBMHD)
                                        return(BAD_FORM);

                                depth=MIN(ilbmFrame.bmHdr.nPlanes, EXDepth);

                                plsize=RowBytes(ilbmFrame.bmHdr.w) *
                                       ilbmFrame.bmHdr.h;

                                DisplayPic(&ilbmFrame);
                                iffp=GetBODY(&formContext,rp->BitMap,NULL,
                                             &ilbmFrame.bmHdr,buffer,bufSz);

                                break;
                        }
                        case ID_MAND: {
                                iffp=GetMAND(&formContext);

                                break;
                        }
                        case END_MARK: {
                                iffp = IFF_DONE;
                                break;
                        }
                }
        while(iffp>=IFF_OKAY);

        if(iffp!=IFF_DONE)
                return(iffp);

        CloseRGroup(&formContext);
        return(iffp);
}

IFFP GetPrILBM(GroupContext *parent)
{
        IFFP iffp;
        GroupContext propContext;
        ILBMFrame *ilbmFrame=(ILBMFrame *)parent->clientFrame;

        if(parent->subtype!=ID_ILBM)
                return(IFF_OKAY);

        iffp=OpenRGroup(parent,&propContext);
        CheckIFFP();

        do
                switch(iffp=GetPChunkHdr(&propContext)) {
                        case ID_BMHD: {
                                ilbmFrame->foundBMHD=TRUE;
                                iffp=GetBMHD(&propContext,&ilbmFrame->bmHdr);
                                break;
                        }
                        case ID_CMAP: {
                                ilbmFrame->nColorRegs=maxColorReg;
                                iffp=GetCMAP(&propContext,
                                             (WORD *)ilbmFrame->colorMap,
                                             &ilbmFrame->nColorRegs);
                                break;
                        }
                }
        while(iffp>=IFF_OKAY);

        CloseRGroup(&propContext);
        return(iffp==END_MARK ? IFF_OKAY : iffp);
}

IFFP GetLiILBM(GroupContext *parent)
{
        ILBMFrame newFrame;

        newFrame=*(ILBMFrame *)parent->clientFrame;

        return(ReadIList(parent,(ClientFrame *)&newFrame));
}

IFFP ReadPicture(LONG file)
{
        ILBMFrame iFrame;
        IFFP iffp=IFF_OKAY;

        iFrame.clientFrame.getList =GetLiILBM;
        iFrame.clientFrame.getProp =GetPrILBM;

        iFrame.clientFrame.getForm =GetFoILBM;
        iFrame.clientFrame.getCat  =ReadICat ;

        iFrame.foundBMHD =FALSE;
        iFrame.nColorRegs=0;

        iffp=ReadIFF(file,(ClientFrame *)&iFrame);

        Close(file);
        return(iffp);
}

VOID LoadIFF(STRPTR filename)
{
        LONG file;
        IFFP iffp=NO_FILE;

        file=Open(filename,MODE_OLDFILE);
        if(file) iffp=ReadPicture(file);
}
