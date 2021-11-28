#pragma once
#include <syscalls.hpp>

namespace tisix
{
void ipc_on_receive(bool (*)(TxIpc ipc));
}