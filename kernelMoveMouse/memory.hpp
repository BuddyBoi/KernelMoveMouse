#pragma once
#include "includes.hpp"
#include <cstdint>

namespace memory
{
	//get process by name
	PEPROCESS get_process( char* process_name )
	{
		uintptr_t list_head = *(uintptr_t*)((uintptr_t)PsInitialSystemProcess + globals::offsets::i_active_process_links);
		uintptr_t list_current = list_head;

		do
		{
			uintptr_t list_entry = list_current - globals::offsets::i_active_process_links;

			if ( !_stricmp( process_name, (char*)(list_entry + globals::offsets::i_image_file_name) ) )
			{
				return (PEPROCESS)list_entry;
			}

			list_current = *(uintptr_t*)list_current;
		} while ( list_current != list_head );

		return NULL;
	}

	//get kernel module
	PLDR_DATA_TABLE_ENTRY system_module( const wchar_t* module_name )
	{
		static const auto ntoskrnl_base = *reinterpret_cast<const char**>(std::uintptr_t( PsLoadedModuleList ) + 0x30);

		UNICODE_STRING unicode_string{ };
		RtlInitUnicodeString( &unicode_string, module_name );

		PLDR_DATA_TABLE_ENTRY system_module_entry = nullptr;

		for ( auto entry = PsLoadedModuleList; entry != PsLoadedModuleList->Blink; entry = entry->Flink )
		{
			PLDR_DATA_TABLE_ENTRY data_table = CONTAINING_RECORD( entry, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks );

			if ( RtlEqualUnicodeString( &data_table->BaseDllName, &unicode_string, TRUE ) )
			{
				system_module_entry = data_table;
				break;
			}
		}

		return system_module_entry;
	}

	//get WinAPI function
	void* system_export( const wchar_t* module_name, const char* export_name )
	{
		static const auto ntoskrnl_base = *reinterpret_cast<const char**>(std::uintptr_t( PsLoadedModuleList ) + 0x30);

		const auto module = system_module( module_name );

		if ( !module )
			return nullptr;

		return RtlFindExportedRoutineByName( module->DllBase, export_name );
	}
}
