#include <exec/types.h>
#include <proto/mathieeedoubbas.h>
#include "hdrs/mand.int.d.h"
/*------------------------------------------------------------------------*/
LONG ieee2int(DOUBLE ieee)
{
        return((LONG)(ieee*(DOUBLE)FUDGE));
}
