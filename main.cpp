#include <iostream>
#include <memory>
#include <thread>
#include <atomic>
#include <gtk/gtk.h>
#include <dev/devs.hpp>

using namespace std;

// Global variables
std::shared_ptr<Device> g_device;
std::atomic<bool> g_device_connected{false};
std::string g_device_sn;

// GUI elements
GtkWidget *g_status_label;
GtkWidget *g_device_info_label;
GtkWidget *g_connect_button;
GtkWidget *g_speed_scale;

// Control state
std::atomic<bool> g_control_active{false};
double g_current_pitch = 0.0;
double g_current_pan = 0.0;
double g_speed_multiplier = 1.0;

// Device detection callback
void onDevChanged(std::string dev_sn, bool in_out, void *param)
{
    cout << "Device sn: " << dev_sn << (in_out ? " Connected" : " Disconnected") << endl;

    if (in_out && !g_device_connected)
    {
        auto dev = Devices::get().getDevBySn(dev_sn);
        if (dev && dev->productType() == ObsbotProdTinySE)
        {
            g_device = dev;
            g_device_sn = dev_sn;
            g_device_connected = true;

            // Update GUI on main thread
            g_idle_add([](gpointer data) -> gboolean {
                gtk_label_set_text(GTK_LABEL(g_status_label), "Status: Device Connected");

                std::string info = "Device: " + g_device->devName() + "\n";
                info += "Version: " + g_device->devVersion() + "\n";
                info += "SN: " + g_device_sn;
                gtk_label_set_text(GTK_LABEL(g_device_info_label), info.c_str());

                gtk_button_set_label(GTK_BUTTON(g_connect_button), "Disconnect");
                gtk_widget_set_sensitive(g_speed_scale, TRUE);

                return G_SOURCE_REMOVE;
            }, nullptr);
        }
    }
    else if (!in_out && dev_sn == g_device_sn)
    {
        g_device.reset();
        g_device_connected = false;
        g_control_active = false;

        // Update GUI on main thread
        g_idle_add([](gpointer data) -> gboolean {
            gtk_label_set_text(GTK_LABEL(g_status_label), "Status: No Device");
            gtk_label_set_text(GTK_LABEL(g_device_info_label), "Waiting for OBSBOT Tiny SE...");
            gtk_button_set_label(GTK_BUTTON(g_connect_button), "Connect");
            gtk_widget_set_sensitive(g_speed_scale, FALSE);
            return G_SOURCE_REMOVE;
        }, nullptr);
    }
}

// Send gimbal control command
void sendGimbalControl()
{
    if (g_device_connected && g_device && g_control_active)
    {
        double pitch = g_current_pitch * g_speed_multiplier;
        double pan = g_current_pan * g_speed_multiplier;
        g_device->aiSetGimbalSpeedCtrlR(pitch, pan, 200.0);
    }
}

// Stop gimbal movement
void stopGimbal()
{
    if (g_device_connected && g_device)
    {
        g_device->aiSetGimbalSpeedCtrlR(0, 0, 0);
    }
}

// Button press handlers
void on_direction_pressed(GtkWidget *widget, gpointer data)
{
    if (!g_device_connected) return;

    const char* direction = (const char*)data;
    g_control_active = true;

    if (strcmp(direction, "up") == 0) {
        g_current_pitch = -60.0;  // Inverted
        g_current_pan = 0.0;
    }
    else if (strcmp(direction, "down") == 0) {
        g_current_pitch = 60.0;  // Inverted
        g_current_pan = 0.0;
    }
    else if (strcmp(direction, "left") == 0) {
        g_current_pitch = 0.0;
        g_current_pan = -60.0;
    }
    else if (strcmp(direction, "right") == 0) {
        g_current_pitch = 0.0;
        g_current_pan = 60.0;
    }
    else if (strcmp(direction, "upleft") == 0) {
        g_current_pitch = -45.0;  // Inverted
        g_current_pan = -45.0;
    }
    else if (strcmp(direction, "upright") == 0) {
        g_current_pitch = -45.0;  // Inverted
        g_current_pan = 45.0;
    }
    else if (strcmp(direction, "downleft") == 0) {
        g_current_pitch = 45.0;  // Inverted
        g_current_pan = -45.0;
    }
    else if (strcmp(direction, "downright") == 0) {
        g_current_pitch = 45.0;  // Inverted
        g_current_pan = 45.0;
    }

    sendGimbalControl();
}

void on_direction_released(GtkWidget *widget, gpointer data)
{
    g_control_active = false;
    g_current_pitch = 0.0;
    g_current_pan = 0.0;
    stopGimbal();
}

// Speed slider handler
void on_speed_changed(GtkRange *range, gpointer data)
{
    g_speed_multiplier = gtk_range_get_value(range);
}

// Connect button handler
void on_connect_clicked(GtkWidget *widget, gpointer data)
{
    if (g_device_connected)
    {
        // Disconnect
        g_device.reset();
        g_device_connected = false;
        g_control_active = false;

        gtk_label_set_text(GTK_LABEL(g_status_label), "Status: Disconnected");
        gtk_label_set_text(GTK_LABEL(g_device_info_label), "Waiting for OBSBOT Tiny SE...");
        gtk_button_set_label(GTK_BUTTON(widget), "Connect");
        gtk_widget_set_sensitive(g_speed_scale, FALSE);
    }
}

// Center button handler
void on_center_clicked(GtkWidget *widget, gpointer data)
{
    if (g_device_connected && g_device)
    {
        // Move to center position (0, 0, 90)
        g_device->aiSetGimbalMotorAngleR(0.0f, 0.0f, 90.0f);
    }
}

// Create a direction button
GtkWidget* create_direction_button(const char* label, const char* direction)
{
    GtkWidget *button = gtk_button_new_with_label(label);
    gtk_widget_set_size_request(button, 80, 60);

    g_signal_connect(button, "pressed", G_CALLBACK(on_direction_pressed), (gpointer)direction);
    g_signal_connect(button, "released", G_CALLBACK(on_direction_released), (gpointer)direction);

    return button;
}

// Main window activation
static void activate(GtkApplication *app, gpointer user_data)
{
    GtkWidget *window;
    GtkWidget *main_box;
    GtkWidget *status_box;
    GtkWidget *control_frame;
    GtkWidget *control_box;
    GtkWidget *direction_grid;
    GtkWidget *speed_box;
    GtkWidget *speed_label;
    GtkWidget *button_box;
    GtkWidget *center_button;

    // Create main window
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "OBSBOT Tiny SE Pan-Tilt Control");
    gtk_window_set_default_size(GTK_WINDOW(window), 450, 550);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    // Main vertical box
    main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), main_box);

    // Status section
    status_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_pack_start(GTK_BOX(main_box), status_box, FALSE, FALSE, 0);

    g_status_label = gtk_label_new("Status: Initializing...");
    gtk_box_pack_start(GTK_BOX(status_box), g_status_label, FALSE, FALSE, 0);

    g_device_info_label = gtk_label_new("Waiting for OBSBOT Tiny SE...");
    gtk_label_set_justify(GTK_LABEL(g_device_info_label), GTK_JUSTIFY_LEFT);
    gtk_box_pack_start(GTK_BOX(status_box), g_device_info_label, FALSE, FALSE, 0);

    g_connect_button = gtk_button_new_with_label("Connect");
    gtk_box_pack_start(GTK_BOX(status_box), g_connect_button, FALSE, FALSE, 0);
    g_signal_connect(g_connect_button, "clicked", G_CALLBACK(on_connect_clicked), NULL);

    // Control frame
    control_frame = gtk_frame_new("Pan-Tilt Control");
    gtk_box_pack_start(GTK_BOX(main_box), control_frame, TRUE, TRUE, 0);

    control_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(control_box), 10);
    gtk_container_add(GTK_CONTAINER(control_frame), control_box);

    // Direction control grid (3x3)
    direction_grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(direction_grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(direction_grid), 5);
    gtk_widget_set_halign(direction_grid, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(control_box), direction_grid, TRUE, TRUE, 0);

    // Top row
    gtk_grid_attach(GTK_GRID(direction_grid), create_direction_button("↖", "upleft"), 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(direction_grid), create_direction_button("↑", "up"), 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(direction_grid), create_direction_button("↗", "upright"), 2, 0, 1, 1);

    // Middle row
    gtk_grid_attach(GTK_GRID(direction_grid), create_direction_button("←", "left"), 0, 1, 1, 1);

    center_button = gtk_button_new_with_label("Center");
    gtk_widget_set_size_request(center_button, 80, 60);
    g_signal_connect(center_button, "clicked", G_CALLBACK(on_center_clicked), NULL);
    gtk_grid_attach(GTK_GRID(direction_grid), center_button, 1, 1, 1, 1);

    gtk_grid_attach(GTK_GRID(direction_grid), create_direction_button("→", "right"), 2, 1, 1, 1);

    // Bottom row
    gtk_grid_attach(GTK_GRID(direction_grid), create_direction_button("↙", "downleft"), 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(direction_grid), create_direction_button("↓", "down"), 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(direction_grid), create_direction_button("↘", "downright"), 2, 2, 1, 1);

    // Speed control
    speed_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_pack_start(GTK_BOX(control_box), speed_box, FALSE, FALSE, 0);

    speed_label = gtk_label_new("Speed Multiplier:");
    gtk_box_pack_start(GTK_BOX(speed_box), speed_label, FALSE, FALSE, 0);

    g_speed_scale = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0.1, 2.0, 0.1);
    gtk_range_set_value(GTK_RANGE(g_speed_scale), 1.0);
    gtk_scale_set_value_pos(GTK_SCALE(g_speed_scale), GTK_POS_RIGHT);
    gtk_widget_set_sensitive(g_speed_scale, FALSE);
    g_signal_connect(g_speed_scale, "value-changed", G_CALLBACK(on_speed_changed), NULL);
    gtk_box_pack_start(GTK_BOX(speed_box), g_speed_scale, FALSE, FALSE, 0);

    // Instructions
    GtkWidget *instructions = gtk_label_new(
        "Instructions:\n"
        "• Press and hold direction buttons to move\n"
        "• Click 'Center' to return to center position\n"
        "• Adjust speed multiplier as needed"
    );
    gtk_label_set_justify(GTK_LABEL(instructions), GTK_JUSTIFY_LEFT);
    gtk_box_pack_start(GTK_BOX(main_box), instructions, FALSE, FALSE, 0);

    gtk_widget_show_all(window);
}

int main(int argc, char **argv)
{
    cout << "OBSBOT Tiny SE Pan-Tilt Control GUI" << endl;
    cout << "Initializing device detection..." << endl;

    // Register device changed callback
    Devices::get().setDevChangedCallback(onDevChanged, nullptr);

    // Disable mdns scan (for USB devices)
    Devices::get().setEnableMdnsScan(false);

    // Create GTK application
    GtkApplication *app;
    int status;

    app = gtk_application_new("com.obsbot.pantilt", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    // Cleanup
    if (g_device_connected && g_device)
    {
        stopGimbal();
    }
    Devices::get().close();

    return status;
}
