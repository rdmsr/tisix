#pragma once
#include <syscalls.hpp>

namespace tisix
{
void ipc_on_receive(bool (*)(TxIpc ipc));

uint32_t in(uint16_t port, int size);
void out(uint16_t port, uint32_t data, int size);

} // namespace tisix