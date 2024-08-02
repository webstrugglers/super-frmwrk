/**
 * @file
 * @brief Implements Router class for mapping http request to specified
 * controllers
 */

#include "router.hpp"
#include <cstdlib>
#include <filesystem>
#include <string>
#include "constants.hpp"
#include "http-status-codes.hpp"
#include "logger.hpp"
#include "path-and-type.hpp"
#include "response.hpp"

Router::Router()
    : routing_table(std::make_unique<std::unordered_map<
                        PathAndType,
                        std::function<void(const Request&, Response&)>>>()),
      mimes(std::make_unique<std::unordered_map<std::string, std::string>>(
          std::initializer_list<std::pair<const std::string, std::string>>{
              {".aac", "audio/aac"},
              {".abw", "application/x-abiword"},
              {".apng", "image/apng"},
              {".arc", "application/x-freearc"},
              {".avif", "image/avif"},
              {".avi", "video/x-msvideo"},
              {".azw", "application/vnd.amazon.ebook"},
              {".bin", "application/octet-stream"},
              {".bmp", "image/bmp"},
              {".bz", "application/x-bzip"},
              {".bz2", "application/x-bzip2"},
              {".cda", "application/x-cdf"},
              {".csh", "application/x-csh"},
              {".css", "text/css"},
              {".csv", "text/csv"},
              {".doc", "application/msword"},
              {".docx",
               "application/"
               "vnd.openxmlformats-officedocument.wordprocessingml.document"},
              {".eot", "application/vnd.ms-fontobject"},
              {".epub", "application/epub+zip"},
              {".gz", "application/gzip"},
              {".gif", "image/gif"},
              {".htm", "text/html"},
              {".html", "text/html"},
              {".ico", "image/vnd.microsoft.icon"},
              {".ics", "text/calendar"},
              {".jar", "application/java-archive"},
              {".jpeg", "image/jpeg"},
              {".jpg", "image/jpeg"},
              {".js", "text/javascript"},
              {".json", "application/json"},
              {".jsonld", "application/ld+json"},
              {".mid", "audio/midi"},
              {".midi", "audio/x-midi"},
              {".mjs", "text/javascript"},
              {".mp3", "audio/mpeg"},
              {".mp4", "video/mp4"},
              {".mpeg", "video/mpeg"},
              {".mpkg", "application/vnd.apple.installer+xml"},
              {".odp", "application/vnd.oasis.opendocument.presentation"},
              {".ods", "application/vnd.oasis.opendocument.spreadsheet"},
              {".odt", "application/vnd.oasis.opendocument.text"},
              {".oga", "audio/ogg"},
              {".ogv", "video/ogg"},
              {".ogx", "application/ogg"},
              {".opus", "audio/ogg"},
              {".otf", "font/otf"},
              {".png", "image/png"},
              {".pdf", "application/pdf"},
              {".php", "application/x-httpd-php"},
              {".ppt", "application/vnd.ms-powerpoint"},
              {".pptx",
               "application/"
               "vnd.openxmlformats-officedocument.presentationml.presentation"},
              {".rar", "application/vnd.rar"},
              {".rtf", "application/rtf"},
              {".sh", "application/x-sh"},
              {".svg", "image/svg+xml"},
              {".tar", "application/x-tar"},
              {".tif", "image/tiff"},
              {".tiff", "image/tiff"},
              {".ts", "video/mp2t"},
              {".ttf", "font/ttf"},
              {".txt", "text/plain"},
              {".vsd", "application/vnd.visio"},
              {".wav", "audio/wav"},
              {".weba", "audio/webm"},
              {".webm", "video/webm"},
              {".webp", "image/webp"},
              {".woff", "font/woff"},
              {".woff2", "font/woff2"},
              {".xhtml", "application/xhtml+xml"},
              {".xls", "application/vnd.ms-excel"},
              {".xlsx",
               "application/"
               "vnd.openxmlformats-officedocument.spreadsheetml.sheet"},
              {".xml", "application/xml"},
              {".xul", "application/vnd.mozilla.xul+xml"},
              {".zip", "application/zip"},
              {".3gp", "video/3gpp"},
              {".3g2", "video/3gpp2"},
              {".7z", "application/x-7z-compressed"}})) {}

void Router::route(
    const MethodType                                              method_type,
    const char*                                                   path,
    const std::function<void(const Request& req, Response& res)>& controller) {
    PathAndType pat = PathAndType(path, method_type);

    auto inserted = routing_table->insert(std::pair(pat, controller)).second;

    if (!inserted) {
        SafeLogger::log("You tried mapping Path (" + pat.path +
                        ") and method type (" + pat.method_type +
                        ") more than once");
        exit(EXIT_FAILURE);
    }
}

void Router::get(
    const char*                                                   path,
    const std::function<void(const Request& req, Response& res)>& controller) {
    route(HTTP_GET, path, controller);
}

void Router::post(
    const char*                                                   path,
    const std::function<void(const Request& req, Response& res)>& controller) {
    route(HTTP_POST, path, controller);
}

void Router::put(
    const char*                                                   path,
    const std::function<void(const Request& req, Response& res)>& controller) {
    route(HTTP_PUT, path, controller);
}

// TODO: handle errors
void Router::call(const PathAndType& pat, const Request& req, Response& res) {
    auto route_handle_it = this->routing_table->find(pat);
    if (route_handle_it != this->routing_table->end()) {
        route_handle_it->second(req, res);
        auto file = res.file();
        if (!file.empty()) {
            res.set("Content-Type", mimes->at(file.extension().string()));
            res.set("Content-Length",
                    std::to_string(std::filesystem::file_size(file)));
        }
    } else {
        if (!(pat.path.find("..") == std::string::npos)) {
            SafeLogger::log("Path traversal attempt");
            return;
        }
        // If the requested function is not found, we will check if the client
        // is trying to access a file that was created after the server started.
        std::filesystem::path local = this->static_root;
        local.concat(pat.path);
        SafeLogger::log(local);
        if (std::filesystem::exists(local)) {
            auto controller = [local](const Request& /*req*/, Response& res2) {
                res2.status(OK).attachment(local);
            };
            this->routing_table->emplace(pat, controller);
            controller(req, res);
        } else {
            res.status(NOT_FOUND).attachment("./public/notfound.html");
        }
    }
}

// TODO: dinamicko resolvovanje rute
void Router::serve_static(const std::filesystem::path& path) {
    std::filesystem::path abs_path;
    try {
        abs_path = std::filesystem::canonical(path);
    } catch (std::filesystem::filesystem_error& err) {
        SafeLogger::log("Error converting to absolute path");
        SafeLogger::log(err.what());
        exit(EXIT_FAILURE);
    }

    if (!std::filesystem::is_directory(abs_path)) {
        SafeLogger::log("Router::serve_static Must provide directory!");
        exit(EXIT_FAILURE);
    }

    // save this path for dynamic routing in the future
    this->static_root = abs_path;

    {
        // if path/index.html exists map GET / to it
        auto index_path = abs_path;
        index_path.concat("/index.html");
        if (std::filesystem::exists(index_path)) {
            auto index_func = [index_path](const Request& /*req*/,
                                           Response& res) {
                res.status(OK).attachment(index_path);
            };
            this->routing_table->emplace(PathAndType("/", HTTP_GET),
                                         index_func);
        }
    }
    for (const auto& entry :
         std::filesystem::recursive_directory_iterator(abs_path)) {
        if (entry.is_regular_file()) {
            std::string path_str  = abs_path.string();
            std::string entry_str = entry.path().string();
            auto        func = [entry](const Request& /*req*/, Response res) {
                res.status(OK).attachment(entry);
            };

            auto uri = entry_str.substr(path_str.length());
            SafeLogger::log(uri);
            this->routing_table->emplace(PathAndType(uri, HTTP_GET), func);
        }
    }
}
