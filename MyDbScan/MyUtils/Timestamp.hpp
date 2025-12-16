#pragma once

#include <vector>
#include <chrono>


namespace MyUtils
{
	inline std::vector<int> GetTimestamp ( )
	{
		const auto now = std::chrono::system_clock::now ( );
		const auto ms = std::chrono::duration_cast< std::chrono::milliseconds >(
			now.time_since_epoch ( ) ) % 1000;// .count();
		const int _ms = static_cast< int > ( ms.count ( ) );

		std::time_t t = std::chrono::system_clock::to_time_t ( now );
		std::tm tm;
#ifdef _WIN32
		::localtime_s ( &tm , &t );
#else
		::localtime_r ( &t , &tm );
#endif

		return { tm.tm_year + 1900 , tm.tm_mon + 1 , tm.tm_mday , tm.tm_hour, tm.tm_min, tm.tm_sec, _ms };
	}
}