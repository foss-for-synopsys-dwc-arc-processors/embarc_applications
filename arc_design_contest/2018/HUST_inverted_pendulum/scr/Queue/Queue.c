/*
 *simple queue, if using template, don't need differ char and int  
 */

#include "embARC.h"

#include "Queue.h"
#include <malloc.h>
#include <stdbool.h>

/** [q_init_char: initialize queue (type:char)] */
void q_init_char(Queue *q, int alen)
{
	q->array = malloc(sizeof(char) * alen);
	configASSERT(q->array!=NULL);
	q->num = NULL;
	q->alen = alen;
	q->head = 0;
	q->tail = 0;
}

/** [q_init_char: initialize queue (type:int)] */
void q_init_int(Queue *q, int alen)
{
	q->array = NULL;
	q->num = malloc(sizeof(int) * alen);
	configASSERT(q->num!=NULL);
	q->alen = alen;
	q->head = 0;
	q->tail = 0;
}

/** [q_clr: clear queue, but don't free the memory] */
void q_clr(Queue *q)
{
	q->head = 0;
	q->tail = 0;
} 

/** [q_l: calc queue current length] */
int q_l(Queue *q)
{
	int rtn;
    if(q->tail >= q->head)
    {
        rtn = q->tail - q->head;
    }
    else
    {
        rtn = q->alen + q->tail - q->head;
    }
    return rtn;
}

/** [q_en_char: enqueue (char) until queue is full, ingore next datas] */
bool q_en_char(Queue *q, char c)
{
	bool rtn = false;
	if(q_l(q) < q->alen - 1)
	{
		q->array[q->tail] = c;
		if(q->tail == q->alen - 1)
		{
			q->tail = 0;
		}
		else 
		{
			q->tail++;
		}
		rtn = true;
	}
	return rtn;
}

/** [q_en_int: enqueue (int) until queue is full, ingore next datas] */
bool q_en_int(Queue *q, int c)
{
	bool rtn = false;
	if(q_l(q) < q->alen - 1)
	{
		q->num[q->tail] = c;
		if(q->tail == q->alen - 1)
		{
			q->tail = 0;
		}
		else 
		{
			q->tail++;
		}
		rtn = true;
	}
	return rtn;
}

/** [q_en_char: dequeue (char) until queue is empty, keep final value] */
bool q_de_char(Queue *q, char *c)
{
	bool rtn = false;
	if(q_l(q) > 0)
	{
		*c = q->array[q->head];
		if(q->head == q->alen - 1)
		{
			q->head = 0;
		}
		else 
		{
			q->head++;
		}
		rtn = true;
	}
	return rtn;
}

/** [q_en_int: dequeue (int) until queue is empty, keep final value] */
bool q_de_int(Queue *q, int *c)
{
	bool rtn = false;
	if(q_l(q) > 0)
	{
		*c = q->num[q->head];
		if(q->head == q->alen - 1)
		{
			q->head = 0;
		}
		else 
		{
			q->head++;
		}
		rtn = true;
	}
	return rtn;
}
