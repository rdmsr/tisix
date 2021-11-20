bits 64

global gdt_update
gdt_update:
  lgdt [rdi]
  mov ax, 0x10
  mov ss, ax
  mov ds, ax
  mov es, ax
  pop rdi
  mov rax, 0x8
  push rax
  push rdi
  retfq

global tss_update
tss_update:
  mov ax, 0x28
  ltr ax
  ret