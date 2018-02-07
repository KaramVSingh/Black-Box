#ifndef DETAIL_H
#define DETAIL_H

#define GRID_DENSITY 16

enum class gateType {
    AND,
    OR,
    NOT,
    DFLIPFLOP,
    HIGH,
    LOW,
    OUTPUT,
    CUSTOM
};

enum class mode {
    place,
    zoomIn,
    zoomOut
};

#endif // DETAIL_H
