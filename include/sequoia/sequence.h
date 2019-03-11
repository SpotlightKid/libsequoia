/*

    Copyright 2018, Chris Chronopoulos

    This file is part of libsequoia.

    libsequoia is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    libsequoia is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with libsequoia.  If not, see <https://www.gnu.org/licenses/>.

*/

#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <pthread.h>
#include <jack/midiport.h>
#include <jack/ringbuffer.h>

#include "trigger.h"
#include "port.h"

#define MAX_NAME_LENGTH 255

typedef jack_midi_data_t midi_packet[3]; // equivalent to 3 unsigned chars

enum _sequence_param {SEQUENCE_SET_TRIG, SEQUENCE_CLEAR_TRIG, SEQUENCE_TRANSPOSE, SEQUENCE_PH,
                        SEQUENCE_DIV, SEQUENCE_MUTE};

typedef struct {

    enum _sequence_param param;

    // parameter-dependent value fields
    int vi;
    float vf;
    bool vb;
    sq_trigger_t *vp;

} _sequence_ctrl_msg_t;

typedef struct {

    // these are touched by both the audio and the UI thread
    char name[MAX_NAME_LENGTH + 1];
    int transpose;
    sq_trigger_t *trigs;
    sq_trigger_t **microgrid;
    int ph;
    midi_packet *buf_off;
    int ridx_off;

    // TBD
    sq_port_t *outport;
    void *outport_buf;

    // this is only touched by UI
    bool is_playing;

    int nsteps;
    int tps;
    int nticks;

    jack_ringbuffer_t *rb;

    int div, idiv;
    bool mute;

} sq_sequence_t;

void sq_sequence_init(sq_sequence_t*, int, int);

void _sequence_prepare_outport(sq_sequence_t*, jack_nframes_t);
void _sequence_tick(sq_sequence_t*, jack_nframes_t);
void _sequence_reset_now(sq_sequence_t*);

void sq_sequence_set_name(sq_sequence_t*, const char*);
void sq_sequence_set_outport(sq_sequence_t*, sq_port_t*);

void sq_sequence_set_trig(sq_sequence_t*, int, sq_trigger_t*);
void _sequence_set_trig_now(sq_sequence_t*, int, sq_trigger_t*);

void sq_sequence_clear_trig(sq_sequence_t*, int);
void _sequence_clear_trig_now(sq_sequence_t*, int);

void sq_sequence_set_transpose(sq_sequence_t*, int);
void _sequence_set_transpose_now(sq_sequence_t*, int);

void sq_sequence_set_playhead(sq_sequence_t*, int);
void _sequence_set_playhead_now(sq_sequence_t*, int);

void sq_sequence_set_clockdivide(sq_sequence_t*, int);
void _sequence_set_clockdivide_now(sq_sequence_t*, int);

void sq_sequence_set_mute(sq_sequence_t*, bool);
void _sequence_set_mute_now(sq_sequence_t*, bool);

void sq_sequence_pprint(sq_sequence_t*);

int sq_sequence_get_nsteps(sq_sequence_t*);

#endif
