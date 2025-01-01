//
// Copyright (c) 2025, SpargatTeam
// Wrote by Comical
//
#ifndef FLUXIUM_H
#define FLUXIUM_H

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <regex>

namespace fluxium {

class Element {
public:
    enum Type {
        DIV, SPAN, P, A, IMG, 
        H1, H2, H3, H4, H5, H6,
        UNKNOWN
    };
    Element(Type type, const std::string& content = "");
    void set_attribute(const std::string& key, const std::string& value);
    std::string get_attribute(const std::string& key) const;
    void add_child(std::shared_ptr<Element> child);
    Type type;
    std::string content;
    std::map<std::string, std::string> attributes;
    std::vector<std::shared_ptr<Element>> children;
};

class Style {
public:
    void add_rule(const std::string& selector, const std::string& property, const std::string& value);
    std::string get_style(const std::string& selector, const std::string& property) const;

private:
    std::map<std::string, std::map<std::string, std::string>> styles;
};

class HTMLParser {
public:
    static std::shared_ptr<Element> parse(const std::string& html);

private:
    static std::shared_ptr<Element> parse_tag(const std::string& tag);
    static Element::Type get_element_type(const std::string& tag_name);
};

class CSSParser {
public:
    static Style parse(const std::string& css);
};

class Script {
public:
    static std::string evaluate(const std::string& script);
};

class Renderer {
public:
    static std::string render(std::shared_ptr<Element> root, const Style& style);
};

class NetworkFetcher {
public:
    static std::string fetch_url(const std::string& url);
};

} 

#endif // FLUXIUM_H
