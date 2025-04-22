//------------------------------------------------------------------------
// Copyright(c) 2025 My Plug-in Company.
//------------------------------------------------------------------------

#pragma once

#include "pluginterfaces/base/funknown.h"
#include "pluginterfaces/vst/vsttypes.h"

namespace MyCompanyName {
//------------------------------------------------------------------------
static const Steinberg::FUID kVolumeKnobProcessorUID (0x9E237997, 0x9FCE5739, 0x803E1F60, 0x069FCF96);
static const Steinberg::FUID kVolumeKnobControllerUID (0x30E1A211, 0x70B150DD, 0x82A06BBC, 0x0CEF0EAF);

#define VolumeKnobVST3Category "Fx"

enum {
    kGainId = 1000
};


//------------------------------------------------------------------------
} // namespace MyCompanyName

