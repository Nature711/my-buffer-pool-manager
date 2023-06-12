#pragma once

#include "replacer.h"
#include "config.h"

#include <vector>

/**
 * ClockReplacer implements the clock replacement policy, which approximates the Least Recently Used policy.
 */
class ClockReplacer : public Replacer
{
public:
    /**
     * Create a new ClockReplacer.
     * @param num_frames the maximum number of frames the ClockReplacer will be required to store
     */
    explicit ClockReplacer(size_t num_frames);

    /**
     * Destroys the ClockReplacer.
     */
    ~ClockReplacer() override;

    auto Victim(frame_id_t *frame_id) -> bool override;

    void Pin(frame_id_t frame_id) override;

    void Unpin(frame_id_t frame_id) override;

    auto Size() -> size_t override;

private:
    struct FrameInfo
    {
        bool ref_bit;        // has the frame been recently unpinned (ref flag set to false)?
        bool is_in_replacer; // is the frame currently in the replacer?
    };

    size_t num_frames_;             // total number of frames in BPM
    std::vector<FrameInfo> frames_; // represents the frames in the buffer pool
    size_t clock_hand_;
    size_t num_frames_in_replacer;
};
