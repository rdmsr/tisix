#pragma once

#include <abi/syscalls.hpp>

namespace tisix
{
void ipc_send(TxIpc *data);
void ipc_recv(TxIpc *data);
} // namespace tisix