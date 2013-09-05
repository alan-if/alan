/* timing.c */

#include "sysdep.h"
#include "timing.h"

#include <time.h>


/*======================================================================*/
void tistart(TIBUFP tb)
{
    // TODO Remove times() since it is obsolete, use gettimeofday instead
#ifdef MULTI
    times((struct tms *)tb);
    tb->pu_start = tb->tms.tms_utime;
    tb->cu_start = tb->tms.tms_cutime;
#endif
    time((time_t *)&(tb->real_start));
}


/*======================================================================*/
void tistop(TIBUFP tb)
{
#ifdef MULTI
    times((struct tms *)tb);
    tb->pu_elapsed = TICK * (tb->tms.tms_utime - tb->pu_start);
    tb->cu_elapsed = TICK * (tb->tms.tms_cutime - tb->cu_start);
#endif
    tb->real_elapsed = time((time_t *)&(tb->real_elapsed)) - tb->real_start;
}
