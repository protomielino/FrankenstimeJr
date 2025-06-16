#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include <gtk/gtk.h>
#include <cairo.h>

#include "telemetry.h"

#define WIDTH   640
#define HEIGHT  400
#define PAN

#define ALIGN_LEFT   1
#define ALIGN_RIGHT  2
#define ALIGN_MIDDLE 3

#define FLIP_VERTICAL(y) (-(y) + data->drawing_area_height)

typedef struct
{
    GtkWidget *window;
    GtkWidget *box;
    GtkWidget *fixed;
    GtkWidget *drawing_area;
    GtkWidget *label_coords;
    GtkWidget *label_zoom;
    GtkWidget *spin_lap;
    GtkWidget *file_chooser_button;
    GtkWidget *combo_box_cars;
    GtkWidget *button_remove;
    
    GtkListStore *list_store_cars;
    GtkAdjustment *adjustment_spin_lap;
} AppWidgets;

typedef struct
{
    double pan_x;
    double pan_y;
    double scale_x;
    double scale_y;

    // Variabili per pan e zoom
    double mouse_pos_X;
    double mouse_pos_Y;
    double offset_click_X;
    double offset_click_Y;
    double scroll_origin_X;
    double scroll_origin_Y;
    double select_start_x;
    double select_start_y;
    double select_end_x;
    double select_end_y;
    gboolean panning;
    gboolean zooming;
    gboolean selecting;
    gboolean space_pressed;
    gboolean ctrl_pressed;
    gboolean shift_pressed;

    int drawing_area_width;
    int drawing_area_height;
    float grid_min_x;
    float grid_max_x;
    float grid_min_y;
    float grid_max_y;
    float grid_spacing_x;
    float grid_spacing_y;

    int spin_lap_val;

    AppWidgets widgets;

    telemetry *tel;
} AppData;

typedef struct
{
    double x, y;
} point2d;


gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, AppData *data);
gboolean on_motion_notify_event(GtkWidget *widget, GdkEventMotion *event, AppData *data);
gboolean on_button_press_event(GtkWidget *widget, GdkEventButton *event, AppData *data);
gboolean on_button_release_event(GtkWidget *widget, GdkEventButton *event, AppData *data);
gboolean on_key_press_event(GtkWidget *widget, GdkEventKey *event, AppData *data);
gboolean on_key_release_event(GtkWidget *widget, GdkEventKey *event, AppData *data);
gboolean on_scroll_event(GtkWidget *widget, GdkEventScroll *event, AppData *data);
gboolean on_window_check_resize(GtkContainer *window, AppData *data);
gboolean on_spin_lap_value_changed(GtkWidget *widget, GdkEvent *event, AppData *data);
gboolean on_button_remove_clicked(GtkWidget *widget, AppData *data);
gboolean on_file_chooser_button_file_set(GtkWidget *widget, AppData *data);

#endif /* GRAPHICS_H_ */
