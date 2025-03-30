#pragma once
#include <array>

// On d�finit plusieurs mod�les de feuilles
constexpr uint8_t OAK_LEAVES[5][5][6] = {
    {{0, 0, 0, 1, 0, 0}, {0, 1, 1, 1, 1, 0}, {0, 1, 1, 1, 1, 0}, {0, 1, 1, 1, 1, 0}, {0, 0, 0, 1, 0, 0}},
    {{0, 1, 1, 1, 1, 0}, {1, 1, 1, 1, 1, 1}, {1, 1, 1, 1, 1, 1}, {1, 1, 1, 1, 1, 1}, {0, 1, 1, 1, 1, 0}},
    {{0, 1, 1, 1, 1, 0}, {1, 1, 1, 1, 1, 1}, {0, 0, 1, 1, 1, 1}, {1, 1, 1, 1, 1, 1}, {0, 1, 1, 1, 1, 0}},
    {{0, 1, 1, 1, 1, 0}, {1, 1, 1, 1, 1, 1}, {1, 1, 1, 1, 1, 1}, {1, 1, 1, 1, 1, 1}, {0, 1, 1, 1, 1, 0}},
    {{0, 0, 0, 1, 0, 0}, {0, 1, 1, 1, 1, 0}, {0, 1, 1, 1, 1, 0}, {0, 1, 1, 1, 1, 0}, {0, 0, 0, 1, 0, 0}},
};

constexpr uint8_t OAK_LEAVES_2[5][5][6] = {
    {{0, 0, 0, 0, 0, 0}, {0, 1, 1, 0, 0, 0}, {0, 1, 1, 0, 0, 0}, {0, 1, 1, 0, 0, 0}, {0, 0, 0, 0, 0, 0}},
    {{0, 1, 1, 0, 0, 0}, {0, 1, 1, 1, 0, 0}, {0, 1, 1, 1, 1, 0}, {0, 1, 1, 0, 0, 0}, {0, 1, 1, 0, 0, 0}},
    {{0, 1, 1, 0, 0, 0}, {0, 1, 1, 1, 1, 0}, {0, 0, 1, 1, 1, 0}, {0, 1, 1, 1, 1, 0}, {0, 1, 1, 0, 0, 0}},
    {{0, 1, 1, 0, 0, 0}, {0, 1, 1, 0, 0, 0}, {0, 1, 1, 1, 1, 0}, {0, 1, 1, 0, 0, 0}, {0, 1, 1, 0, 0, 0}},
    {{0, 0, 0, 0, 0, 0}, {0, 1, 1, 0, 0, 0}, {0, 1, 1, 0, 0, 0}, {0, 1, 1, 0, 0, 0}, {0, 0, 0, 0, 0, 0}},
};