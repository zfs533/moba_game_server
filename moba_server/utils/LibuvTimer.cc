#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "uv.h"
#include "LibuvTimer.h"
#include "logger.h"
#include "timestamp.h"

/*
	void cancel(STimer* timer);
	STimer* scheduleOnce(void(*on_timer)(void* udata),void* data,int after_time);
	void free_timer(STimer* timer);
*/
static LibuvTimer* inst;


LibuvTimer::LibuvTimer()
{
	inst = this;
}

LibuvTimer::~LibuvTimer()
{
	if(inst)
	{
		delete inst;
		inst = NULL;
	}
}

LibuvTimer* LibuvTimer::getInstance()
{
	if(!inst)
	{
		inst = new LibuvTimer();
	}
	return inst;
}
/*
on_timer:timer callback func
data:call data
repeat_count:-1 repeatforever
*/
STimer* LibuvTimer::schedule_repeat(void(*on_timer)(void* udata),void* data,int after_time,int time_offset,int repeat_count)
{
	STimer* t = this->allocTimer(on_timer,data,time_offset,repeat_count);
	t->uv_timer.data = t;
	//开始计时器
	uv_timer_start(&t->uv_timer,timer_cb,after_time,time_offset);
	return t;
}
//malloc STimer buffer
STimer* LibuvTimer::allocTimer(void(*on_timer)(void* udata),void* data,int time_offset,int repeat_count)
{
	STimer* t = (STimer*)malloc(sizeof(STimer));
	memset(t,0,sizeof(struct STimer));
	t->on_Timer = on_timer;
	t->data = data;
	t->repeat_count = repeat_count;
	//初始化计时器
	uv_timer_init(uv_default_loop(),&t->uv_timer);
	return t;
}

//libuv timer callback
void LibuvTimer::timer_cb(uv_timer_t* handle)
{
	STimer* t = (STimer*)handle->data;
	if(t->repeat_count<0)//repeatforever
	{
		t->on_Timer(t->data);
	}
	else
	{
		t->repeat_count--;
		t->on_Timer(t->data);
		if(t->repeat_count == 0)
		{
			//stop timer and free buffer
			uv_timer_stop(&t->uv_timer);
			free(t);
		}
	}
}
//cancel timer
void LibuvTimer::cancel(STimer* timer)
{
	if(timer->repeat_count == 0)//is stoped
	{
		return;
	}
	uv_timer_stop(&timer->uv_timer);
	free_timer(timer);
}

void LibuvTimer::free_timer(STimer* t)
{
	free(t);
}

STimer* LibuvTimer::scheduleOnce(void(*on_timer)(void* udata),void* data,int after_time)
{
	return this->schedule_repeat(on_timer,data,after_time,after_time,1);
}

void* LibuvTimer::get_timer_udata(STimer* t)
{
	return t->data;
}