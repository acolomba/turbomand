#include <exec/types.h>
#include <intuition/intuition.h>
#include <proto/exec.h>
#include "hdrs/stuff.p.h"
#include "hdrs/intuimsgs.p.h"
/*------------------------------------------------------------------------*/
BOOL GetIMsg(struct Window *win, ULONG *class ,UWORD *code ,
                                 UWORD *mousex,UWORD *mousey)
{
        struct IntuiMessage *message;

bleah:;

        message=(struct IntuiMessage *)GetMsg(win->UserPort);
        if(message==NULL) return(FALSE);

        *class=message->Class;
        *code=message->Code;
        *mousex=message->MouseX;
        *mousey=message->MouseY;

        if(*class==MENUVERIFY)
                SetWorkPalette();

        ReplyMsg((struct Message *)message);
        return(TRUE);
}
