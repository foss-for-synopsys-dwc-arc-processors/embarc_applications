#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <stdbool.h>

typedef struct Queue
{
	char *array;
	int *num;
	int alen;
	int head;
	int tail;
}Queue;

extern void q_init_char(Queue *q, int alen);
extern void q_init_int(Queue *q, int alen);
extern void q_clr(Queue *q);
extern int q_l(Queue *q);
extern bool q_en_char(Queue *q, char c);
extern bool q_en_int(Queue *q, int c);
extern bool q_de_char(Queue *q, char *c);
extern bool q_de_int(Queue *q, int *c);

#endif