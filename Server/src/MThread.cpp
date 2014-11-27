/*
 * Mthread.cpp
 *
 *  Created on: Feb 13, 2013
 *      Author: Eliav Menachi
 */

#include "MThread.h"
#include <iostream>
#include <stdio.h>
using namespace std;

void* worker(void* arg){
	MThread* threadObj = (MThread*)arg;
	threadObj->run();
	threadObj->threadId = 0;
	return NULL;
}

void MThread::start(){
	pthread_create(&threadId,NULL,worker,(void*)this);
}

void MThread::waitForThread(){
	pthread_join(threadId,NULL);
	threadId=0;
}

MThread::~MThread(){
	if (threadId>0){
		pthread_cancel(threadId);

	}
}

