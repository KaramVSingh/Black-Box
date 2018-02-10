#ifndef DETAIL_H
#define DETAIL_H

#define GRID_DENSITY 16

enum class GateType {
    AND,
    OR,
    NOT,
    DFLIPFLOP,
    HIGH,
    LOW,
    OUTPUT,
    CUSTOM
};

enum class Mode {
    place,
    zoomIn,
    zoomOut,
    move,
    wire
};

#endif // DETAIL_H
