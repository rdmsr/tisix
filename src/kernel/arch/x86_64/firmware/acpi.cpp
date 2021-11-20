#include "tisix/string_view.hpp"
#include <firmware/acpi.hpp>
#include <pmm.hpp>
#include <tisix/log.hpp>

using namespace tisix;

Acpi::Acpi(Handover *handover)
{
    auto rsdp = (AcpiRsdp *)handover->rsdp;

    AcpiRsdt *rsdt = (AcpiRsdt *)((uintptr_t)rsdp->rsdt + MMAP_IO_BASE);

    size_t entries = (rsdt->descriptor.length - sizeof(rsdt->descriptor)) / 4;

    log("ACPI dump:");
    for (size_t i = 0; i < entries; i++)
    {
        AcpiSdt *h = (AcpiSdt *)(rsdt->sptr[i] + MMAP_IO_BASE);

        char nulled_string[5] = {0};
        memcpy(nulled_string, h->signature, 4);

        log("\t{}", nulled_string);

        if (StringView(h->signature) == StringView("APIC"))
        {
            this->madt = (AcpiMadt *)h;

            this->lapic_base = this->madt->lapic;

            log("\t   - address: {#p}", this->lapic_base);
        }
    }
}