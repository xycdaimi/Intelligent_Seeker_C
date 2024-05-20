#pragma once

#include <assert.h>
#include "WakeWord/pa_ringbuffer.h"
#include "WakeWord/pa_util.h"
#include "WakeWord/portaudio.h"
#include <stdio.h>
#include <stdlib.h>
#include "WakeWord/snowboy-detect-c-wrapper.h"
#include <string>

namespace Watchdog
{
    using FUNCTION = void(void *);

    void working(std::string resource, std::string model, std::string sensitivity_str, bool *is_exit, FUNCTION fun, void *path);
} // namespace Watchdog
