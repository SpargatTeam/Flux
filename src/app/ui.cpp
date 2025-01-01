//
// Copyright (c) 2025, SpargatTeam
// Wrote by Comical
//
#include "ui.h"
#include "fluxium.h"
#include <FL/fl_ask.H>
#include <iostream>
#include <cstdlib>

namespace flux_ui {

void WebTab::resize(int x, int y, int w, int h) {
    Fl_Group::resize(x, y, w, h);
    int input_height = 30;
    address_input->resize(x + 10, y + 10, w - 150, input_height);
    go_button->resize(x + w - 130, y + 10, 120, input_height);
    web_view->resize(x + 10, y + 50, w - 20, h - 100);
}

WebTab::WebTab(int x, int y, int w, int h, const char* title) 
    : Fl_Group(x, y, w, h, title) {
    address_input = new Fl_Input(x + 10, y + 10, w - 150, 30);
    address_input->tooltip("Enter a web address");
    go_button = new Fl_Button(x + w - 130, y + 10, 120, 30, "Go");
    go_button->callback([](Fl_Widget* w, void* v) {
        WebTab* tab = (WebTab*)v;
        tab->load_url(tab->address_input->value());
    }, this);
    web_view = new Fl_Text_Display(x + 10, y + 50, w - 20, h - 100);
    web_view->buffer(new Fl_Text_Buffer());
    load_url("https://www.github.com/SpargatTeam/Flux");
    end(); 
}

void WebTab::load_url(const std::string& url) {
    try {
        std::string full_url = (url.find("://") == std::string::npos) 
            ? "https://" + url 
            : url;
        address_input->value(full_url.c_str());
        std::string content = fluxium::NetworkFetcher::fetch_url(full_url);
        auto html_root = fluxium::HTMLParser::parse(content);
        fluxium::Style style; 
        std::string rendered_content = fluxium::Renderer::render(html_root, style);
        web_view->buffer()->text(rendered_content.c_str());
    }
    catch (const std::exception& e) {
        fl_alert("Error loading URL: %s", e.what());
    }
}

BrowserWindow::BrowserWindow(int w, int h, const char* title) 
    : Fl_Double_Window(w, h, title), is_maximized(false), prev_x(0), prev_y(0), prev_w(0), prev_h(0) {
    menu_bar = new Fl_Menu_Bar(0, 0, w, 30);
    menu_bar->add("File/New Tab", FL_CTRL + 't', new_tab_cb, this);
    menu_bar->add("View/Maximize", FL_CTRL + 'm', maximize_cb, this);
    tabs = new Fl_Tabs(0, 30, w, h - 30);
    tabs->selection_color(FL_LIGHT2);
    add_tab();
    end();
}

void BrowserWindow::add_tab(const std::string& url) {
    WebTab* new_tab = new WebTab(
        0, 30, 
        w(), h() - 30, 
        "New Tab"
    );
    new_tab->load_url(url);
    web_tabs.push_back(new_tab);
    char label[20];
    snprintf(label, sizeof(label), "Tab %zu", web_tabs.size());
    new_tab->label(label);
}

void BrowserWindow::remove_tab(int index) {
    if (index < 0 || static_cast<size_t>(index) >= web_tabs.size()) return;
    auto it = web_tabs.begin() + index;
    delete *it;
    web_tabs.erase(it);
    if (web_tabs.empty()) {
        add_tab();
    }
}

void BrowserWindow::resize(int x, int y, int w, int h) {
    Fl_Double_Window::resize(x, y, w, h);
    menu_bar->resize(0, 0, w, 30);
    tabs->resize(0, 30, w, h - 30);
    for (auto* tab : web_tabs) {
        tab->resize(0, 30, w, h - 30);
    }
}

void BrowserWindow::new_tab_cb(Fl_Widget* w, void* v) {
    BrowserWindow* browser = (BrowserWindow*)v;
    browser->add_tab();
}

void BrowserWindow::close_tab_cb(Fl_Widget* w, void* v) {
    // TODO: Implement tab closing
}

void BrowserWindow::go_url_cb(Fl_Widget* w, void* v) {
    // TODO: Implement URL navigation
}

void BrowserWindow::maximize_cb(Fl_Widget* w, void* v) {
    BrowserWindow* browser = (BrowserWindow*)v;
    if (browser->is_maximized) {
        browser->size(browser->prev_w, browser->prev_h);
        browser->position(browser->prev_x, browser->prev_y);
        browser->is_maximized = false;
    } else {
        browser->prev_x = browser->x();
        browser->prev_y = browser->y();
        browser->prev_w = browser->w();
        browser->prev_h = browser->h();
        int screen_w = Fl::w();
        int screen_h = Fl::h();
        browser->resize(0, 0, screen_w, screen_h);
        browser->is_maximized = true;
    }
}

int BrowserApp::run() {
    BrowserWindow* window = new BrowserWindow(1024, 768, "Flux");
    window->show();
    return Fl::run();
}

} 