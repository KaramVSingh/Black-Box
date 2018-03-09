#ifndef DETAIL_H
#define DETAIL_H

#define GRID_DENSITY 16

enum class GateType {
    AND,
    OR,
    NOT,
    DFLIPFLOP,
    INPUT,
    OUTPUT,
    CUSTOM,
    DECODER,
    ENCODER,
    CLOCK
};

enum class Mode {
    place,
    zoomIn,
    zoomOut,
    move,
    wire,
    interact,
    blackBox
};

#endif // DETAIL_H
