#include"stdafx.h"

#include<mmsystem.h>
#include"Timer.h"
Timer::Timer()
{
  time = 0.0f;
  // init high- and low-res timers
  LARGE_INTEGER n[1];
  highResCounterSupported = QueryPerformanceFrequency(n);
  freq = 1.0f / float(n[0].QuadPart);
  timeBeginPeriod(1);  // make Sleep() and timeGetTime() more accurate
  // get first tick for high- and low-res timers
  QueryPerformanceCounter(n);
  curr = n[0].QuadPart;
  lowResCurr = timeGetTime();
}


float Timer::tick()//返回两次调用之间消耗的时间
	{
		if(highResCounterSupported)
		{
			prev = curr;
			LARGE_INTEGER n[1];
			QueryPerformanceCounter(n);
			curr = n[0].QuadPart;
			if(curr >= prev) 
				time = float(curr - prev) * freq;
			// else use time from last frame
		}
		else{
			lowResPrev = lowResCurr;
			lowResCurr = timeGetTime();
			if(lowResCurr >= lowResPrev) 
				time = float(lowResCurr - lowResPrev) * 0.001f;
			// else use time from last frame
		}
		return time;
	}

void Timer::Reset()
{
	LARGE_INTEGER n[1];
  QueryPerformanceCounter(n);
  curr = n[0].QuadPart;
  prev =  curr ;
  lowResPrev = lowResCurr ;

}