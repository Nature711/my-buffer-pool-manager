
#pragma once

#include "replacer.h"
#include "config.h"

/**
 * ClockReplacer implements the clock replacement policy, which approximates the Least Recently Used policy.
 */
class ClockReplacer : public Replacer
{
public:
    /**
     * Create a new ClockReplacer.
     * @param num_pages the maximum number of pages the ClockReplacer will be required to store
     */
    explicit ClockReplacer(size_t num_pages);

    /**
     * Destroys the ClockReplacer.
     */
    ~ClockReplacer() override;

    auto Victim(frame_id_t *frame_id) -> bool override;

    void Pin(frame_id_t frame_id) override;

    void Unpin(frame_id_t frame_id) override;

    auto Size() -> size_t override;

private:
    // TODO(student): implement me!
};
