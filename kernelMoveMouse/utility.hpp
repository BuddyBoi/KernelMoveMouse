#pragma once
#include "includes.hpp"
#include "memory.hpp"

namespace utility
{
	void move_mouse( ULONG point1, ULONG point2 )
	{
		PEPROCESS winlogon_peprocess = memory::get_process( (char*)"winlogon.exe" );
		if ( winlogon_peprocess )
		{
			KAPC_STATE apc = { 0 };
			KeStackAttachProcess( winlogon_peprocess, &apc );

			uint64_t gptCursorAsync = reinterpret_cast<uint64_t>(memory::system_export( L"win32kbase.sys", "gptCursorAsync" ));

			POINT cursor = *(POINT*)(gptCursorAsync);

			cursor.x = point1;
			cursor.y = point2;

			*(POINT*)(gptCursorAsync) = cursor;

			KeUnstackDetachProcess( &apc );
		}
	}
}