#pragma once

// 필요할 때 수정~

#include <string>
#include <fstream> // json 로드,저장
#include "../json.hpp"
#include <filesystem>


using json = nlohmann::json;
namespace fs = std::filesystem;


//////////////////////////
// Setting JSON 키 목록 //
//////////////////////////
#define K_CAMERA               "camera"
#define K_IP                   "ip"
#define K_EXPOSURE_TIME        "exposure_time"
#define K_GAIN                 "gain"
#define K_TRIGGER_DELAY_TIME   "trigger_delay_time"
#define K_WHITE_BALANCE        "white_balance"

#define K_YOCTO                "yocto"
#define K_IO_1                 "io_1"
#define K_IO_2                 "io_2"
#define K_ENABLE               "enable"
#define K_SN                   "S/N"
#define K_OUT_PIN              "output_pin"
#define K_IN_PIN               "input_pin"
#define K_LIGHT_CH1            "light_ch1"
#define K_LIGHT_CH2            "light_ch2"
#define K_LIGHT_CH3            "light_ch3"
#define K_LIGHT_CH4            "light_ch4"
#define K_HEATER_SWITCH        "heater_switch"
#define K_MCS_DONE             "mcs_done"
#define K_LIGHT_MANUAL         "light_manual"
#define K_HEATER_ALARM         "heater_alarm"
#define K_LIGHT_ALARM          "light_alarm"
#define K_MCS_ERROR            "mcs_error"


#define K_LOOP                 "loop"

#define K_SENSOR               "sensor"

#define K_ETC                  "etc"
#define K_DEBUG                "debug"
#define K_AUTO_SORT_LIGHT_CH   "auto_sort_light_ch"
#define K_AUTO_CALIBRATION_WB  "auto_calibration_wb"
#define K_OUTPUT_DIR           "output_dir"
#define K_VESSEL_MAX_L         "vessel_max_L"
#define K_LEN_GRAPH            "len_graph"
#define K_RESULT_FONT_SCALE    "result_font_scale"
#define K_SEND_PERIOD	       "send_period"
#define K_SHOW_PERIOD          "show_period"
#define K_CAM_TEMP_LIMIT	   "camera_temp_limit"

////////////////////////
// Model JSON 키 목록 //
////////////////////////
#define K_BLUR_SIZE        "blur_size"
#define K_KERNEL_SIZE      "kernel_size"
#define K_ERODE_ITER       "erode_iter"
#define K_DILATE_ITER      "dilate_iter"
#define K_HSV_S_THRESH     "hsv_s_thresh"
#define K_HSV_V_THRESH     "hsv_v_thresh"

#define K_INNER_CIRCLE_R   "inner_circle_r"
#define K_INNER_CIRCLE_X   "inner_circle_x"
#define K_INNER_CIRCLE_Y   "inner_circle_y"

#define K_OUTER_CIRCLE_R   "outer_circle_r"
#define K_OUTER_CIRCLE_X   "outer_circle_x"
#define K_OUTER_CIRCLE_Y   "outer_circle_y"

#define K_FOAM_RATIO_THRESH  "foam_ratio_thresh"

//////////////////////////////////////////////////////////////////////////

namespace MyUtils::Json
{
	// 설정 JSON 검증
	inline bool validateSetting ( const json& jsonSrc , std::string& strErr )
	{
		try
		{
			// ===== camera =====
			if ( !jsonSrc.contains ( K_CAMERA ) || !jsonSrc[ K_CAMERA ].is_object ( ) )
				throw std::runtime_error ( std::format ( 
					"\n\t[{}({}): {}]\n\t\tMissing '{}'' settings." , 
					fs::path ( __FILE__ ).filename ( ).string ( ) , __LINE__ , __FUNCSIG__ , K_CAMERA ) );
			const auto& cam = jsonSrc[ K_CAMERA ];

			// camera.ip : string
			if ( !cam.contains ( K_IP ) || !cam[ K_IP ].is_string ( ) )
				throw std::runtime_error ( std::format ( 
					"\n\t[{}({}): {}]\n\t\tInvalid type for 'jsonSetting.{}.{}'. Expected string." ,
					fs::path ( __FILE__ ).filename ( ).string ( ) , __LINE__ , __FUNCSIG__ , K_CAMERA , K_IP ) );

			// camera.exposure_time : number
			if ( !cam.contains ( K_EXPOSURE_TIME ) || !cam[ K_EXPOSURE_TIME ].is_number ( ) )
				throw std::runtime_error ( std::format ( 
					"\n\t[{}({}): {}]\n\t\tInvalid type for 'jsonSetting.{}.{}'. Expected number." ,
					fs::path ( __FILE__ ).filename ( ).string ( ) , __LINE__ , __FUNCSIG__ , K_CAMERA , K_EXPOSURE_TIME ) );

			// camera.gain : number
			if ( !cam.contains ( K_GAIN ) || !cam[ K_GAIN ].is_number ( ) )
				throw std::runtime_error ( std::format ( 
					"\n\t[{}({}): {}]\n\t\tInvalid type for 'jsonSetting.{}.{}'. Expected number." ,
					fs::path ( __FILE__ ).filename ( ).string ( ) , __LINE__ , __FUNCSIG__ , K_CAMERA , K_GAIN ) );

			// camera.trigger_delay_time : number
			if ( !cam.contains ( K_TRIGGER_DELAY_TIME ) || !cam[ K_TRIGGER_DELAY_TIME ].is_number ( ) )
				throw std::runtime_error ( std::format ( 
					"\n\t[{}({}): {}]\n\t\tInvalid type for 'jsonSetting.{}.{}'. Expected number." ,
					fs::path ( __FILE__ ).filename ( ).string ( ) , __LINE__ , __FUNCSIG__ , K_CAMERA , K_TRIGGER_DELAY_TIME ) );

			// camera.white_balance : array[3] of number
			if ( !cam.contains ( K_WHITE_BALANCE ) || !cam[ K_WHITE_BALANCE ].is_array ( ) )
				throw std::runtime_error ( std::format ( 
					"\n\t[{}({}): {}]\n\t\tInvalid type for 'jsonSetting.{}.{}'. Expected array." ,
					fs::path ( __FILE__ ).filename ( ).string ( ) , __LINE__ , __FUNCSIG__ , K_CAMERA , K_WHITE_BALANCE ) );
			if ( cam[ K_WHITE_BALANCE ].size ( ) != 3 )
				throw std::runtime_error ( std::format ( 
					"\n\t[{}({}): {}]\n\t\tInvalid size for 'jsonSetting.{}.{}'. Expected 3 elements." ,
					fs::path ( __FILE__ ).filename ( ).string ( ) , __LINE__ , __FUNCSIG__ , K_CAMERA , K_WHITE_BALANCE ) );
			for ( size_t i = 0; i < 3; ++i )
			{
				if ( !cam[ K_WHITE_BALANCE ][ i ].is_number ( ) )
					throw std::runtime_error ( std::format ( 
						"\n\t[{}({}): {}]\n\t\tInvalid element type for 'jsonSetting.{}.{}[{}]'. Expected number." ,
						fs::path ( __FILE__ ).filename ( ).string ( ) , __LINE__ , __FUNCSIG__ , K_CAMERA , K_WHITE_BALANCE , i ) );
				if ( cam[ K_WHITE_BALANCE ][ i ].get<double> ( ) < 0 )
					throw std::runtime_error ( std::format ( 
						"\n\t[{}({}): {}]\n\t\tNegative value for 'jsonSetting.{}.{}[{}]'. Expected non-negative number." ,
						fs::path ( __FILE__ ).filename ( ).string ( ) , __LINE__ , __FUNCSIG__ , K_CAMERA , K_WHITE_BALANCE , i ) );
			}


			// ===== yocto =====
			if ( !jsonSrc.contains ( K_YOCTO ) || !jsonSrc[ K_YOCTO ].is_object ( ) )
				throw std::runtime_error ( std::format ( 
					"\n\t[{}({}): {}]\n\t\tMissing '{}'' settings." , 
					fs::path ( __FILE__ ).filename ( ).string ( ) , __LINE__ , __FUNCSIG__ , K_YOCTO ) );
			const auto& yocto = jsonSrc[ K_YOCTO ];

			// yocto.io_1
			if ( !yocto.contains ( K_IO_1 ) || !yocto[ K_IO_1 ].is_object ( ) )
				throw std::runtime_error ( std::format ( 
					"\n\t[{}({}): {}]\n\t\tMissing '{}.{}' settings." , 
					fs::path ( __FILE__ ).filename ( ).string ( ) , __LINE__ , __FUNCSIG__ , K_YOCTO , K_IO_1 ) );

			// yocto.io_1.enable : boolean
			if ( !yocto[ K_IO_1 ].contains ( K_ENABLE ) || !yocto[ K_IO_1 ][ K_ENABLE ].is_boolean ( ) )
				throw std::runtime_error ( std::format ( 
					"\n\t[{}({}): {}]\n\t\tInvalid type for '{}.{}.{}'. Expected boolean." ,
					fs::path ( __FILE__ ).filename ( ).string ( ) , __LINE__ , __FUNCSIG__ , K_YOCTO , K_IO_1 , K_ENABLE ) );

			// yocto.io_1.S/N : string
			if ( !yocto[ K_IO_1 ].contains ( K_SN ) || !yocto[ K_IO_1 ][ K_SN ].is_string ( ) )
				throw std::runtime_error ( std::format ( 
					"\n\t[{}({}): {}]\n\t\tInvalid type for '{}.{}.{}'. Expected string." ,
					fs::path ( __FILE__ ).filename ( ).string ( ) , __LINE__ , __FUNCSIG__ , K_YOCTO , K_IO_1 , K_SN ) );

			// yocto.io_1.output_pin
			if ( !yocto[ K_IO_1 ].contains ( K_OUT_PIN ) || !yocto[ K_IO_1 ][ K_OUT_PIN ].is_object ( ) )
				throw std::runtime_error ( std::format ( 
					"\n\t[{}({}): {}]\n\t\tMissing '{}.{}.{}' settings." ,
					fs::path ( __FILE__ ).filename ( ).string ( ) , __LINE__ , __FUNCSIG__ , K_YOCTO , K_IO_1 , K_OUT_PIN ) );

			// yocto.io_1.input_pin
			if ( !yocto[ K_IO_1 ].contains ( K_IN_PIN ) || !yocto[ K_IO_1 ][ K_IN_PIN ].is_object ( ) )
				throw std::runtime_error ( std::format ( 
					"\n\t[{}({}): {}]\n\t\tMissing '{}.{}.{}' settings." ,
					fs::path ( __FILE__ ).filename ( ).string ( ) , __LINE__ , __FUNCSIG__ , K_YOCTO , K_IO_1 , K_IN_PIN ) );

			// yocto.io_2
			if ( !yocto.contains ( K_IO_2 ) || !yocto[ K_IO_2 ].is_object ( ) )
				throw std::runtime_error ( std::format ( 
					"\n\t[{}({}): {}]\n\t\tMissing '{}.{}' settings." , 
					fs::path ( __FILE__ ).filename ( ).string ( ) , __LINE__ , __FUNCSIG__ , K_YOCTO , K_IO_2 ) );

			// yocto.io_2.enable : boolean
			if ( !yocto[ K_IO_2 ].contains ( K_ENABLE ) || !yocto[ K_IO_2 ][ K_ENABLE ].is_boolean ( ) )
				throw std::runtime_error ( std::format ( 
					"\n\t[{}({}): {}]\n\t\tInvalid type for '{}.{}.{}'. Expected boolean." ,
					fs::path ( __FILE__ ).filename ( ).string ( ) , __LINE__ , __FUNCSIG__ , K_YOCTO , K_IO_2 , K_ENABLE ) );

			// yocto.io_2.S/N : string
			if ( !yocto[ K_IO_2 ].contains ( K_SN ) || !yocto[ K_IO_2 ][ K_SN ].is_string ( ) )
				throw std::runtime_error ( std::format ( 
					"\n\t[{}({}): {}]\n\t\tInvalid type for '{}.{}.{}'. Expected string." ,
					fs::path ( __FILE__ ).filename ( ).string ( ) , __LINE__ , __FUNCSIG__ , K_YOCTO , K_IO_2 , K_SN ) );

			// yocto.io_2.output_pin
			if ( !yocto[ K_IO_2 ].contains ( K_OUT_PIN ) || !yocto[ K_IO_2 ][ K_OUT_PIN ].is_object ( ) )
				throw std::runtime_error ( std::format ( 
					"\n\t[{}({}): {}]\n\t\tMissing '{}.{}.{}' settings." ,
					fs::path ( __FILE__ ).filename ( ).string ( ) , __LINE__ , __FUNCSIG__ , K_YOCTO , K_IO_2 , K_OUT_PIN ) );

			// yocto.io_2.input_pin
			if ( !yocto[ K_IO_2 ].contains ( K_IN_PIN ) || !yocto[ K_IO_2 ][ K_IN_PIN ].is_object ( ) )
				throw std::runtime_error ( std::format ( 
					"\n\t[{}({}): {}]\n\t\tMissing '{}.{}.{}' settings." ,
					fs::path ( __FILE__ ).filename ( ).string ( ) , __LINE__ , __FUNCSIG__ , K_YOCTO , K_IO_2 , K_IN_PIN ) );

#if 1
			// yocto.io_1.output_pin.light_ch1 : integer
			if ( !yocto[ K_IO_1 ][ K_OUT_PIN ].contains ( K_LIGHT_CH1 ) || !yocto[ K_IO_1 ][ K_OUT_PIN ][ K_LIGHT_CH1 ].is_number_integer ( ) )
				throw std::runtime_error ( std::format ( 
					"\n\t[{}({}): {}]\n\t\tInvalid or missing '{}.{}.{}.{}'. Expected integer." ,
					fs::path ( __FILE__ ).filename ( ).string ( ) , __LINE__ , __FUNCSIG__ , K_YOCTO , K_IO_1 , K_OUT_PIN , K_LIGHT_CH1 ) );

			// yocto.io_1.output_pin.light_ch2 : integer
			if ( !yocto[ K_IO_1 ][ K_OUT_PIN ].contains ( K_LIGHT_CH2 ) || !yocto[ K_IO_1 ][ K_OUT_PIN ][ K_LIGHT_CH2 ].is_number_integer ( ) )
				throw std::runtime_error ( std::format ( 
					"\n\t[{}({}): {}]\n\t\tInvalid or missing '{}.{}.{}.{}'. Expected integer." ,
					fs::path ( __FILE__ ).filename ( ).string ( ) , __LINE__ , __FUNCSIG__ , K_YOCTO , K_IO_1 , K_OUT_PIN , K_LIGHT_CH2 ) );

			// yocto.io_1.output_pin.light_ch3 : integer
			if ( !yocto[ K_IO_1 ][ K_OUT_PIN ].contains ( K_LIGHT_CH3 ) || !yocto[ K_IO_1 ][ K_OUT_PIN ][ K_LIGHT_CH3 ].is_number_integer ( ) )
				throw std::runtime_error ( std::format ( 
					"\n\t[{}({}): {}]\n\t\tInvalid or missing '{}.{}.{}.{}'. Expected integer." ,
					fs::path ( __FILE__ ).filename ( ).string ( ) , __LINE__ , __FUNCSIG__ , K_YOCTO , K_IO_1 , K_OUT_PIN , K_LIGHT_CH3 ) );
			
			// yocto.io_1.output_pin.light_ch4 : integer
			if ( !yocto[ K_IO_1 ][ K_OUT_PIN ].contains ( K_LIGHT_CH4 ) || !yocto[ K_IO_1 ][ K_OUT_PIN ][ K_LIGHT_CH4 ].is_number_integer ( ) )
				throw std::runtime_error ( std::format ( 
					"\n\t[{}({}): {}]\n\t\tInvalid or missing '{}.{}.{}.{}'. Expected integer." ,
					fs::path ( __FILE__ ).filename ( ).string ( ) , __LINE__ , __FUNCSIG__ , K_YOCTO , K_IO_1 , K_OUT_PIN , K_LIGHT_CH4 ) );

			// yocto.io_1.output_pin.heater_switch : integer
			if ( !yocto[ K_IO_1 ][ K_OUT_PIN ].contains ( K_HEATER_SWITCH ) || !yocto[ K_IO_1 ][ K_OUT_PIN ][ K_HEATER_SWITCH ].is_number_integer ( ) )
				throw std::runtime_error ( std::format ( 
					"\n\t[{}({}): {}]\n\t\tInvalid or missing '{}.{}.{}.{}'. Expected integer." ,
					fs::path ( __FILE__ ).filename ( ).string ( ) , __LINE__ , __FUNCSIG__ , K_YOCTO , K_IO_1 , K_OUT_PIN , K_HEATER_SWITCH ) );

			// yocto.io_1.input_pin.light_manual : integer
			if ( !yocto[ K_IO_1 ][ K_IN_PIN ].contains ( K_LIGHT_MANUAL ) || !yocto[ K_IO_1 ][ K_IN_PIN ][ K_LIGHT_MANUAL ].is_number_integer ( ) )
				throw std::runtime_error ( std::format ( 
					"\n\t[{}({}): {}]\n\t\tInvalid or missing '{}.{}.{}.{}'. Expected integer." ,
					fs::path ( __FILE__ ).filename ( ).string ( ) , __LINE__ , __FUNCSIG__ , K_YOCTO , K_IO_1 , K_IN_PIN , K_LIGHT_MANUAL ) );

			// yocto.io_1.input_pin.heater_alarm : integer
			if ( !yocto[ K_IO_1 ][ K_IN_PIN ].contains ( K_HEATER_ALARM ) || !yocto[ K_IO_1 ][ K_IN_PIN ][ K_HEATER_ALARM ].is_number_integer ( ) )
				throw std::runtime_error ( std::format ( 
					"\n\t[{}({}): {}]\n\t\tInvalid or missing '{}.{}.{}.{}'. Expected integer." ,
					fs::path ( __FILE__ ).filename ( ).string ( ) , __LINE__ , __FUNCSIG__ , K_YOCTO , K_IO_1 , K_IN_PIN , K_HEATER_ALARM ) );

			// yocto.io_1.input_pin.light_alarm : integer
			if ( !yocto[ K_IO_1 ][ K_IN_PIN ].contains ( K_LIGHT_ALARM ) || !yocto[ K_IO_1 ][ K_IN_PIN ][ K_LIGHT_ALARM ].is_number_integer ( ) )
				throw std::runtime_error ( std::format ( 
					"\n\t[{}({}): {}]\n\t\tInvalid or missing '{}.{}.{}.{}'. Expected integer." ,
					fs::path ( __FILE__ ).filename ( ).string ( ) , __LINE__ , __FUNCSIG__ , K_YOCTO , K_IO_1 , K_IN_PIN , K_LIGHT_ALARM ) );

			// yocto.io_2.output_pin.mcs_error : integer
			if ( !yocto[ K_IO_2 ][ K_OUT_PIN ].contains ( K_MCS_ERROR ) || !yocto[ K_IO_2 ][ K_OUT_PIN ][ K_MCS_ERROR ].is_number_integer ( ) )
				throw std::runtime_error ( std::format ( 
					"\n\t[{}({}): {}]\n\t\tInvalid or missing '{}.{}.{}.{}'. Expected integer." ,
					fs::path ( __FILE__ ).filename ( ).string ( ) , __LINE__ , __FUNCSIG__ , K_YOCTO , K_IO_2 , K_OUT_PIN , K_MCS_ERROR ) );

			// yocto.io_2.input_pin.mcs_done : integer
			if ( !yocto[ K_IO_2 ][ K_IN_PIN ].contains ( K_MCS_DONE ) || !yocto[ K_IO_2 ][ K_IN_PIN ][ K_MCS_DONE ].is_number_integer ( ) )
				throw std::runtime_error ( std::format ( 
					"\n\t[{}({}): {}]\n\t\tInvalid or missing '{}.{}.{}.{}'. Expected integer." ,
					fs::path ( __FILE__ ).filename ( ).string ( ) , __LINE__ , __FUNCSIG__ , K_YOCTO , K_IO_2 , K_IN_PIN , K_MCS_DONE ) );
#endif
			// yocto.loop
			if ( !yocto.contains ( K_LOOP ) || !yocto[ K_LOOP ].is_object ( ) )
				throw std::runtime_error ( std::format ( 
					"\n\t[{}({}): {}]\n\t\tMissing '{}.{}' settings." , 
					fs::path ( __FILE__ ).filename ( ).string ( ) , __LINE__ , __FUNCSIG__ , K_YOCTO , K_LOOP ) );

			// yocto.loop.enable : boolean
			if ( !yocto[ K_LOOP ].contains ( K_ENABLE ) || !yocto[ K_LOOP ][ K_ENABLE ].is_boolean ( ) )
				throw std::runtime_error ( std::format ( 
					"\n\t[{}({}): {}]\n\t\tInvalid type for '{}.{}.{}'. Expected boolean." ,
					fs::path ( __FILE__ ).filename ( ).string ( ) , __LINE__ , __FUNCSIG__ , K_YOCTO , K_LOOP , K_ENABLE ) );

			// yocto.loop.S/N
			if ( !yocto[ K_LOOP ].contains ( K_SN ) || !yocto[ K_LOOP ][ K_SN ].is_string ( ) )
				throw std::runtime_error ( std::format ( 
					"\n\t[{}({}): {}]\n\t\tInvalid type for '{}.{}.{}'. Expected string." ,
					fs::path ( __FILE__ ).filename ( ).string ( ) , __LINE__ , __FUNCSIG__ , K_YOCTO , K_LOOP , K_SN ) );

			// yocto.sensor
			if ( !yocto.contains ( K_SENSOR ) || !yocto[ K_SENSOR ].is_object ( ) )
				throw std::runtime_error ( std::format ( 
					"\n\t[{}({}): {}]\n\t\tMissing '{}.{}' settings." , 
					fs::path ( __FILE__ ).filename ( ).string ( ) , __LINE__ , __FUNCSIG__ , K_YOCTO , K_SENSOR ) );

			// yocto.sensor.enable : boolean
			if ( !yocto[ K_SENSOR ].contains ( K_ENABLE ) || !yocto[ K_SENSOR ][ K_ENABLE ].is_boolean ( ) )
				throw std::runtime_error ( std::format ( 
					"\n\t[{}({}): {}]\n\t\tInvalid type for '{}.{}.{}'. Expected boolean." ,
					fs::path ( __FILE__ ).filename ( ).string ( ) , __LINE__ , __FUNCSIG__ , K_YOCTO , K_SENSOR , K_ENABLE ) );

			// yocto.sensor.S/N
			if ( !yocto[ K_SENSOR ].contains ( K_SN ) || !yocto[ K_SENSOR ][ K_SN ].is_string ( ) )
				throw std::runtime_error ( std::format ( 
					"\n\t[{}({}): {}]\n\t\tInvalid type for '{}.{}.{}'. Expected string." ,
					fs::path ( __FILE__ ).filename ( ).string ( ) , __LINE__ , __FUNCSIG__ , K_YOCTO , K_SENSOR , K_SN ) );


			// ===== etc =====
			if ( !jsonSrc.contains ( K_ETC ) || !jsonSrc[ K_ETC ].is_object ( ) )
				throw std::runtime_error ( std::format ( 
					"\n\t[{}({}): {}]\n\t\tMissing '{}'' settings." , 
					fs::path ( __FILE__ ).filename ( ).string ( ) , __LINE__ , __FUNCSIG__ , K_ETC ) );
			const auto& etc = jsonSrc[ K_ETC ];

			// etc.debug : boolean
			if ( !etc.contains ( K_DEBUG ) || !etc[ K_DEBUG ].is_boolean ( ) )
				throw std::runtime_error ( std::format ( 
					"\n\t[{}({}): {}]\n\t\tInvalid type for 'jsonSetting.{}.{}'. Expected boolean." ,
					fs::path ( __FILE__ ).filename ( ).string ( ) , __LINE__ , __FUNCSIG__ , K_ETC , K_DEBUG ) );

			// etc.auto_sort_light_ch : boolean
			if ( !etc.contains ( K_AUTO_SORT_LIGHT_CH ) || !etc[ K_AUTO_SORT_LIGHT_CH ].is_boolean ( ) )
				throw std::runtime_error ( std::format ( 
					"\n\t[{}({}): {}]\n\t\tInvalid type for 'jsonSetting.{}.{}'. Expected boolean." ,
					fs::path ( __FILE__ ).filename ( ).string ( ) , __LINE__ , __FUNCSIG__ , K_ETC , K_AUTO_SORT_LIGHT_CH ) );

			// etc.auto_calibration_wb : boolean
			if ( !etc.contains ( K_AUTO_CALIBRATION_WB ) || !etc[ K_AUTO_CALIBRATION_WB ].is_boolean ( ) )
				throw std::runtime_error ( std::format ( 
					"\n\t[{}({}): {}]\n\t\tInvalid type for 'jsonSetting.{}.{}'. Expected boolean." ,
					fs::path ( __FILE__ ).filename ( ).string ( ) , __LINE__ , __FUNCSIG__ , K_ETC , K_AUTO_CALIBRATION_WB ) );

			// etc.output_dir : string
			if ( !etc.contains ( K_OUTPUT_DIR ) || !etc[ K_OUTPUT_DIR ].is_string ( ) )
				throw std::runtime_error ( std::format ( 
					"\n\t[{}({}): {}]\n\t\tInvalid type for 'jsonSetting.{}.{}'. Expected string." ,
					fs::path ( __FILE__ ).filename ( ).string ( ) , __LINE__ , __FUNCSIG__ , K_ETC , K_OUTPUT_DIR ) );

			// etc.vessel_max_L : number
			if ( !etc.contains ( K_VESSEL_MAX_L ) || !etc[ K_VESSEL_MAX_L ].is_number ( ) )
				throw std::runtime_error ( std::format ( 
					"\n\t[{}({}): {}]\n\t\tInvalid type for 'jsonSetting.{}.{}'. Expected number." ,
					fs::path ( __FILE__ ).filename ( ).string ( ) , __LINE__ , __FUNCSIG__ , K_ETC , K_VESSEL_MAX_L ) );
			if ( etc[ K_VESSEL_MAX_L ].get<double> ( ) <= 0 )
				throw std::runtime_error ( std::format ( 
					"\n\t[{}({}): {}]\n\t\tInvalid value for 'jsonSetting.{}.{}'. Expected positive number." ,
					fs::path ( __FILE__ ).filename ( ).string ( ) , __LINE__ , __FUNCSIG__ , K_ETC , K_VESSEL_MAX_L ) );

			// etc.len_graph : number
			if ( !etc.contains ( K_LEN_GRAPH ) || !etc[ K_LEN_GRAPH ].is_number ( ) )
				throw std::runtime_error ( std::format ( 
					"\n\t[{}({}): {}]\n\t\tInvalid type for 'jsonSetting.{}.{}'. Expected number." ,
					fs::path ( __FILE__ ).filename ( ).string ( ) , __LINE__ , __FUNCSIG__ , K_ETC , K_LEN_GRAPH ) );
			if ( etc[ K_LEN_GRAPH ].get<int> ( ) <= 0 )
				throw std::runtime_error ( std::format ( 
					"\n\t[{}({}): {}]\n\t\tInvalid value for 'jsonSetting.{}.{}'. Expected positive number." ,
					fs::path ( __FILE__ ).filename ( ).string ( ) , __LINE__ , __FUNCSIG__ , K_ETC , K_LEN_GRAPH ) );

			// etc.result_font_scale : number
			if ( !etc.contains ( K_RESULT_FONT_SCALE ) || !etc[ K_RESULT_FONT_SCALE ].is_number ( ) )
				throw std::runtime_error ( std::format ( 
					"\n\t[{}({}): {}]\n\t\tInvalid type for 'jsonSetting.{}.{}'. Expected number." ,
					fs::path ( __FILE__ ).filename ( ).string ( ) , __LINE__ , __FUNCSIG__ , K_ETC , K_RESULT_FONT_SCALE ) );
			if ( etc[ K_RESULT_FONT_SCALE ].get<double> ( ) <= 0 )
				throw std::runtime_error ( std::format ( 
					"\n\t[{}({}): {}]\n\t\tInvalid value for 'jsonSetting.{}.{}'. Expected positive number." ,
					fs::path ( __FILE__ ).filename ( ).string ( ) , __LINE__ , __FUNCSIG__ , K_ETC , K_RESULT_FONT_SCALE ) );

			// etc.send_period : number
			if ( !etc.contains ( K_SEND_PERIOD ) || !etc[ K_SEND_PERIOD ].is_number ( ) )
				throw std::runtime_error ( std::format ( 
					"\n\t[{}({}): {}]\n\t\tInvalid type for 'jsonSetting.{}.{}'. Expected number." ,
					fs::path ( __FILE__ ).filename ( ).string ( ) , __LINE__ , __FUNCSIG__ , K_ETC , K_SEND_PERIOD ) );
			if ( etc[ K_SEND_PERIOD ].get<int> ( ) <= 0 )
				throw std::runtime_error ( std::format ( 
					"\n\t[{}({}): {}]\n\t\tInvalid value for 'jsonSetting.{}.{}'. Expected positive number." ,
					fs::path ( __FILE__ ).filename ( ).string ( ) , __LINE__ , __FUNCSIG__ , K_ETC , K_SEND_PERIOD ) );

			// etc.show_period : number
			if ( !etc.contains ( K_SHOW_PERIOD ) || !etc[ K_SHOW_PERIOD ].is_number ( ) )
				throw std::runtime_error ( std::format ( 
					"\n\t[{}({}): {}]\n\t\tInvalid type for 'jsonSetting.{}.{}'. Expected number." ,
					fs::path ( __FILE__ ).filename ( ).string ( ) , __LINE__ , __FUNCSIG__ , K_ETC , K_SHOW_PERIOD ) );
			if ( etc[ K_SHOW_PERIOD ].get<int> ( ) <= 0 )
				throw std::runtime_error ( std::format ( 
					"\n\t[{}({}): {}]\n\t\tInvalid value for 'jsonSetting.{}.{}'. Expected positive number." ,
					fs::path ( __FILE__ ).filename ( ).string ( ) , __LINE__ , __FUNCSIG__ , K_ETC , K_SHOW_PERIOD ) );

			// etc.camera_temp_limit : number
			if ( !etc.contains ( K_CAM_TEMP_LIMIT ) || !etc[ K_CAM_TEMP_LIMIT ].is_number ( ) )
				throw std::runtime_error ( std::format ( 
					"\n\t[{}({}): {}]\n\t\tInvalid type for 'jsonSetting.{}.{}'. Expected number." ,
					fs::path ( __FILE__ ).filename ( ).string ( ) , __LINE__ , __FUNCSIG__ , K_ETC , K_CAM_TEMP_LIMIT ) );

		}
		catch ( const std::exception& e )
		{
			strErr = e.what ( );
			return false;
		}

		return true;
	}

	// JSON 모델 검증
	inline bool validateModel ( const json& jsonSrc , std::string& strErr )
	{
		auto check_number = [ & ] ( const char* key ) -> bool {
			if ( !jsonSrc.contains ( key ) || !jsonSrc[ key ].is_number ( ) )
			{
				strErr = std::format (
					"\n\t[{}({}): {}]\n\t\tInvalid or missing '{}'. Expected number." ,
					fs::path(__FILE__).filename().string() , __LINE__ , __FUNCSIG__ , key
				);
				return false;
			}
			return true;
			};

		// 숫자 값들 검사
		if ( !check_number ( K_BLUR_SIZE ) )      return false;
		if ( !check_number ( K_KERNEL_SIZE ) )    return false;
		if ( !check_number ( K_ERODE_ITER ) )     return false;
		if ( !check_number ( K_DILATE_ITER ) )    return false;
		if ( !check_number ( K_HSV_S_THRESH ) )   return false;
		if ( !check_number ( K_HSV_V_THRESH ) )   return false;

		// inner_circle / outer_circle
		if ( !check_number ( K_INNER_CIRCLE_R ) ) return false;
		if ( !check_number ( K_INNER_CIRCLE_X ) ) return false;
		if ( !check_number ( K_INNER_CIRCLE_Y ) ) return false;
		if ( !check_number ( K_OUTER_CIRCLE_R ) ) return false;
		if ( !check_number ( K_OUTER_CIRCLE_X ) ) return false;
		if ( !check_number ( K_OUTER_CIRCLE_Y ) ) return false;

		if ( !check_number ( K_FOAM_RATIO_THRESH ) ) return false;

		return true;
	}
}
