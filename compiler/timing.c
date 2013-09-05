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
    tb->pu_start = tb->proc_user_time;
    tb->cu_start = tb->child_user_time;
#endif
    time((time_t *)&(tb->real_start));
}


/*======================================================================*/
void tistop(TIBUFP tb)
{
#ifdef MULTI
    times((struct tms *)tb);
    tb->pu_elapsed = TICK * (tb->proc_user_time - tb->pu_start);
    tb->cu_elapsed = TICK * (tb->child_user_time - tb->cu_start);
#endif
    tb->real_elapsed = time((time_t *)&(tb->real_elapsed)) - tb->real_start;
}
