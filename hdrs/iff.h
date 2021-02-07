#if !defined IFF_H
        #define IFF_H

        #if !defined LIBRARIES_DOS_H
                #include <libraries/dos.h>
        #endif

        typedef LONG IFFP;

        #define IFF_OKAY 0
        #define END_MARK -1
        #define IFF_DONE -2

        #define DOS_ERROR       -3
        #define NOT_IFF         -4
        #define NO_FILE         -5
        #define CLIENT_ERROR    -6
        #define BAD_FORM        -7
        #define SHORT_CHUNK     -8
        #define BAD_IFF         -9
        #define LAST_ERROR      BAD_IFF

        #define CheckIFFP()     { if(iffp!=IFF_OKAY) return(iffp); }

        typedef LONG ID;

        #define MakeID(a,b,c,d)  ((a)<<24|(b)<<16|(c)<<8|(d))

        #define FORM    MakeID('F','O','R','M')
        #define PROP    MakeID('P','R','O','P')
        #define LIST    MakeID('L','I','S','T')
        #define CAT     MakeID('C','A','T',' ')

        #define FILLER  MakeID(' ',' ',' ',' ')

        #define NULL_CHUNK 0L


        typedef struct {
                        ID      ckID;
                        LONG    ckSize;
                }
                ChunkHeader;

        typedef struct {
                        ID      ckID;
                        LONG    ckSize;
                        UBYTE ckData[1];
                }
                Chunk;

        #define szNotYetKnown           0x80000001L
        #define IS_ODD(a)               ((a)&1)
        #define WordAlign(size)         ((size+1)&~1)
        #define ChunkPSize(dataSize)    (WordAlign(dataSize)+sizeof(ChunkHeader))

        typedef struct {
                        ID      ckID;
                        LONG    ckSize;
                        ID      grpSubID;
                }
                GroupHeader;

        typedef struct {
                        ID      ckID;
                        LONG    ckSize;
                        ID      grpSubID;
                        UBYTE   grpData[1];
                }
                GroupChunk;

        typedef IFFP ClientProc(struct _GroupContext *);

        typedef struct _ClientFrame {
                        ClientProc      *getList,*getProp,
                                        *getForm,*getCat;
                }
                ClientFrame;

        typedef struct _GroupContext {
                        struct _GroupContext *parent;
                        ClientFrame *clientFrame;
                        BPTR file;
                        LONG position;
                        LONG bound;
                        ChunkHeader ckHdr;
                        ID subtype;
                        LONG bytesSoFar;
                }
                GroupContext;

        #define ChunkMoreBytes(gc) ((gc)->ckHdr.ckSize-(gc)->bytesSoFar)

        extern IFFP     OpenRIFF(BPTR, GroupContext *, ClientFrame *),
                        OpenRGroup(GroupContext *, GroupContext *),
                        CloseRGroup(GroupContext *);
        extern ID       GetChunkHdr(GroupContext *);
        extern IFFP     IFFReadBytes(GroupContext *, BYTE *, LONG),
                        SkipGroup(GroupContext *),
                        ReadIFF(BPTR, ClientFrame *),
                        ReadIList(GroupContext *, ClientFrame *),
                        ReadICat(GroupContext *);
        extern ID       GetFChunkHdr(GroupContext *),
                        GetF1ChunkHdr(GroupContext *),
                        GetPChunkHdr(GroupContext *);

        extern IFFP     OpenWIFF(BPTR, GroupContext *, LONG),
                        StartWGroup(GroupContext *, ID, LONG, ID, GroupContext *),
                        EndWGroup(GroupContext *),
                        OpenWGroup(GroupContext *, GroupContext *),
                        CloseWGroup(GroupContext *),
                        PutCk(GroupContext *, ID, LONG, BYTE *),
                        PutCkHdr(GroupContext *, ID, LONG),
                        IFFWriteBytes(GroupContext *, BYTE *, LONG),
                        PutCkEnd(GroupContext *);
#endif
