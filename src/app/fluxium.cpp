//
// Copyright (c) 2025, SpargatTeam
// Wrote by Comical
//
#include "fluxium.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <curl/curl.h>

namespace fluxium {

static size_t write_callback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t total_size = size * nmemb;
    output->append((char*)contents, total_size);
    return total_size;
}

Element::Element(Type type, const std::string& content) 
    : type(type), content(content) {}

void Element::set_attribute(const std::string& key, const std::string& value) {
    attributes[key] = value;
}

std::string Element::get_attribute(const std::string& key) const {
    auto it = attributes.find(key);
    return (it != attributes.end()) ? it->second : "";
}

void Element::add_child(std::shared_ptr<Element> child) {
    children.push_back(child);
}

void Style::add_rule(const std::string& selector, const std::string& property, const std::string& value) {
    styles[selector][property] = value;
}

std::string Style::get_style(const std::string& selector, const std::string& property) const {
    auto selector_it = styles.find(selector);
    if (selector_it != styles.end()) {
        auto prop_it = selector_it->second.find(property);
        if (prop_it != selector_it->second.end()) {
            return prop_it->second;
        }
    }
    return "";
}

std::shared_ptr<Element> HTMLParser::parse(const std::string& html) {
    std::shared_ptr<Element> root = std::make_shared<Element>(Element::DIV, "");
    std::regex tag_regex("<([a-zA-Z0-9]+)([^>]*)>(.*?)</\\1>");
    std::sregex_iterator it(html.begin(), html.end(), tag_regex);
    std::sregex_iterator end;
    while (it != end) {
        std::string tag_name = (*it)[1];
        std::string attributes = (*it)[2];
        std::string content = (*it)[3];
        auto element = std::make_shared<Element>(get_element_type(tag_name), content);
        std::regex attr_regex("(\\w+)=\"([^\"]+)\"");
        std::sregex_iterator attr_it(attributes.begin(), attributes.end(), attr_regex);
        std::sregex_iterator attr_end;
        while (attr_it != attr_end) {
            element->set_attribute((*attr_it)[1], (*attr_it)[2]);
            ++attr_it;
        }
        root->add_child(element);
        ++it;
    }
    return root;
}

Element::Type HTMLParser::get_element_type(const std::string& tag_name) {
    static const std::map<std::string, Element::Type> type_map = {
        {"div", Element::DIV},
        {"span", Element::SPAN},
        {"p", Element::P},
        {"a", Element::A},
        {"img", Element::IMG},
        {"h1", Element::H1},
        {"h2", Element::H2},
        {"h3", Element::H3},
        {"h4", Element::H4},
        {"h5", Element::H5},
        {"h6", Element::H6}
    };
    auto it = type_map.find(tag_name);
    return (it != type_map.end()) ? it->second : Element::UNKNOWN;
}

Style CSSParser::parse(const std::string& css) {
    Style style;
    std::regex rule_regex("([^{]+)\\{([^}]+)\\}");
    std::sregex_iterator it(css.begin(), css.end(), rule_regex);
    std::sregex_iterator end;
    while (it != end) {
        std::string selector = (*it)[1];
        std::string rules = (*it)[2];
        selector.erase(0, selector.find_first_not_of(" \t"));
        selector.erase(selector.find_last_not_of(" \t") + 1);
        std::regex prop_regex("([^:]+):([^;]+);?");
        std::sregex_iterator prop_it(rules.begin(), rules.end(), prop_regex);
        std::sregex_iterator prop_end;
        while (prop_it != prop_end) {
            std::string property = (*prop_it)[1];
            std::string value = (*prop_it)[2];
            property.erase(0, property.find_first_not_of(" \t"));
            property.erase(property.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);
            style.add_rule(selector, property, value);
            ++prop_it;
        }
        ++it;
    }
    return style;
}

std::string Script::evaluate(const std::string& script) {
    std::stringstream result;
    result << "Script evaluation not fully implemented: " << script;
    return result.str();
}

std::string Renderer::render(std::shared_ptr<Element> root, const Style& style) {
    std::stringstream output;
    std::function<void(std::shared_ptr<Element>, int)> render_element = 
        [&](std::shared_ptr<Element> element, int indent) {
        std::string indentation(indent * 2, ' ');
        switch (element->type) {
            case Element::H1: output << indentation << "# "; break;
            case Element::H2: output << indentation << "## "; break;
            case Element::H3: output << indentation << "### "; break;
            case Element::DIV: output << indentation << "[DIV] "; break;
            case Element::P: output << indentation; break;
            default: output << indentation << "[ELEMENT] "; break;
        }
        output << element->content << std::endl;
        for (auto& child : element->children) {
            render_element(child, indent + 1);
        }
    };
    render_element(root, 0);
    return output.str();
}

std::string NetworkFetcher::fetch_url(const std::string& url) {
    CURL* curl;
    CURLcode res;
    std::string response;
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " 
                      << curl_easy_strerror(res) << std::endl;
        }
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    return response;
}

} 