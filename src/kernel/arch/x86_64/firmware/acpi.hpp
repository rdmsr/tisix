#pragma once
#include "tisix/handover.hpp"
#include <tisix/std.hpp>

struct AcpiSdt
{
    char signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    char oem_id[6];
    char oem_table_id[8];
    uint32_t oem_revision;
    uint32_t creator_id;
    uint32_t creator_revision;
};

struct AcpiRsdt
{
    AcpiSdt descriptor;
    uint32_t sptr[];
};

struct AcpiRsdp
{
    char sign[8];
    uint8_t checksum;
    char oemid[6];
    uint8_t rev;
    uint32_t rsdt;
};

struct AcpiMadtRecord
{
    uint8_t type;
    uint8_t length;
};

struct AcpiMadt
{

    AcpiSdt base;
    uint32_t lapic;
    uint32_t flags;
    AcpiMadtRecord records[];
};

class Acpi
{
public:
    Acpi(tisix::Handover *handover);

public:
    AcpiMadt *madt;
    uintptr_t lapic_base = 0;
};