/* timing.h */

typedef struct {
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

#ifdef VAX
#define TICK (1000/100)		/* Factor to make ticks to ms */
#else
#define TICK (1000/60)		/* Factor to make ticks to ms */
#endif

typedef enum { TIP_PROC, TIP_CHILD, TIP_REAL } TI_TIME;
typedef enum { TIP_S_F, TIP_M_S, TIP_M_S_F } TI_MODE;


#ifdef _PROTOTYPES_
extern void tistart(TIBUFP tb);
extern void tistop(TIBUFP tb);
extern void tipr(TIBUFP tb, TI_TIME type, TI_MODE mode);
#else
extern void tistart();
extern void tistop();
extern void tipr();
#endif
