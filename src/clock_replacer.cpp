#include "clock_replacer.h"
#include <iostream>

ClockReplacer::ClockReplacer(size_t num_frames) : num_frames_(num_frames), clock_hand_(0), num_frames_in_replacer(0)
{
    // initialize ClockReplacer with the same number of frames as BPM since it contains placeholders for all frames in BPM
    frames_.resize(num_frames);
    // initially there is no frame in replacer
    for (auto &frame : frames_)
    {
        frame.is_in_replacer = false;
        frame.ref_bit = false;
    }
}

ClockReplacer::~ClockReplacer()
{
}

auto ClockReplacer::Victim(frame_id_t *frame_id) -> bool
{
    while (true)
    {
        clock_hand_ = (clock_hand_ + 1) / num_frames_;
        if (frames_[clock_hand_].is_in_replacer && !frames_[clock_hand_].ref_bit)
        {
            *frame_id = clock_hand_;
            num_frames_in_replacer--;
            return true;
        }
        frames_[clock_hand_].ref_bit = false;
    }
    return false;
}

void ClockReplacer::Pin(frame_id_t frame_id)
{
    frames_[frame_id].ref_bit = true;
    if (frames_[frame_id].is_in_replacer)
    {
        frames_[frame_id].is_in_replacer = false;
        num_frames_in_replacer--;
    }
}

void ClockReplacer::Unpin(frame_id_t frame_id)
{
    if (!frames_[frame_id].is_in_replacer)
    {
        frames_[frame_id].is_in_replacer = true;
        num_frames_in_replacer++;
    }
}

auto ClockReplacer::Size() -> size_t
{
    return num_frames_in_replacer;
}

void ClockReplacer::RemoveFromReplacer(frame_id_t frame_id)
{
    frames_[frame_id].is_in_replacer = false;
    num_frames_in_replacer--;
}
