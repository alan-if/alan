/* timing.c */

#include <time.h>
#include "sysdep.h"
#include "timing.h"


/* TISTART
 !
 ! Start a phase timing.
 */

#ifdef _PROTOTYPES_
void tistart(TIBUFP tb)
#else
void tistart(tb)
TIBUFP tb;
#endif
{
#ifdef MULTI
    times((struct tms *)tb);
    tb->pu_start = tb->proc_user_time;
    tb->cu_start = tb->child_user_time;
#endif
    time((time_t *)&(tb->real_start));
}


/* TISTOP
 !
 ! Start a phase timing.
 */

#ifdef _PROTOTYPES_
void tistop(TIBUFP tb)
#else
void tistop(tb)
TIBUFP tb;
#endif
{
#ifdef MULTI
    times((struct tms *)tb);
    tb->pu_elapsed = TICK * (tb->proc_user_time - tb->pu_start);
    tb->cu_elapsed = TICK * (tb->child_user_time - tb->cu_start);
#endif
    tb->real_elapsed = time((time_t *)&(tb->real_elapsed)) - tb->real_start;
}


/* TIPR
 !
 ! Print required time.
 */

#ifdef _PROTOTYPES_
void tipr(TIBUFP tb, TI_TIME type, TI_MODE mode)
#else
void tipr(tb, type, mode)
TIBUFP tb;
TI_TIME type;
TI_MODE mode;
#endif
{
    long ms, s, sec, min;

    switch (type) {
	case TIP_REAL:
	    ms= 0;
	    s= tb->real_elapsed;
	    break;
	case TIP_PROC:
	    ms= tb->pu_elapsed % 1000;
	    s=  tb->pu_elapsed / 1000;
	    break;
	case TIP_CHILD:
	    ms= tb->cu_elapsed % 1000;
	    s=  tb->cu_elapsed / 1000;
	    break;
	default:
	    ms= 0;
	    s= tb->real_elapsed;
	    break;
    }
    sec= s % 60;
    min= s / 60;

    switch (mode) {
	case TIP_M_S:
	    printf("%02ld:%02ld", min, sec);
	    break;
	case TIP_M_S_F:
	    printf("%02ld:%02ld.%03ld", min, sec, ms);
	    break;
	case TIP_S_F:
	default:
	    printf("%3ld.%03ld", s, ms);
    }
}

