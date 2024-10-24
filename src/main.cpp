#include <iostream>
#include <fstream>
#include <string>
#include <json/json.h>
#include <cstdlib>

std::string escapeString(const std::string& str) {
    std::string escaped;
    for (char c : str) {
        switch (c) {
            case '"': escaped += "\\\""; break;
            case '\\': escaped += "\\\\"; break;
            case '\n': escaped += "\\n"; break;
            case '\r': escaped += "\\r"; break;
            case '\t': escaped += "\\t"; break;
            default: escaped += c; break;
        }
    }
    return escaped;
}

void createProjectStructure() {
    std::ofstream htmlFile("index.html");
    htmlFile << "<!DOCTYPE html><html><head><link rel='stylesheet' href='styles.css'></head><body><script src='main.js'></script></body></html>";
    htmlFile.close();

    std::ofstream cssFile("styles.css");
    cssFile << "body { background-color: #f0f0f0; }";
    cssFile.close();

    std::ofstream jsFile("main.js");
    jsFile << "console.log('Hello, Neox!');";
    jsFile.close();

    std::ofstream jsonFile("neox.json");
    jsonFile << R"({
    "appName": "My Neox App",
    "identifier": "com.example.neoxapp",
    "html": "index.html",
    "js": "main.js",
    "css": "styles.css"
})";
    jsonFile.close();

    std::cout << "Project initialized." << std::endl;
}

void buildProject() {
    std::ifstream jsonFile("neox.json");
    if (!jsonFile.is_open()) {
        std::cerr << "Could not open neox.json" << std::endl;
        return;
    }

    Json::Value config;
    jsonFile >> config;

    std::string htmlFile = config["html"].asString();
    std::string jsFile = config["js"].asString();
    std::string cssFile = config["css"].asString();
    std::string appName = config["appName"].asString();
    std::string identifier = config["identifier"].asString();

    // Validate required fields
    if (appName.empty() || identifier.empty()) {
        std::cerr << "Error: appName and identifier are required in neox.json" << std::endl;
        return;
    }

    std::ifstream htmlStream(htmlFile);
    std::ifstream jsStream(jsFile);
    std::ifstream cssStream(cssFile);

    if (!htmlStream.is_open() || !jsStream.is_open() || !cssStream.is_open()) {
        std::cerr << "Error reading project files." << std::endl;
        return;
    }

    std::string htmlContent((std::istreambuf_iterator<char>(htmlStream)), std::istreambuf_iterator<char>());
    std::string jsContent((std::istreambuf_iterator<char>(jsStream)), std::istreambuf_iterator<char>());
    std::string cssContent((std::istreambuf_iterator<char>(cssStream)), std::istreambuf_iterator<char>());

    htmlContent = escapeString(htmlContent);
    jsContent = escapeString(jsContent);
    cssContent = escapeString(cssContent);

    std::ofstream cppFile("generated.cpp");
    cppFile << R"(
#include <webkit2/webkit2.h>
#include <gtk/gtk.h>
#include <string>

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *webview;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), ")" << appName << R"(");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    webview = webkit_web_view_new();
    gtk_container_add(GTK_CONTAINER(window), webview);

    const char *html = ")" << htmlContent << R"(";
    const char *js = ")" << jsContent << R"(";
    const char *css = ")" << cssContent << R"(";
    std::string combined = "<style>" + std::string(css) + "</style><script>" + std::string(js) + "</script>" + std::string(html);

    webkit_web_view_load_html(WEBKIT_WEB_VIEW(webview), combined.c_str(), nullptr);

    gtk_widget_show_all(window);
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new(")" << identifier << R"(", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
)";
    cppFile.close();

    system("g++ generated.cpp -o neox-app `pkg-config --cflags --libs webkit2gtk-4.0 gtk+-3.0`");

    std::cout << "Build complete." << std::endl;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: neox <init|build>" << std::endl;
        return 1;
    }

    std::string command = argv[1];
    if (command == "init") {
        createProjectStructure();
    } else if (command == "build") {
        buildProject();
    } else {
        std::cerr << "Invalid command." << std::endl;
        return 1;
    }

    return 0;
}
