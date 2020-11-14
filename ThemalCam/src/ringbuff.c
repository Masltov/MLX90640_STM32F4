/*
 * ringbuff.c
 *
 *  Created on: 21.03.2020
 *      Author: masltov
 */


#include "ringbuff.h"

RingBuffer Get16Ringbuffer(void)
{
    struct ringBuff rBuff;
    rBuff.bufSize = 0;
    return rBuff;
}

void rb_addValue(RingBuffer *rBuff,  float newVal)
{
	if(rBuff->bufSize==16)
	{
		for(int i =0; i< 15; i++)
		{
			rBuff->buffer[i] = rBuff->buffer[i+1];
		}
		rBuff->buffer[15] = newVal;
	}
	else
	{
		rBuff->buffer[rBuff->bufSize] = newVal;
		rBuff->bufSize++;
	}
	rb_calcMvgAvg(rBuff);
}

float rb_getMvAvg(RingBuffer* rBuff)
{
	return rBuff->mvgAvg;
}

void rb_calcMvgAvg(RingBuffer* rBuff)
{
	float buf =0.0;
	for(int i= 0; i<rBuff->bufSize; i++)
	{
		buf += rBuff->buffer[i];
	}
	rBuff->mvgAvg = buf/rBuff->bufSize;
}
