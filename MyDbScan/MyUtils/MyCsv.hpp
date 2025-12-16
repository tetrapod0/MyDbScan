#pragma once

#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <format>
#include <chrono>
#include <filesystem>
#include "Timestamp.hpp"


namespace fs = std::filesystem;


namespace MyUtils
{
	class MyCsv
	{
	public:
		MyCsv ( ) {};
		MyCsv ( const std::vector<std::string>& header ) { m_header = header; };
		MyCsv ( const MyCsv& other ) { m_header = other.m_header; };
		~MyCsv ( ) {};


	private:
		std::vector<std::string> m_header;


	public:
		void setHeader ( const std::vector<std::string>& header ) { m_header = header; }


		bool save ( const fs::path& filePath , const std::vector<std::string>& data , bool bAppend = true )
		{
			// 헤더 검사
			if ( m_header.size ( ) == 0 )
				return false;

			// 데이터 컬럼 크기 검사
			if ( data.size ( ) != m_header.size ( ) )
				return false;

			// csv 인지 검사
			if ( filePath.extension ( ) != ".csv" )
				return false;

			// 디렉터리 생성
			fs::path fileDir = filePath.parent_path ( );
			fs::create_directories ( fileDir );

			// 파일 유무 확인
			bool bFileExist = fs::exists ( filePath );

			// 파일 열기
			std::ofstream ofs;
			if ( bAppend )
				ofs.open ( filePath , std::ios::out | std::ios::app );
			else
				ofs.open ( filePath , std::ios::out | std::ios::trunc );

			// 파일 열기 실패
			if ( !ofs.is_open ( ) || !ofs.good ( ) )
				return false;

			// 파일이 없었으면 헤더 작성
			if ( !bFileExist )
			{
				for ( size_t i = 0; i < m_header.size ( ); ++i )
				{
					ofs << m_header[ i ];
					if ( i != m_header.size ( ) - 1 )
						ofs << ",";
				}
				ofs << "\n";
			}

			// 데이터 작성
			for ( size_t i = 0; i < data.size ( ); ++i )
			{
				ofs << data[ i ];
				if ( i != data.size ( ) - 1 )
					ofs << ",";
			}
			ofs << "\n";

			// 파일 닫기
			ofs.close ( );

			return true;
		}


		bool save ( const fs::path& filePath , const std::vector<std::vector<std::string>>& vecData , bool bAppend = true )
		{
			// 헤더 검사
			if ( m_header.size ( ) == 0 )
				return false;

			// 데이터 컬럼 크기 검사
			for ( const auto& data : vecData )
				if ( data.size ( ) != m_header.size ( ) )
					return false;

			// csv 인지 검사
			if ( filePath.extension ( ) != ".csv" )
				return false;

			// 디렉터리 생성
			fs::path fileDir = filePath.parent_path ( );
			fs::create_directories ( fileDir );

			// 파일 유무 확인
			bool bFileExist = fs::exists ( filePath );

			// 파일 열기
			std::ofstream ofs;
			if ( bAppend )
				ofs.open ( filePath , std::ios::out | std::ios::app );
			else
				ofs.open ( filePath , std::ios::out | std::ios::trunc );

			if ( !ofs.is_open ( ) || !ofs.good ( ) )
				return false;

			// 파일이 없었으면 헤더 작성
			if ( !bFileExist )
			{
				for ( size_t i = 0; i < m_header.size ( ); ++i )
				{
					ofs << m_header[ i ];
					if ( i != m_header.size ( ) - 1 )
						ofs << ",";
				}
				ofs << "\n";
			}

			// 데이터 작성
			for ( const auto& data : vecData )
			{
				for ( size_t i = 0; i < data.size ( ); ++i )
				{
					ofs << data[ i ];
					if ( i != data.size ( ) - 1 )
						ofs << ",";
				}
				ofs << "\n";
			}

			// 파일 닫기
			ofs.close ( );

			return true;
		}


		bool load ( const fs::path& filePath , std::vector<std::vector<std::string>>& vecData )
		{
			// 파일 존재 여부
			if ( !fs::exists ( filePath ) )
				return false;

			// 확장자 검사
			if ( filePath.extension ( ) != ".csv" )
				return false;

			// 파일 열기
			std::ifstream ifs ( filePath );
			if ( !ifs.is_open ( ) || !ifs.good ( ) )
				return false;

			// 데이터 읽기
			vecData.clear ( );
			std::string line;
			while ( std::getline ( ifs , line ) )
			{
				std::vector<std::string> row;
				std::stringstream ss ( line );
				std::string cell;
				while ( std::getline ( ss , cell , ',' ) )
				{
					row.push_back ( cell );
				}
				vecData.push_back ( row );
			}
			ifs.close ( );

			return true;
		}
	};
}
