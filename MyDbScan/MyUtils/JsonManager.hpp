#pragma once

#include <string>
#include <fstream> // json 로드,저장
#include "../json.hpp"
#include <filesystem>

using json = nlohmann::json;
namespace fs = std::filesystem;


namespace MyUtils::Json
{
	// JSON 파일 로드
	inline bool load ( const std::string& strPath , json& jsonDst , std::string& strErr )
	{
		try
		{
			std::ifstream ( strPath , std::ios::in ) >> jsonDst;
		}
		catch ( ... )
		{
			strErr = std::format ( "\n\t[{}({}): {}]\n\t\tJson Loading Error." , fs::path(__FILE__).filename().string() , __LINE__ , __FUNCSIG__ );
			return false;
		}

		return true;
	}

	// JSON 파일 저장
	inline bool save ( const std::string& strPath , const json& jsonSrc , std::string& strErr )
	{
		try
		{
			// trunc 모드로 저장 // 기존 파일 내용 삭제 후 새로 작성
			std::ofstream ( strPath , std::ios::out | std::ios::trunc ) << jsonSrc.dump ( 2 ) << std::endl;
		}
		catch ( ... )
		{
			strErr = std::format ( "\n\t[{}({}): {}]\n\t\tJson Saving Error." , fs::path(__FILE__).filename().string() , __LINE__ , __FUNCSIG__ );
			return false;
		}
		return true;
	}
};
