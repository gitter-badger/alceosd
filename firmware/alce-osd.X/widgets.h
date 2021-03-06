/*
    AlceOSD - Graphical OSD
    Copyright (C) 2015  Luis Alves

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef WIDGETS_H
#define	WIDGETS_H

#include "videocore.h"


enum {
    WIDGET_ALTITUDE_ID = 0,
    WIDGET_HORIZON_ID,
    WIDGET_BATTERY_INFO_ID,
    WIDGET_COMPASS_ID,
    WIDGET_FLIGHT_MODE_ID,
    WIDGET_GPS_INFO_ID,
    WIDGET_RSSI_ID,
    WIDGET_SPEED_ID,
    WIDGET_THROTTLE_ID,
    WIDGET_VARIOMETER_ID,
    WIDGET_RC_CHANNELS_ID,
    WIDGET_HOME_INFO_ID,
    WIDGET_RADAR_ID,
    WIDGET_WIND_ID,
};

#define WIDGET_SCHEDULED    (0x1)

typedef union {
    unsigned int raw;
    struct {
        unsigned vjust:2;
        unsigned hjust:2;
        unsigned mode:4;
        unsigned units:3;
        unsigned :5;
    };
} widget_props;


struct widget_config {
    unsigned int tab;
    unsigned int widget_id;
    int x, y;
    widget_props props;
    unsigned int w, h;
};

struct widget;

struct widget_ops {
    unsigned int id;
    int (*init)(struct widget *w);
    void (*render)(struct widget *w);
    char name[];
};

struct widget {
    const struct widget_ops *ops;
    struct widget_config *cfg;
    void *priv;
    struct canvas ca;
    unsigned int status;
};

void* widget_malloc(unsigned int size);
void widgets_init(void);
void build_tab_list(void);
void load_tab(unsigned char tab);
void widgets_process(void);
void schedule_widget(struct widget *w);
const struct widget_ops *get_widget_ops(unsigned int id);

#endif
