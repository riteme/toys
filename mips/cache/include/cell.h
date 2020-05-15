#pragma once

#include "device.h"

template <typename TDeviceProvider>
class Cell {
public:
    Cell() {
        _init();
    }

    auto operator=(const Cell &z) = delete;

    Cell(const Cell &z) {
        _init();
        set(z.get());
    }

    Cell(Cell &&z) {
        _init();
        set(z.get());
    }

    auto operator=(Cell &&z) {
        set(z.get());
        return *this;
    }

    bool operator<(const Cell &z) const {
        return get() < z.get();
    }

    auto get() const -> u32 {
        return _dev->read(_addr);
    }

    void set(u32 data) const {
        _dev->write(_addr, data);
    }

private:
    u32 _addr;
    Device *_dev;

    void _init() {
        auto provider = TDeviceProvider();
        _dev = provider();
        _addr = _dev->alloc();
    }
};