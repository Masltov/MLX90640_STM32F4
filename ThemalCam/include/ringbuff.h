/*
 * ringbuff.h
 *
 *  Created on: 21.03.2020
 *      Author: masltov
 */
#ifdef __cplusplus
extern "C" {
#endif
#ifndef INCLUDE_RINGBUFF_H_
#define INCLUDE_RINGBUFF_H_


/**
 * DO NOT USE STRUCT DIRECTLY
 * Instead use Get16Ringbuffer()
 */
typedef struct ringBuff
{
	float 	buffer[16];
	int 	bufSize;
	float 	mvgAvg;
} RingBuffer;

RingBuffer Get16Ringbuffer(void);
void 	rb_addValue(RingBuffer *self, float newVal);
void 	rb_calcMvgAvg(RingBuffer *self);
float 	getMvAvg(RingBuffer *self);

#endif /* INCLUDE_RINGBUFF_H_ */


#ifdef __cplusplus
}
#endif
