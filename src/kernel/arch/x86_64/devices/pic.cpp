#include "pic.hpp"
#include "asm.hpp"

static void pic_remap(void)
{
    // FIXME: stop using magic values
    asm_out8(PIC1, ICW1_INIT + ICW1_ICW4);
    asm_out8(PIC2, ICW1_INIT + ICW1_ICW4);
    asm_out8(PIC1_DATA, PIC1_OFFSET);
    asm_out8(PIC2_DATA, PIC2_OFFSET);
    asm_out8(PIC1_DATA, 0x04);
    asm_out8(PIC2_DATA, 0x02);
    asm_out8(PIC1_DATA, ICW1_ICW4);
    asm_out8(PIC2_DATA, ICW1_ICW4);
    asm_out8(PIC1_DATA, 0x0);
    asm_out8(PIC2_DATA, 0x0);
}

void pic_initialize(void)
{
    pic_remap();
}

void pic_eoi(int intno)
{
    if (intno >= 40)
    {
        asm_out8(PIC2, 0x20);
    }

    asm_out8(PIC1, 0x20);
}