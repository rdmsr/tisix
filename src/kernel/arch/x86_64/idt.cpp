#include "idt.hpp"
#include "asm.hpp"

extern "C" void idt_flush(uintptr_t idt_ptr);

static IdtDescriptor idt[256];
static IdtPointer idtr;
extern uintptr_t __interrupt_vector[];

static IdtDescriptor idt_make_entry(uint64_t offset, uint8_t type)
{
    return (IdtDescriptor){
        .offset_lo = static_cast<uint16_t>(offset & 0xFFFF),
        .selector = 0x08,
        .ist = 0,
        .type_attr = type,
        .offset_mid = static_cast<uint16_t>((offset >> 16) & 0xFFFF),
        .offset_hi = static_cast<uint32_t>((offset >> 32) & 0xFFFFFFFF),
        .zero = 0};
}

void install_isr(void)
{
    for (auto i = 0; i < 3; i++)
    {
        idt[i] = idt_make_entry(__interrupt_vector[i], INTGATE);
    }

    idt[3] = idt_make_entry(__interrupt_vector[3], TRAPGATE);
    idt[4] = idt_make_entry(__interrupt_vector[4], TRAPGATE);

    for (auto i = 5; i < 48; i++)
    {
        idt[i] = idt_make_entry(__interrupt_vector[i], INTGATE);
    }

    idt[66] = idt_make_entry(__interrupt_vector[48], 0xee);
}

void idt_initialize()
{
    idtr.size = (256 * sizeof(IdtDescriptor)) - 1;

    idtr.addr = (uint64_t)idt;

    install_isr();

    idt_flush((uintptr_t)&idtr);

    asm_sti();

    log("initialized idt");
}