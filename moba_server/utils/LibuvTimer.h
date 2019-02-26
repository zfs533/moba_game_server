#ifndef __LIBUVTIMER_H
#define __LIBUVTIMER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "uv.h"
//计时器数据结构
struct STimer
{
	uv_timer_t uv_timer;//timer句柄
	void(*on_Timer)(void* udata);//带参数回调函数
	void* data;//发送的数据
	int repeat_count;//重复次数，-1表示一直循环
};

class LibuvTimer
{
public:
	LibuvTimer();
	~LibuvTimer();
	static LibuvTimer* getInstance();
	STimer* schedule_repeat(void(*on_timer)(void* udata),void* data,int after_time,int time_offset,int repeat_count);
	STimer* allocTimer(void(*on_timer)(void* udata),void* data,int time_offset,int repeat_count);
	static void timer_cb(uv_timer_t* handle);
	void cancel(STimer* timer);
	STimer* scheduleOnce(void(*on_timer)(void* udata),void* data,int after_time);
	void free_timer(STimer* timer);
	void* get_timer_udata(STimer* t);
};
#ifdef __cplusplus
}
#endif
#endif