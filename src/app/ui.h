//
// Copyright (c) 2025, SpargatTeam
// Wrote by Comical
//
#ifndef FLUX_UI_H
#define FLUX_UI_H

#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Tabs.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Menu_Bar.H>
#include <string>
#include <vector>

namespace flux_ui {

class WebTab : public Fl_Group {
public:
    WebTab(int x, int y, int w, int h, const char* title);
    void load_url(const std::string& url);
    Fl_Input* get_address_input() { return address_input; }
    Fl_Button* get_go_button() { return go_button; }
    Fl_Text_Display* get_web_view() { return web_view; }
    void resize(int x, int y, int w, int h) override;

private:
    Fl_Input* address_input;
    Fl_Button* go_button;
    Fl_Text_Display* web_view;
};

class BrowserWindow : public Fl_Double_Window {
public:
    BrowserWindow(int w, int h, const char* title);
    void add_tab(const std::string& url = "https://www.google.com");
    void remove_tab(int index);
    void resize(int x, int y, int w, int h) override;

private:
    Fl_Tabs* tabs;
    Fl_Menu_Bar* menu_bar;
    std::vector<WebTab*> web_tabs;
    bool is_maximized;
    int prev_x, prev_y, prev_w, prev_h;
    static void new_tab_cb(Fl_Widget* w, void* v);
    static void close_tab_cb(Fl_Widget* w, void* v);
    static void go_url_cb(Fl_Widget* w, void* v);
    static void maximize_cb(Fl_Widget* w, void* v);
};

class BrowserApp {
public:
    static int run();
};

} 

#endif // FLUX_UI_H
