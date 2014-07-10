#ifndef _TIMER_
#define _TIMER_

class Timer
{
public:
	
	Timer();

	~Timer(){}

	void Reset();
	
	float tick();//�������ε���֮�����ĵ�ʱ��

	BOOL highResCounterSupported;
	
	float freq;  // high frequency system counts per second
	
	LONGLONG curr, prev;
	// for low res timer if high res is unavailable
	DWORD lowResCurr, lowResPrev;

	float time;
	
};


#endif

