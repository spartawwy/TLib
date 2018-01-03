// By Xixin Silas Cheng
// time, clock related class
// heavily used, inlined

#ifndef TSYSTEM_TIME_H_
#define TSYSTEM_TIME_H_

#include <chrono>
#include <ctime>
#include <memory>
#include <string>
#include <thread>
#include <tuple>

#include "TLib/core/tsystem_time.pb.h"

namespace TSystem
{
	// forward declear
	class FakeClock; 
	class TError;

	// use class SystemClock to get current timepoint
	// can be set to realtime / faketime mode
	class SystemClock
	{
	public:

		// constructor
		SystemClock();
		~SystemClock();

		// set to fake mode
		// t0: the start time point of fake time
		// speed: speed of time elapse
		void FakeMode(std::chrono::system_clock::time_point t0, unsigned short speed);

		// reset start time point of fake mode
		bool ResetFake();

		// set to real mode
		void RealMode();

		// is real mode
		bool IsReal() const;

		// get current time point
		std::chrono::system_clock::time_point Now() const;

	private:
		std::unique_ptr<FakeClock>  fake_imp_;
	};

	// the default global clock
	SystemClock& GetClock();

	// wrapper class for std::chrono::system_clock::time_point
	class TimePoint
	{
	public:
		// convert a time_point to TimeStamp
		explicit TimePoint( const std::chrono::system_clock::time_point& tpoint);

		// accessors
		inline int	   year()  const { return tm_.tm_year + 1900; }

		inline int	   month() const { return tm_.tm_mon + 1;     } 
		
		inline int	   day()   const { return tm_.tm_mday;        }
		
		inline int	   hour()  const { return tm_.tm_hour;        }
		
		inline int     min()   const { return tm_.tm_min;         }
		
		inline int     sec()   const { return tm_.tm_sec;         }
		
		inline double  frac_sec() const 
		{ 
			return std::chrono::duration_cast<std::chrono::duration<double>>(frac_sec_).count(); 
		}

		int64_t     TimeValue() const { return time_t_; }

		const std::tm* TimeStruct() const { return &tm_; }

		std::string Str() const;

	private:
		int64_t                               time_t_;       
		std::tm								  tm_;
		std::chrono::system_clock::duration   frac_sec_;	// fraction of second
	};

	// make time_point from input, use this to test if is a valid time point
	bool MakeTimePoint( std::chrono::system_clock::time_point* tp, TError& ec, int year, 
			int month, int day, 
			int hour = 0, int min = 0, int sec = 0, int mil = 0);	

	// throw TException version
	std::chrono::system_clock::time_point MakeTimePoint( int year, 
			int month, int day, 
			int hour = 0, int min = 0, int sec = 0, int mil = 0);

	// make time_point from time_t and fraction of second
	std::chrono::system_clock::time_point MakeTimePoint( time_t a, double b);

	// Performance count class
	class TimeCount
	{
	public:
		TimeCount();

		// start count
		void Start();

		// number of ms seconds
		double Count() const;

	private:
		std::chrono::system_clock::time_point	start_;
	};

	// longdate to component
	typedef std::tuple<int,int,int> DateComponent;
	DateComponent FromLongdate(int longdate);

	// componenet to longdate
	int  ToLongdate(int yyyy, int mm, int dd);

	// today 
	int  Today();

	// wait for condition
	template <typename ConditionPred>
	bool WaitFor(ConditionPred pred, size_t time_out)
	{
		TimeCount counter;
		counter.Start();
		while( counter.Count() < time_out )
		{ 
			if( pred() ) 
				return true;
			else
				std::this_thread::yield();
		}
		return false;
	}

	template <typename ConditionPred, typename ConditionVar>
	bool WaitFor( const ConditionVar& var,  ConditionPred pred, size_t time_out)
	{
		TimeCount counter;
		counter.Start();
		while( counter.Count() < time_out )
		{ 
			if( pred(var) ) 
				return true;
			else
				std::this_thread::yield();
		}
		return false;
	}

	//-------------------------------
	// free functions for class Time
	//-------------------------------
	// fill network time message
	void FillTime(const TimePoint& t, Time& msg);
	
	// output network time message
	std::string ToString(const Time& t);
}

#endif // TSYSTEM_TIME_H_