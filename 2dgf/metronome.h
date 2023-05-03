#ifndef __METRONOME_H__
#define __METRONOME_H__

#include <stdio.h>
#include <stdlib.h>
#include "gfc_vector.h"
#include "entity.h"
#include "simple_logger.h"

Entity* metronome_new(Vector2D position, int BPM);
int metronome_isBeatTrue();

#endif