#if !defined PACKER_H
        #define PACKER_H

        #define MaxPackedSize(rowSize)  ((rowSize)+(((rowSize)+127)>>7))

        extern LONG PackRow(BYTE **, BYTE **, LONG);
        extern BOOL UnPackRow(BYTE **, BYTE **, LONG, LONG);
#endif
