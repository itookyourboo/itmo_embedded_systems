#ifndef __studio_h_
#include <stdio.h>
#endif

#ifndef __stdlib_h_
#include <stdlib.h>
#endif

#ifndef __stdint_h_
#include <stdint.h>
#endif

#if !defined(WORD) || !defined(DWORD) || !defined(BYTE)
typedef uint32_t DWORD;
typedef uint16_t WORD;
typedef uint8_t BYTE;
#endif

#ifndef IMAGE_SIZEOF_SECTION_HEADER
#define IMAGE_SIZEOF_SECTION_HEADER 40
#endif

#ifndef _IMAGE_FILE_HEADER
typedef struct _IMAGE_FILE_HEADER {
    WORD  Machine;
    WORD  NumberOfSections;
    DWORD TimeDateStamp;
    DWORD PointerToSymbolTable;
    DWORD NumberOfSymbols;
    WORD  SizeOfOptionalHeader;
    WORD  Characteristics;
} IMAGE_FILE_HEADER, *PIMAGE_FILE_HEADER;

#endif
#ifndef _IMAGE_DATA_DIRECTORY
typedef struct _IMAGE_DATA_DIRECTORY {
    DWORD VirtualAddress;
    DWORD Size;
} IMAGE_DATA_DIRECTORY, *PIMAGE_DATA_DIRECTORY;
#endif
#ifndef _IMAGE_OPTIONAL_HEADER
typedef struct _IMAGE_OPTIONAL_HEADER {
    WORD                 Magic;
    BYTE                 MajorLinkerVersion;
    BYTE                 MinorLinkerVersion;
    DWORD                SizeOfCode;
    DWORD                SizeOfInitializedData;
    DWORD                SizeOfUninitializedData;
    DWORD                AddressOfEntryPoint;
    DWORD                BaseOfCode;
    DWORD                BaseOfData;
    DWORD                ImageBase;
    DWORD                SectionAlignment;
    DWORD                FileAlignment;
    WORD                 MajorOperatingSystemVersion;
    WORD                 MinorOperatingSystemVersion;
    WORD                 MajorImageVersion;
    WORD                 MinorImageVersion;
    WORD                 MajorSubsystemVersion;
    WORD                 MinorSubsystemVersion;
    DWORD                Win32VersionValue;
    DWORD                SizeOfImage;
    DWORD                SizeOfHeaders;
    DWORD                CheckSum;
    WORD                 Subsystem;
    WORD                 DllCharacteristics;
    DWORD                SizeOfStackReserve;
    DWORD                SizeOfStackCommit;
    DWORD                SizeOfHeapReserve;
    DWORD                SizeOfHeapCommit;
    DWORD                LoaderFlags;
    DWORD                NumberOfRvaAndSizes;
    IMAGE_DATA_DIRECTORY DataDirectory[16];
} IMAGE_OPTIONAL_HEADER, *PIMAGE_OPTIONAL_HEADER;
#endif

#ifndef _IMAGE_NT_HEADERS
typedef struct _IMAGE_NT_HEADERS {
    DWORD                 Signature;
    IMAGE_FILE_HEADER     FileHeader;
    IMAGE_OPTIONAL_HEADER OptionalHeader;
} IMAGE_NT_HEADERS, *PIMAGE_NT_HEADERS;

#endif

#ifndef _IMAGE_SECTION_HEADER
typedef struct _IMAGE_SECTION_HEADER {
    BYTE  Name[8];
    union {
        DWORD PhysicalAddress;
        DWORD VirtualSize;
    } Misc;
    DWORD VirtualAddress;
    DWORD SizeOfRawData;
    DWORD PointerToRawData;
    DWORD PointerToRelocations;
    DWORD PointerToLinenumbers;
    WORD  NumberOfRelocations;
    WORD  NumberOfLinenumbers;
    DWORD Characteristics;
} IMAGE_SECTION_HEADER, *PIMAGE_SECTION_HEADER;

#endif

#ifndef _IMAGE_DOS_HEADER
typedef struct _IMAGE_DOS_HEADER {
    WORD e_magic;       // Сигнатура заголовка
    WORD e_cblp;        // количество байт на последней странице файла
    WORD e_cp;          // количество страниц в файле
    WORD e_crlc;        // Relocations
    WORD e_cparhdr;     // Размер заголовка в параграфах
    WORD e_minalloc;    // Минимальные дополнительные параграфы
    WORD e_maxalloc;    // Максимальные дополнительные параграфы
    WORD e_ss;          // начальное  относительное значение регистра SS
    WORD e_sp;          // начальное значение регистра SP
    WORD e_csum;        // контрольная сумма
    WORD e_ip;          // начальное значение регистра IP
    WORD e_cs;          // начальное относительное значение регистра CS
    WORD e_lfarlc;		// адрес в файле на таблицу переадресации
    WORD e_ovno;		// количество оверлеев
    WORD e_res[4];		// Зарезервировано
    WORD e_oemid;		// OEM идентифкатор
    WORD e_oeminfo;		// OEM информация
    WORD e_res2[10];	// Зарезервировано
    DWORD e_lfanew;     // адрес в файле нового .exe заголовка (PE)
} IMAGE_DOS_HEADER, *PIMAGE_DOS_HEADER;

#endif