#ifndef _TIMER_
#define _TIMER_

class Timer
{
public:
	
	Timer();

	~Timer(){}

	void Reset();
	
	float tick();//返回两次调用之间消耗的时间

	BOOL highResCounterSupported;
	
	float freq;  // high frequency system counts per second
	
	LONGLONG curr, prev;
	// for low res timer if high res is unavailable
	DWORD lowResCurr, lowResPrev;

	float time;
	
};


#endif

