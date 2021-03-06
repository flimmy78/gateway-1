/*
 * =============================================================================
 *
 *       Filename:  Queue.c
 *
 *    Description:  队列
 *
 *        Version:  1.0
 *        Created:  2016-08-16 09:08:15
 *       Revision:  none
 *
 *         Author:  xubin
 *        Company:  Taichuan
 *
 * =============================================================================
 */
/* ---------------------------------------------------------------------------*
 *                      include head files
 *----------------------------------------------------------------------------*/

/* ---------------------------------------------------------------------------*
 *                  extern variables declare
 *----------------------------------------------------------------------------*/
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <pthread.h>

#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <semaphore.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "queue.h"


/* ---------------------------------------------------------------------------*
 *                  internal functions declare
 *----------------------------------------------------------------------------*/

/* ---------------------------------------------------------------------------*
 *                        macro define
 *----------------------------------------------------------------------------*/
#define QUEUE_LOCK()   pthread_mutex_lock(&This->priv->mutex)
#define QUEUE_UNLOCK() pthread_mutex_unlock(&This->priv->mutex)

#define saveLog( ... ) printf( __VA_ARGS__ )
typedef struct _QueuePriv {
	QueueType type;
	unsigned int size;
	// mqd_t queue; // rtos
	int queue; // linux
	void *buf[MAX_COMMAND_QUEUE_SIZE];

	int current_length; // 当前fifo已存储的长度
	int index_post;				//索引
	int index_get;				//索引

	pthread_mutex_t mutex;		//队列控制互斥信号
	pthread_mutexattr_t mutexattr2;

}QueuePriv;
/* ---------------------------------------------------------------------------*
 *                      variables define
 *----------------------------------------------------------------------------*/



/* ----------------------------------------------------------------*/
/**
 * 线程操作时信号量处理
 * @brief my_sem_post_get
 * @brief my_sem_wait_get
 *
 * @param This
 */
/* ----------------------------------------------------------------*/
static void semPost(Queue * This)
{
	// sem_post(This->priv->sem);
}
static void semWait(Queue * This)
{
	// sem_wait(This->priv->sem);
}

//----------------------------------------------------------------------------
static void destroy(Queue * This)
{
	unsigned int i;
	// sem_destroy(This->priv->sem);
	for(i=0;i<MAX_COMMAND_QUEUE_SIZE;i++) {
		if (This->priv->buf[i]) {
			free(This->priv->buf[i]);
			This->priv->buf[i] = NULL;
		}
	}
	// free(This->priv->sem);
	pthread_mutex_destroy (&This->priv->mutex);
	free(This->priv);
	free(This);
}

//----------------------------------------------------------------------------
static void queuePost(Queue * This,void *data)
{
	QUEUE_LOCK();
	msgsnd(This->priv->queue, data, This->priv->size, 0);
	QUEUE_UNLOCK();
}

static int queueGet(Queue *This,void *data)
{
	return msgrcv(This->priv->queue, data, This->priv->size, 0, 0);
}

/* ----------------------------------------------------------------*/
/**
 * @brief queueCreate 创建对列
 *
 * @param Size 参数大小
 *
 * @returns
 */
/* ----------------------------------------------------------------*/
Queue * queueCreate(char *queue_name,QueueType type,unsigned int Size)
{
	Queue * This = (Queue *)calloc(1,sizeof(Queue));
	if(This == NULL) {
		saveLog("fifo calloc failed !!!\n");
		return NULL;
	}
	This->priv = (QueuePriv *)calloc(1,sizeof(QueuePriv));
	if(This == NULL) {
		saveLog("fifo priv calloc failed !!!\n");
		free(This);
		return NULL;
	}
	This->priv->type = type;
	//设置互斥锁属性
	pthread_mutexattr_init(&This->priv->mutexattr2);
	/* Set the mutex as a recursive mutex */
	pthread_mutexattr_settype(&This->priv->mutexattr2,
			PTHREAD_MUTEX_RECURSIVE_NP); // linux
	// PTHREAD_MUTEX_RECURSIVE); // rtos
	/* create the mutex with the attributes set */
	pthread_mutex_init(&This->priv->mutex, &This->priv->mutexattr2);
	/* destroy the attribute */
	pthread_mutexattr_destroy(&This->priv->mutexattr2);

	This->priv->queue= msgget((key_t)1234, 0666  | IPC_CREAT);
	if (This->priv->queue == -1)
		perror("msg_id");

	This->priv->size = Size;

	This->post = queuePost;
	This->get = queueGet;
	This->destroy = destroy;

	return This;
}
