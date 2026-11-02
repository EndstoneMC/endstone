#pragma once

class BedHelper {
public:
    float north_dir_;
    float south_dir_;
    float west_dir_;
    float east_dir_;
    float bed_offset_x_;
    float bed_offset_z_;
    float mob_offset_west_x_;
    float mob_offset_east_x_;
    float mob_offset_south_z_;
    float mob_offset_north_z_;
};

static_assert(sizeof(BedHelper) == 40);
