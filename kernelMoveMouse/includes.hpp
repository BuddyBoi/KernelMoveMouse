#pragma once
#include <ntifs.h>
#include <windef.h>

//global offsets
namespace globals
{
    //windows version
    ULONG u_ver_build = 0;
    ULONG u_ver_major = 0;

    namespace offsets
    {
        //EPROCESS->ImageFileName
        int i_image_file_name = 0x0;

        //EPROCESS->ActiveThreads
        int i_active_threads = 0x0;

        //EPROCESS->ActiveProcessLinks
        int i_active_process_links = 0x0;
    }
}

//undefined kernel structs
typedef struct _LDR_DATA_TABLE_ENTRY
{
    LIST_ENTRY InLoadOrderLinks;
    LIST_ENTRY InMemoryOrderLinks;
    LIST_ENTRY InInitializationOrderLinks;
    PVOID DllBase;
    PVOID EntryPoint;
    ULONG SizeOfImage;
    UNICODE_STRING FullDllName;
    UNICODE_STRING BaseDllName;
    ULONG Flags;
    WORD LoadCount;
    WORD TlsIndex;
    union
    {
        LIST_ENTRY HashLinks;
        struct
        {
            PVOID SectionPointer;
            ULONG CheckSum;
        };
    };
    union
    {
        ULONG TimeDateStamp;
        PVOID LoadedImports;
    };
    VOID* EntryPointActivationContext;
    PVOID PatchInformation;
    LIST_ENTRY ForwarderLinks;
    LIST_ENTRY ServiceTagLinks;
    LIST_ENTRY StaticLinks;
} LDR_DATA_TABLE_ENTRY, * PLDR_DATA_TABLE_ENTRY;


//undefined kernel functions
extern "C"
{
    __declspec(dllimport) PLIST_ENTRY NTAPI PsLoadedModuleList;
    __declspec(dllimport) POBJECT_TYPE* IoDriverObjectType;
    __declspec(dllimport) PVOID NTAPI RtlFindExportedRoutineByName( PVOID, PCCH );
    __declspec(dllimport) PVOID NTAPI PsGetProcessSectionBaseAddress( PEPROCESS );
    __declspec(dllimport) PPEB NTAPI PsGetProcessPeb( PEPROCESS );
    __declspec(dllimport) NTSTATUS NTAPI ZwProtectVirtualMemory( HANDLE, PVOID*, PSIZE_T, ULONG, PULONG );
    __declspec(dllimport) PIMAGE_NT_HEADERS NTAPI RtlImageNtHeader( PVOID );
    __declspec(dllimport) NTSTATUS NTAPI ObReferenceObjectByName( PUNICODE_STRING, ULONG, PACCESS_STATE, ACCESS_MASK, POBJECT_TYPE, KPROCESSOR_MODE, PVOID OPTIONAL, PVOID* );
}
