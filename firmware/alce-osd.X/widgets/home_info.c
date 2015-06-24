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


#include "alce-osd.h"

static struct widget_priv {
    struct home_data *home;
    struct canvas ca;
    struct widget_config *cfg;
} priv;

const struct widget home_info_widget;
extern struct alceosd_config config;

static void timer_callback(struct timer *t, void *d)
{
    schedule_widget(&home_info_widget);
}


static void init(struct widget_config *wcfg)
{
    struct canvas *ca = &priv.ca;

    priv.cfg = wcfg;
    priv.home = get_home_data();

    alloc_canvas(ca, wcfg, 92, 14*4);

    /* refresh rate of 0.5 sec */
    add_timer(TIMER_WIDGET, 5, timer_callback, NULL);
}


static int render(void)
{
    struct canvas *ca = &priv.ca;
    char buf[15];

    if (init_canvas(ca, 0))
        return 1;

    sprintf(buf, "home");
    draw_str3(buf, 0, 0, ca);

    if (priv.home->lock != HOME_LOCKED) {
        sprintf(buf, "no lock");
        draw_str3(buf, 0, 12, ca);
        if (priv.home->lock & HOME_LOCK_FIX)
            buf[0] = 'f';
        else
            buf[0] = ' ';
        if (priv.home->lock & HOME_LOCK_POS)
            buf[1] = 'p';
        else
            buf[1] = ' ';
        if (priv.home->lock & HOME_LOCK_ALT)
            buf[2] = 'a';
        else
            buf[2] = ' ';
        buf[3] = 0;
        draw_str3(buf, 0, 24, ca);
        sprintf(buf, "lock %d", config.home_lock_sec - priv.home->lock_sec);
        draw_str3(buf, 0, 12*3, ca);
    } else {
        sprintf(buf, "alt %dm", priv.home->altitude);
        draw_str3(buf, 0, 12, ca);
        sprintf(buf, "dis %dm", (unsigned int) priv.home->distance);
        draw_str3(buf, 0, 12*2, ca);
        sprintf(buf, "dir %d", priv.home->direction);
        draw_str3(buf, 0, 12*3, ca);
    }

    schedule_canvas(ca);
    return 0;
}


const struct widget home_info_widget = {
    .name = "Home info",
    .id = WIDGET_HOME_INFO_ID,
    .init = init,
    .render = render,
};
