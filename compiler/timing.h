/* timing.h */

#include <sys/times.h>

typedef struct {
    struct tms tms;
        long proc_user_time;	/* ticks */
        long proc_system_time;	/* ticks */
        long child_user_time;	/* ticks */
        long child_system_time;	/* ticks */
        long pu_start;		/* ticks */
        long pu_elapsed;	/* ms */
        long cu_start;		/* ticks */
        long cu_elapsed;	/* ms */
        long real_start;	/* s */
        long real_elapsed;	/* s */
    } TIBUF, *TIBUFP;

#define TICK (1000/60)		/* Factor to make ticks to ms */

typedef enum { TIP_PROC, TIP_CHILD, TIP_REAL } TI_TIME;


extern void tistart(TIBUFP tb);
extern void tistop(TIBUFP tb);
