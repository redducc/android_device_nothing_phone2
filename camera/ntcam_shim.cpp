/*
 * SPDX-FileCopyrightText: Paranoid Android
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ui/GraphicBufferMapper.h>

using android::GraphicBufferMapper;
using android::status_t;

// importBuffer() validates against NtCam's own size math, which no longer
// matches SnapAlloc's alignments. The blob is patched to call this instead.
extern "C" status_t _ZN7android19GraphicBufferMapper12importBuffeREPK13native_handlejjjimjPS3_(
        GraphicBufferMapper* thisptr, const native_handle_t* rawHandle, uint32_t, uint32_t,
        uint32_t, int, uint64_t, uint32_t, buffer_handle_t* outHandle) {
    return thisptr->importBufferNoValidate(rawHandle, outHandle);
}
