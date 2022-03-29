#include "includes.hpp"
#include "utility.hpp"

namespace driver
{
	bool get_offsets()
	{
		//offsets from 19041 - 22000 (Windows 11 21H2 - Windows 10 20H1)
		if ( globals::u_ver_build == 22000 || globals::u_ver_build == 19041 || globals::u_ver_build == 19042 || globals::u_ver_build == 19043 || globals::u_ver_build == 19044 )
		{
			globals::offsets::i_image_file_name = 0x5a8;
			globals::offsets::i_active_process_links = 0x448;
			globals::offsets::i_active_threads = 0x5f0;

			return true;
		}

		//1903-1909
		else if ( globals::u_ver_build == 18362 || globals::u_ver_build == 18363 )
		{
			globals::offsets::i_image_file_name = 0x450;
			globals::offsets::i_active_process_links = 0x2f0;
			globals::offsets::i_active_threads = 0x498;

			return true;
		}

		//if no offsets for their version is found
		return false;
	}

	bool b_version_check()
	{
		//get windows version info
		static OSVERSIONINFOW v_info{};
		RtlGetVersion( &v_info );

		//Close driver is user is not windows 10 or higher
		if ( v_info.dwMajorVersion < 10 )
		{
			DbgPrint( "Version check failed" );
			return false;
		}

		//store windows versions for later info
		globals::u_ver_major = v_info.dwMajorVersion;
		globals::u_ver_build = v_info.dwBuildNumber;

		//get windows offsets for this windows build
		if ( !get_offsets() )
		{
			DbgPrint( "Failed to get current offsets" );
			return false;
		}

		return true;
	}

	extern "C" NTSTATUS driver_entry( PDRIVER_OBJECT  driver_object, PUNICODE_STRING registry_path )
	{
		UNREFERENCED_PARAMETER( registry_path );
		UNREFERENCED_PARAMETER( driver_object );

		//check version and get version offsets
		if ( !b_version_check() )
			return STATUS_UNSUCCESSFUL;
			

		ULONG mouse_x = 150;
		ULONG mouse_y = 150;

		//move mouse from WinApi function reference
		//sadly, only works while mouse position is already being updated, i.e. while the mouse is already moving
		utility::move_mouse( mouse_x, mouse_y );

		return STATUS_SUCCESS;
	}
}