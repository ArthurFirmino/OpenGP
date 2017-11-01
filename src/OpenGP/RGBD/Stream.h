// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <unordered_map>

#include <OpenGP/headeronly.h>
#include <OpenGP/types.h>
#include <OpenGP/util/GenericIterable.h>


//=============================================================================
namespace OpenGP {
//=============================================================================

struct StreamIntrinsics {
    int width, height;
    Vec2 pixel_center;
    Vec2 focal_length;
};

struct StreamExtrinsics {
    Vec3 translation;
    Mat3x3 rotation;
};

class SensorStream {
private:

    const void **data_ptr;

    StreamIntrinsics intrinsics;
    StreamExtrinsics extrinsics;

public:

    HEADERONLY_INLINE SensorStream(const void **data_ptr, const StreamIntrinsics &intrinsics, const StreamExtrinsics &extrinsics);

    HEADERONLY_INLINE const void *get_data() const;

    const StreamIntrinsics &get_intrinsics() const { return intrinsics; }
    const StreamExtrinsics &get_extrinsics() const { return extrinsics; }

};

class SensorDevice {
private:

    std::unordered_map<std::string, SensorStream> streams;

    std::function<bool(bool)> advance_frame_callback;

public:

    template <typename Iterable>
    SensorDevice(const Iterable &pairs, std::function<bool(bool)> advance_frame_callback) :
        streams(pairs.begin(), pairs.end()),
        advance_frame_callback(advance_frame_callback) {}

    HEADERONLY_INLINE void advance_frame();
    HEADERONLY_INLINE bool try_advance_frame();

    HEADERONLY_INLINE const SensorStream &get_stream(const char *name) const;

    HEADERONLY_INLINE GenericIterable<const SensorStream> get_streams() const;

};

//=============================================================================
} // OpenGP::
//=============================================================================

#ifdef HEADERONLY
    #include "Stream.cpp"
#endif
