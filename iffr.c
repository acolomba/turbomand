#include <exec/types.h>
#include <libraries/dos.h>
#include <proto/dos.h>
#include "hdrs/iff.h"
/*------------------------------------------------------------------------*/
#ifndef OFFSET_BEGINNING
#define OFFSET_BEGINNING OFFSET_BEGINING
#endif
/*------------------------------------------------------------------------*/
IFFP OpenRIFF(BPTR file0, GroupContext *new0, ClientFrame *clientFrame)
{
        REGISTER BPTR file=file0;
        REGISTER GroupContext *new = new0;
        IFFP iffp=IFF_OKAY;

        new->parent      =NULL;
        new->clientFrame =clientFrame;
        new->file        =file;
        new->position    =0;
        new->ckHdr.ckID  =new->subtype   =NULL_CHUNK;
        new->ckHdr.ckSize=new->bytesSoFar=0;

        if(file<=0) return(NO_FILE);

        Seek(file,0,OFFSET_END);
        new->bound=Seek(file,0,OFFSET_CURRENT);
        if(new->bound<0) return(DOS_ERROR);

        Seek(file,0,OFFSET_BEGINNING);
        if(new->bound<sizeof(ChunkHeader))
        iffp=NOT_IFF;

        return(iffp);
}

IFFP OpenRGroup(GroupContext *parent0, GroupContext *new0)
{
        REGISTER GroupContext *parent=parent0;
        REGISTER GroupContext *new   =new0;
        IFFP iffp=IFF_OKAY;

        new->parent      =parent;
        new->clientFrame =parent->clientFrame;
        new->file        =parent->file;
        new->position    =parent->position;
        new->bound       =parent->position+ChunkMoreBytes(parent);
        new->ckHdr.ckID  =new->subtype   =NULL_CHUNK;
        new->ckHdr.ckSize=new->bytesSoFar=0;

        if(new->bound>parent->bound || IS_ODD(new->bound))
                iffp=BAD_IFF;

        return(iffp);
}

IFFP CloseRGroup(GroupContext *context)
{
        REGISTER LONG position;

        if(context->parent!=NULL){
                position=context->position;
                context->parent->bytesSoFar+=position-context->parent->position;
                context->parent->position=position;
        }

        return(IFF_OKAY);
}

IFFP SkipFwd(GroupContext *context, LONG bytes)
{
        IFFP iffp=IFF_OKAY;

        if(bytes>0){
                if (Seek(context->file, bytes, OFFSET_CURRENT)==-1)
                        iffp = BAD_IFF;
                else    context->position += bytes;
        }

        return(iffp);
}

ID GetChunkHdr(GroupContext *context0)
{
        REGISTER GroupContext *context=context0;
        REGISTER IFFP iffp;
        LONG remaining;

        iffp=SkipFwd(context,ChunkMoreBytes(context)+
                             IS_ODD(context->ckHdr.ckSize));

        CheckIFFP();

        context->ckHdr.ckID=BAD_IFF;
        context->subtype   =NULL_CHUNK;
        context->bytesSoFar=0;

        remaining=context->bound-context->position;
        if(remaining==0){
                context->ckHdr.ckSize=0;
                context->ckHdr.ckID  =END_MARK;
        }
        else if(sizeof(ChunkHeader)>remaining){
                context->ckHdr.ckSize = remaining;
        }else {
                switch(Read(context->file,&context->ckHdr,
                       sizeof(ChunkHeader))){
                        case -1: return(context->ckHdr.ckID=DOS_ERROR);
                        case  0: return(context->ckHdr.ckID=BAD_IFF);
                }

                if (context->parent==NULL)
                        switch(context->ckHdr.ckID){
                                case FORM:
                                case LIST:
                                case CAT : break;
                                default: return(context->ckHdr.ckID=NOT_IFF);
                        }

                context->position+=sizeof(ChunkHeader);
                remaining        -=sizeof(ChunkHeader);

                if(context->ckHdr.ckID<=0)
                        context->ckHdr.ckID=BAD_IFF;
                else if(context->ckHdr.ckSize<0
                     || context->ckHdr.ckSize>remaining){
                        context->ckHdr.ckSize=remaining;
                        context->ckHdr.ckID  =BAD_IFF;
                }else switch(context->ckHdr.ckID){
                        case LIST:
                        case FORM:
                        case PROP:
                        case CAT : {
                                iffp = IFFReadBytes(context,
                                                   (BYTE *)&context->subtype,
                                                    sizeof(ID));
                                if(iffp!=IFF_OKAY)
                                        context->ckHdr.ckID=iffp;
                                break;
                        }
                }
        }

        return(context->ckHdr.ckID);
}

IFFP IFFReadBytes(GroupContext *context, BYTE *buffer, LONG nBytes)
{
        REGISTER IFFP iffp=IFF_OKAY;

        if(nBytes<0)
                iffp=CLIENT_ERROR;
        else if(nBytes>ChunkMoreBytes(context))
                iffp=SHORT_CHUNK;
        else if(nBytes>0)
                switch(Read(context->file,buffer,nBytes)){
                        case -1: {
                                iffp=DOS_ERROR;
                                break;
                        }
                        case  0: {
                                iffp=BAD_IFF;
                                break;
                        }
                        default: {
                                context->position  +=nBytes;
                                context->bytesSoFar+=nBytes;
                        }
                }

        return(iffp);
}

IFFP SkipGroup(GroupContext *context)
{}

IFFP ReadIFF(BPTR file, ClientFrame *clientFrame)
{
        IFFP iffp;
        GroupContext context;

        iffp=OpenRIFF(file,&context);
        context.clientFrame=clientFrame;

        if(iffp==IFF_OKAY)
                switch(iffp=GetChunkHdr(&context)) {
                        case FORM: {
                                iffp=(*clientFrame->getForm)(&context);
                                break;
                        }
                        case LIST: {
                                iffp=(*clientFrame->getList)(&context);
                                break;
                        }
                        case CAT : {
                                iffp=(*clientFrame->getCat )(&context);
                                break;
                        }
                }

        CloseRGroup(&context);

        if(iffp>0)
                iffp = NOT_IFF;

        return(iffp);
}

IFFP ReadIList(GroupContext *parent, ClientFrame *clientFrame)
{
        GroupContext listContext;
        IFFP iffp;
        BOOL propOk=TRUE;

        iffp=OpenRGroup(parent,&listContext);
        CheckIFFP();

        if(parent->ckHdr.ckID==CAT)
                propOk=FALSE;
        else
                listContext.clientFrame=clientFrame;

        do {
                switch(iffp=GetChunkHdr(&listContext)) {
                        case PROP: {
                                if(propOk) iffp=(*clientFrame->getProp)
                                                (&listContext);
                                else       iffp=BAD_IFF;

                                break;
                        }
                        case FORM: {
                                iffp=(*clientFrame->getForm)(&listContext);
                                break;
                        }
                        case LIST: {
                                iffp=(*clientFrame->getList)(&listContext);
                                break;
                        }
                        case CAT : {
                                iffp=(*clientFrame->getCat )(&listContext);
                                break;
                        }
                }

                if (listContext.ckHdr.ckID!=PROP)
                        propOk=FALSE;
        } while(iffp==IFF_OKAY);

        CloseRGroup(&listContext);

        if(iffp>0) iffp=BAD_IFF;

        return(iffp==END_MARK ? IFF_OKAY : iffp);
}

IFFP ReadICat(GroupContext *parent)
{
        return(ReadIList(parent,NULL));
}

ID GetFChunkHdr(GroupContext *context)
{
        REGISTER ID id;

        id=GetChunkHdr(context);
        if(id==PROP) context->ckHdr.ckID=id=BAD_IFF;

        return(id);
}

ID GetF1ChunkHdr(GroupContext *context)
{
        REGISTER ID id;
        REGISTER ClientFrame *clientFrame=context->clientFrame;

        switch(id=GetChunkHdr(context)){
                case PROP: {
                        id=BAD_IFF;
                        break;
                }
                case FORM: {
                        id=(*clientFrame->getForm)(context);
                        break;
                }
                case LIST: {
                        id=(*clientFrame->getList)(context);
                        break;
                }
                case CAT : {
                        id=(*clientFrame->getCat )(context);
                        break;
                }
        }

        return(context->ckHdr.ckID = id);
}

ID GetPChunkHdr(GroupContext *context)
{
        REGISTER ID id;

        id=GetChunkHdr(context);

        switch(id) {
                case LIST:
                case FORM:
                case PROP:
                case CAT : {
                        id=context->ckHdr.ckID=BAD_IFF;
                        break;
                }
        }

        return(id);
}
