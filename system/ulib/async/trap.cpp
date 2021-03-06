// Copyright 2017 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <lib/async/cpp/trap.h>

namespace async {

GuestBellTrapBase::GuestBellTrapBase(async_guest_bell_trap_handler_t* handler)
    : trap_{{ASYNC_STATE_INIT}, handler} {}

GuestBellTrapBase::~GuestBellTrapBase() = default;

zx_status_t GuestBellTrapBase::SetTrap(
    async_t* async, const zx::guest& guest, zx_vaddr_t addr, size_t length) {
    return async_set_guest_bell_trap(async, &trap_, guest.get(), addr, length);
}

GuestBellTrap::GuestBellTrap(Handler handler)
    : GuestBellTrapBase(&GuestBellTrap::CallHandler), handler_(fbl::move(handler)) {}

GuestBellTrap::~GuestBellTrap() = default;

void GuestBellTrap::CallHandler(async_t* async, async_guest_bell_trap_t* trap,
                                zx_status_t status, const zx_packet_guest_bell_t* bell) {
    auto self = Dispatch<GuestBellTrap>(trap);
    self->handler_(async, self, status, bell);
}

} // namespace async
