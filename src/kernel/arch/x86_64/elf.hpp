#pragma once
#include <tisix/std.hpp>

#define ELF_HEADER_MAGIC "\177ELF"

namespace tisix
{
enum ElfClass
{
    ELF_CLASS_INVALID = 0,
    ELF_CLASS_32 = 1,
    ELF_CLASS_64 = 2,
};

enum ElfEncoding
{
    ELF_ENCODING_INVALID = 0,
    ELF_ENCODING_LITTLE_ENDIAN = 1,
    ELF_ENCODING_BIG_ENDIAN = 2,
};

enum ElfType
{
    ELF_TYPE_NONE = 0,
    ELF_TYPE_RELOCATABLE = 1,
    ELF_TYPE_EXECUTABLE = 2,
    ELF_TYPE_DYNAMIC = 3,
    ELF_TYPE_CORE = 4,
};

enum ElfProgramHeaderType
{
    ELF_PROGRAM_HEADER_NULL = 0,
    ELF_PROGRAM_HEADER_LOAD = 1,
    ELF_PROGRAM_HEADER_DYNAMIC = 2,
    ELF_PROGRAM_HEADER_INTERPRET = 3,
    ELF_PROGRAM_HEADER_NOTE = 4,
};

enum ElfProgramHeaderFlags
{
    ELF_PROGRAM_HEADER_EXECUTABLE = 1 << 0,
    ELF_PROGRAM_HEADER_WRITABLE = 1 << 1,
    ELF_PROGRAM_HEADER_READABLE = 1 << 2,
};

struct PACKED Elf64Ident
{
    uint8_t magics[4];
    uint8_t elf_class;
    uint8_t data_encoding;
    uint8_t version;
    uint8_t os;
    uint8_t abi_version;
    uint8_t _padding[7];
};

static_assert(sizeof(Elf64Ident) == 16, "");

struct PACKED Elf64Header
{
    Elf64Ident ident;

    uint16_t object_type;
    uint16_t machine_type;
    uint32_t object_version;

    uint64_t entry;

    uint64_t program_header_table_file_offset;
    uint64_t section_header_table_file_offset;

    uint32_t flags;

    uint16_t elf_header_size;

    uint16_t program_header_table_entry_size;
    uint16_t program_header_table_entry_count;

    uint16_t section_header_table_entry_size;
    uint16_t section_header_table_entry_count;

    uint16_t section_header_string_table_idx;
};

struct PACKED Elf64ProgramHeader
{
    uint32_t type;
    uint32_t flags;

    uint64_t file_offset;
    uint64_t virtual_address;
    uint64_t physical_address;

    uint64_t file_size;
    uint64_t memory_size;

    uint64_t alignment;
};

inline bool elf_validate(Elf64Header const *header)
{
    return header->ident.magics[0] == '\177' &&
           header->ident.magics[1] == 'E' &&
           header->ident.magics[2] == 'L' &&
           header->ident.magics[3] == 'F';
}

} // namespace tisix