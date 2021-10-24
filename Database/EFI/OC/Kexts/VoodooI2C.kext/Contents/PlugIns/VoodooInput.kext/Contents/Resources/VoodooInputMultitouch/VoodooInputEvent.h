//
//  VoodooInputEvent.h
//  VooodooInput
//
//  Copyright © 2019 Kishor Prins. All rights reserved.
// Copyright (c) 2020 Leonard Kleinhans <leo-labs>
//

#ifndef VOODOO_INPUT_EVENT_H
#define VOODOO_INPUT_EVENT_H

#include "VoodooInputTransducer.h"

struct VoodooInputEvent {
    UInt8 contact_count;
    AbsoluteTime timestamp;
    VoodooInputTransducer transducers[VOODOO_INPUT_MAX_TRANSDUCERS];
};

struct VoodooInputDimensions {
    SInt32 min_x;
    SInt32 max_x;
    SInt32 min_y;
    SInt32 max_y;
};

struct RelativePointerEvent {
    uint64_t timestamp;
    int dx;
    int dy;
    int buttons;
};

struct ScrollWheelEvent {
    uint64_t timestamp;
    short deltaAxis1;
    short deltaAxis2;
    short deltaAxis3;
};

#endif /* VoodooInputEvent_h */
