#include <iostream>
#include <memory>
#include <thread>
#include <atomic>
#include <gtk/gtk.h>
#include <dev/devs.hpp>

using namespace std;

// グローバル変数
std::shared_ptr<Device> g_device;
std::atomic<bool> g_device_connected{false};
std::string g_device_sn;

// GUI要素
GtkWidget *g_status_label;
GtkWidget *g_device_info_label;
GtkWidget *g_connect_button;
GtkWidget *g_speed_scale;
GtkWidget *g_zoom_scale;
GtkWidget *g_gesture_toggle;

// 制御状態
std::atomic<bool> g_control_active{false};
double g_current_pitch = 0.0;
double g_current_pan = 0.0;
double g_speed_multiplier = 1.0;
float g_current_zoom = 1.0f;
bool g_gesture_enabled = false;

// デバイス検出コールバック
void onDevChanged(std::string dev_sn, bool in_out, void *param)
{
    cout << "Device sn: " << dev_sn << (in_out ? " 接続" : " 切断") << endl;

    if (in_out && !g_device_connected)
    {
        auto dev = Devices::get().getDevBySn(dev_sn);
        if (dev && dev->productType() == ObsbotProdTinySE)
        {
            g_device = dev;
            g_device_sn = dev_sn;
            g_device_connected = true;

            // メインスレッドでGUIを更新
            g_idle_add([](gpointer data) -> gboolean {
                gtk_label_set_text(GTK_LABEL(g_status_label), "ステータス: 接続済み");

                std::string info = "デバイス: " + g_device->devName() + "\n";
                info += "バージョン: " + g_device->devVersion() + "\n";
                info += "SN: " + g_device_sn;
                gtk_label_set_text(GTK_LABEL(g_device_info_label), info.c_str());

                gtk_button_set_label(GTK_BUTTON(g_connect_button), "切断");
                gtk_widget_set_sensitive(g_speed_scale, TRUE);
                gtk_widget_set_sensitive(g_zoom_scale, TRUE);
                gtk_widget_set_sensitive(g_gesture_toggle, TRUE);

                return G_SOURCE_REMOVE;
            }, nullptr);
        }
    }
    else if (!in_out && dev_sn == g_device_sn)
    {
        g_device.reset();
        g_device_connected = false;
        g_control_active = false;

        // メインスレッドでGUIを更新
        g_idle_add([](gpointer data) -> gboolean {
            gtk_label_set_text(GTK_LABEL(g_status_label), "ステータス: デバイスなし");
            gtk_label_set_text(GTK_LABEL(g_device_info_label), "OBSBOTカメラを待機中...");
            gtk_button_set_label(GTK_BUTTON(g_connect_button), "接続");
            gtk_widget_set_sensitive(g_speed_scale, FALSE);
            gtk_widget_set_sensitive(g_zoom_scale, FALSE);
            gtk_widget_set_sensitive(g_gesture_toggle, FALSE);
            return G_SOURCE_REMOVE;
        }, nullptr);
    }
}

// ジンバル制御コマンドを送信
void sendGimbalControl()
{
    if (g_device_connected && g_device && g_control_active)
    {
        double pitch = g_current_pitch * g_speed_multiplier;
        double pan = g_current_pan * g_speed_multiplier;
        g_device->aiSetGimbalSpeedCtrlR(pitch, pan, 200.0);
    }
}

// ジンバルを停止
void stopGimbal()
{
    if (g_device_connected && g_device)
    {
        g_device->aiSetGimbalSpeedCtrlR(0, 0, 0);
    }
}

// ボタン押下ハンドラ
void on_direction_pressed(GtkWidget *widget, gpointer data)
{
    if (!g_device_connected) return;

    const char* direction = (const char*)data;
    g_control_active = true;

    // AIスマートトラッキングをオフにする
    g_device->cameraSetAiModeU(Device::AiWorkModeNone);
    
    // ジェスチャー制御がオンの場合はオフにする
    if (g_gesture_enabled)
    {
        g_gesture_enabled = false;
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(g_gesture_toggle), FALSE);
        
        // 個別のジェスチャー機能も無効化
        g_device->aiSetGestureCtrlIndividualR(0, false);
        g_device->aiSetGestureCtrlIndividualR(1, false);
        g_device->aiSetGestureCtrlIndividualR(2, false);
        
        std::cout << "手動操作開始: AIスマートトラッキングとジェスチャー制御を無効化しました" << std::endl;
    }
    else
    {
        std::cout << "手動操作開始: AIスマートトラッキングを無効化しました" << std::endl;
    }

    if (strcmp(direction, "up") == 0) {
        g_current_pitch = -60.0;  // 反転
        g_current_pan = 0.0;
    }
    else if (strcmp(direction, "down") == 0) {
        g_current_pitch = 60.0;  // 反転
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
        g_current_pitch = -45.0;  // 反転
        g_current_pan = -45.0;
    }
    else if (strcmp(direction, "upright") == 0) {
        g_current_pitch = -45.0;  // 反転
        g_current_pan = 45.0;
    }
    else if (strcmp(direction, "downleft") == 0) {
        g_current_pitch = 45.0;  // 反転
        g_current_pan = -45.0;
    }
    else if (strcmp(direction, "downright") == 0) {
        g_current_pitch = 45.0;  // 反転
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

// 速度スライダーハンドラ
void on_speed_changed(GtkRange *range, gpointer data)
{
    g_speed_multiplier = gtk_range_get_value(range);
}

// ズームスライダーハンドラ
void on_zoom_changed(GtkRange *range, gpointer data)
{
    if (g_device_connected && g_device)
    {
        g_current_zoom = (float)gtk_range_get_value(range);
        g_device->cameraSetZoomAbsoluteR(g_current_zoom);
    }
}

// ジェスチャー制御トグルハンドラ
void on_gesture_toggled(GtkToggleButton *toggle, gpointer data)
{
    if (g_device_connected && g_device)
    {
        g_gesture_enabled = gtk_toggle_button_get_active(toggle);
        
        // 個別のジェスチャー機能を有効/無効にする
        // 0: ジェスチャーターゲット
        g_device->aiSetGestureCtrlIndividualR(0, g_gesture_enabled);
        // 1: ジェスチャーズーム
        g_device->aiSetGestureCtrlIndividualR(1, g_gesture_enabled);
        // 2: ジェスチャーダイナミックズーム (tiny2シリーズ、tail air用)
        g_device->aiSetGestureCtrlIndividualR(2, g_gesture_enabled);
        
        // ジェスチャー制御をオフにする時、人物追跡もオフにする
        if (!g_gesture_enabled)
        {
            // Tiny SEではcameraSetAiModeU()でAIスマートトラッキングを無効化
            g_device->cameraSetAiModeU(Device::AiWorkModeNone);
            std::cout << "人物追跡を無効化しました" << std::endl;
        }
        
        std::cout << "ジェスチャー制御 " << (g_gesture_enabled ? "有効" : "無効") << std::endl;
        std::cout << "  - ジェスチャーターゲット: " << (g_gesture_enabled ? "ON" : "OFF") << std::endl;
        std::cout << "  - ジェスチャーズーム: " << (g_gesture_enabled ? "ON" : "OFF") << std::endl;
        std::cout << "  - ジェスチャーダイナミックズーム: " << (g_gesture_enabled ? "ON" : "OFF") << std::endl;
    }
}

// 接続ボタンハンドラ
void on_connect_clicked(GtkWidget *widget, gpointer data)
{
    if (g_device_connected)
    {
        // 切断
        g_device.reset();
        g_device_connected = false;
        g_control_active = false;

        gtk_label_set_text(GTK_LABEL(g_status_label), "ステータス: 切断済み");
        gtk_label_set_text(GTK_LABEL(g_device_info_label), "OBSBOTカメラを待機中...");
        gtk_button_set_label(GTK_BUTTON(widget), "接続");
        gtk_widget_set_sensitive(g_speed_scale, FALSE);
        gtk_widget_set_sensitive(g_zoom_scale, FALSE);
        gtk_widget_set_sensitive(g_gesture_toggle, FALSE);
    }
}

// センターボタンハンドラ
void on_center_clicked(GtkWidget *widget, gpointer data)
{
    if (g_device_connected && g_device)
    {
        // AIスマートトラッキングをオフにする
        g_device->cameraSetAiModeU(Device::AiWorkModeNone);
        
        // ジェスチャー制御がオンの場合はオフにする
        if (g_gesture_enabled)
        {
            g_gesture_enabled = false;
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(g_gesture_toggle), FALSE);
            
            // 個別のジェスチャー機能も無効化
            g_device->aiSetGestureCtrlIndividualR(0, false);
            g_device->aiSetGestureCtrlIndividualR(1, false);
            g_device->aiSetGestureCtrlIndividualR(2, false);
            
            std::cout << "センター移動: AIスマートトラッキングとジェスチャー制御を無効化しました" << std::endl;
        }
        else
        {
            std::cout << "センター移動: AIスマートトラッキングを無効化しました" << std::endl;
        }
        
        // センター位置に移動 (0, 0, 90)
        g_device->aiSetGimbalMotorAngleR(0.0f, 0.0f, 90.0f);
    }
}

// 方向ボタンを作成
GtkWidget* create_direction_button(const char* label, const char* direction)
{
    GtkWidget *button = gtk_button_new_with_label(label);
    gtk_widget_set_size_request(button, 80, 60);

    g_signal_connect(button, "pressed", G_CALLBACK(on_direction_pressed), (gpointer)direction);
    g_signal_connect(button, "released", G_CALLBACK(on_direction_released), (gpointer)direction);

    return button;
}

// メインウィンドウのアクティベーション
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
    GtkWidget *zoom_box;
    GtkWidget *zoom_label;
    GtkWidget *gesture_box;
    GtkWidget *button_box;
    GtkWidget *center_button;

    // メインウィンドウを作成
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "OBSBOT Linux GUI");
    gtk_window_set_default_size(GTK_WINDOW(window), 450, 650);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    // メイン垂直ボックス
    main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), main_box);

    // ステータスセクション
    status_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_pack_start(GTK_BOX(main_box), status_box, FALSE, FALSE, 0);

    g_status_label = gtk_label_new("ステータス: 初期化中...");
    gtk_box_pack_start(GTK_BOX(status_box), g_status_label, FALSE, FALSE, 0);

    g_device_info_label = gtk_label_new("OBSBOTカメラを待機中...");
    gtk_label_set_justify(GTK_LABEL(g_device_info_label), GTK_JUSTIFY_LEFT);
    gtk_box_pack_start(GTK_BOX(status_box), g_device_info_label, FALSE, FALSE, 0);

    g_connect_button = gtk_button_new_with_label("接続");
    gtk_box_pack_start(GTK_BOX(status_box), g_connect_button, FALSE, FALSE, 0);
    g_signal_connect(g_connect_button, "clicked", G_CALLBACK(on_connect_clicked), NULL);

    // 制御フレーム
    control_frame = gtk_frame_new("パンチルト制御");
    gtk_box_pack_start(GTK_BOX(main_box), control_frame, TRUE, TRUE, 0);

    control_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(control_box), 10);
    gtk_container_add(GTK_CONTAINER(control_frame), control_box);

    // 方向制御グリッド (3x3)
    direction_grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(direction_grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(direction_grid), 5);
    gtk_widget_set_halign(direction_grid, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(control_box), direction_grid, TRUE, TRUE, 0);

    // 上段
    gtk_grid_attach(GTK_GRID(direction_grid), create_direction_button("↖", "upleft"), 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(direction_grid), create_direction_button("↑", "up"), 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(direction_grid), create_direction_button("↗", "upright"), 2, 0, 1, 1);

    // 中段
    gtk_grid_attach(GTK_GRID(direction_grid), create_direction_button("←", "left"), 0, 1, 1, 1);

    center_button = gtk_button_new_with_label("中央");
    gtk_widget_set_size_request(center_button, 80, 60);
    g_signal_connect(center_button, "clicked", G_CALLBACK(on_center_clicked), NULL);
    gtk_grid_attach(GTK_GRID(direction_grid), center_button, 1, 1, 1, 1);

    gtk_grid_attach(GTK_GRID(direction_grid), create_direction_button("→", "right"), 2, 1, 1, 1);

    // 下段
    gtk_grid_attach(GTK_GRID(direction_grid), create_direction_button("↙", "downleft"), 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(direction_grid), create_direction_button("↓", "down"), 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(direction_grid), create_direction_button("↘", "downright"), 2, 2, 1, 1);

    // 速度制御
    speed_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_pack_start(GTK_BOX(control_box), speed_box, FALSE, FALSE, 0);

    speed_label = gtk_label_new("速度倍率:");
    gtk_box_pack_start(GTK_BOX(speed_box), speed_label, FALSE, FALSE, 0);

    g_speed_scale = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0.1, 2.0, 0.1);
    gtk_range_set_value(GTK_RANGE(g_speed_scale), 1.0);
    gtk_scale_set_value_pos(GTK_SCALE(g_speed_scale), GTK_POS_RIGHT);
    gtk_widget_set_sensitive(g_speed_scale, FALSE);
    g_signal_connect(g_speed_scale, "value-changed", G_CALLBACK(on_speed_changed), NULL);
    gtk_box_pack_start(GTK_BOX(speed_box), g_speed_scale, FALSE, FALSE, 0);

    // ズーム制御
    zoom_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_pack_start(GTK_BOX(control_box), zoom_box, FALSE, FALSE, 0);

    zoom_label = gtk_label_new("ズームレベル:");
    gtk_box_pack_start(GTK_BOX(zoom_box), zoom_label, FALSE, FALSE, 0);

    g_zoom_scale = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 1.0, 2.0, 0.05);
    gtk_range_set_value(GTK_RANGE(g_zoom_scale), 1.0);
    gtk_scale_set_value_pos(GTK_SCALE(g_zoom_scale), GTK_POS_RIGHT);
    gtk_scale_set_digits(GTK_SCALE(g_zoom_scale), 2);
    gtk_widget_set_sensitive(g_zoom_scale, FALSE);
    g_signal_connect(g_zoom_scale, "value-changed", G_CALLBACK(on_zoom_changed), NULL);
    gtk_box_pack_start(GTK_BOX(zoom_box), g_zoom_scale, FALSE, FALSE, 0);

    // ジェスチャー制御トグル
    gesture_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(control_box), gesture_box, FALSE, FALSE, 0);
    gtk_widget_set_halign(gesture_box, GTK_ALIGN_CENTER);

    g_gesture_toggle = gtk_check_button_new_with_label("ジェスチャー制御を有効化");
    gtk_widget_set_sensitive(g_gesture_toggle, FALSE);
    g_signal_connect(g_gesture_toggle, "toggled", G_CALLBACK(on_gesture_toggled), NULL);
    gtk_box_pack_start(GTK_BOX(gesture_box), g_gesture_toggle, FALSE, FALSE, 0);

    // 使用方法
    GtkWidget *instructions = gtk_label_new(
        "使用方法:\n"
        "• 方向ボタンを押し続けて移動\n"
        "• 「中央」をクリックして中央位置に戻る\n"
        "• 速度倍率とズームレベルを調整\n"
        "• 「ジェスチャー制御を有効化」で機能のオン・オフ\n"
    );
    gtk_label_set_justify(GTK_LABEL(instructions), GTK_JUSTIFY_LEFT);
    gtk_box_pack_start(GTK_BOX(main_box), instructions, FALSE, FALSE, 0);

    gtk_widget_show_all(window);
}

int main(int argc, char **argv)
{
    cout << "OBSBOT Linux GUI" << endl;
    cout << "デバイス検出を初期化中..." << endl;

    // デバイス変更コールバックを登録
    Devices::get().setDevChangedCallback(onDevChanged, nullptr);

    // mdnsスキャンを無効化 (USBデバイス用)
    Devices::get().setEnableMdnsScan(false);

    // GTKアプリケーションを作成
    GtkApplication *app;
    int status;

    app = gtk_application_new("com.obsbot.pantilt", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    // クリーンアップ
    if (g_device_connected && g_device)
    {
        stopGimbal();
    }
    Devices::get().close();

    return status;
}
