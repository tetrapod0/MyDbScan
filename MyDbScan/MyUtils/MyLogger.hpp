#pragma once

#include <vector>
#include <chrono>
#include <filesystem>
#include <format>
#include <utility>
#include <mutex>
#include "Timestamp.hpp"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"


namespace fs = std::filesystem;


namespace MyUtils
{
	class MyLogger
	{
	public:
		MyLogger ( )
		{
			m_fileLogger = spdlog::basic_logger_mt ( "file_logger" , "log.txt" );
			spdlog::set_pattern ( "[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v" ); // "[2023-10-05 14:23:45.123] [info] message"
			spdlog::flush_every ( std::chrono::seconds ( 3 ) ); // Flush every 3 seconds
			spdlog::set_level ( m_iLogLevel ); // Set global log level to debug
		}
		~MyLogger ( )
		{
			m_fileLogger->flush ( );
		}


	private:
		std::shared_ptr<spdlog::logger> m_fileLogger = nullptr; // spdlog 파일 로거
		std::vector<int> m_vecCurrentHours; // 현재 시간대 // year, month, day, hour
		std::string m_strOutputDir = "./output";
		spdlog::level::level_enum m_iLogLevel = spdlog::level::debug;
		std::mutex m_mutex; // 스레드 안전을 위한 뮤텍스


	public:
		void setLevel ( spdlog::level::level_enum level )
		{
			m_iLogLevel = level;
			spdlog::set_level ( m_iLogLevel );
			m_fileLogger->set_level ( m_iLogLevel );
		}


		void setOutputDir ( const std::string& dir ) { m_strOutputDir = dir; }


		std::vector<int> getCurrentLogHours ( ) const { return m_vecCurrentHours; }


		fs::path mkdirWithCurrentHours ( std::vector<int> vecTimeNow = {} )
		{
			if ( vecTimeNow.empty ( ) )
				vecTimeNow = MyUtils::GetTimestamp ( ); // { 년 , 월 , 일 , 시 , 분 , 초 , 밀리초 }

			// hour까지만 사용
			auto vecTimeNowHours = vecTimeNow;
			vecTimeNowHours.resize ( 4 ); // year, month, day, hour

			// 저장 디렉토리 경로
			fs::path pathDir = fs::path ( m_strOutputDir ) / std::format ( "{:04d}/{:02d}/{:02d}/{:02d}" , vecTimeNowHours[ 0 ] , vecTimeNowHours[ 1 ] , vecTimeNowHours[ 2 ] , vecTimeNowHours[ 3 ] );

			// 시간이 바뀌었으면
			if ( m_vecCurrentHours != vecTimeNowHours )
			{
				// 디렉토리 생성
				pathDir = fs::path ( m_strOutputDir ) / std::format ( "{:04d}/{:02d}/{:02d}/{:02d}" , vecTimeNowHours[ 0 ] , vecTimeNowHours[ 1 ] , vecTimeNowHours[ 2 ] , vecTimeNowHours[ 3 ] );
				if ( !fs::exists ( pathDir ) )
					fs::create_directories ( pathDir );

				// 로그 파일 경로 재설정
				fs::path current_path = pathDir / std::format ( "Log_{:04d}{:02d}{:02d}_{:02d}.txt" , vecTimeNowHours[ 0 ] , vecTimeNowHours[ 1 ] , vecTimeNowHours[ 2 ] , vecTimeNowHours[ 3 ] );
				auto new_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt> ( current_path.string ( ) , false );
				m_fileLogger->sinks ( ).clear ( );
				m_fileLogger->sinks ( ).push_back ( new_sink );
				m_fileLogger->set_pattern ( "[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v" ); // "[2023-10-05 14:23:45.123] [info] message"
				m_fileLogger->set_level ( m_iLogLevel );

				// 현재 시간대 갱신
				m_vecCurrentHours = vecTimeNowHours;
			}

			return pathDir;
		}


		template<typename... Args>
		void debug ( std::format_string<Args...> fmt , Args&&... args )
		{
			std::lock_guard<std::mutex> lock ( m_mutex ); // 스레드 안전을 위한 뮤텍스 잠금
			if ( m_fileLogger ) {
				this->mkdirWithCurrentHours ( );
				m_fileLogger->debug ( std::format ( fmt , std::forward<Args> ( args )... ) );
			}
		}


		template<typename... Args>
		void info ( std::format_string<Args...> fmt , Args&&... args )
		{
			std::lock_guard<std::mutex> lock ( m_mutex ); // 스레드 안전을 위한 뮤텍스 잠금
			if ( m_fileLogger ) {
				this->mkdirWithCurrentHours ( );
				m_fileLogger->info ( std::format ( fmt , std::forward<Args> ( args )... ) );
			}
		}


		template<typename... Args>
		void warn ( std::format_string<Args...> fmt , Args&&... args )
		{
			std::lock_guard<std::mutex> lock ( m_mutex ); // 스레드 안전을 위한 뮤텍스 잠금	
			if ( m_fileLogger ) {
				this->mkdirWithCurrentHours ( );
				m_fileLogger->warn ( std::format ( fmt , std::forward<Args> ( args )... ) );
				m_fileLogger->flush ( );
			}
		}


		template<typename... Args>
		void error ( std::format_string<Args...> fmt , Args&&... args )
		{
			std::lock_guard<std::mutex> lock ( m_mutex ); // 스레드 안전을 위한 뮤텍스 잠금
			if ( m_fileLogger ) {
				this->mkdirWithCurrentHours ( );
				m_fileLogger->error ( std::format ( fmt , std::forward<Args> ( args )... ) );
				m_fileLogger->flush ( );
			}
		}
	};
}
