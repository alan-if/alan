#ifndef SCO_H
#define SCO_H
/*----------------------------------------------------------------------*\

				SCO.H
				Scores
                                                                     
\*----------------------------------------------------------------------*/




/* Types: */

/* Data: */

extern int scocnt;
extern int scotot;



/* Functions: */
#ifdef _PROTOTYPES_

/* Prepare a score table */
extern void prepscos(void);

/* Generate the score table */
extern Aaddr gesco(void);

#else
extern void prepscos();
extern Aaddr gesco();
#endif

#endif
