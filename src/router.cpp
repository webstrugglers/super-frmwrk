/**
 * @file
 * @brief Implements Router class for mapping http request to specified
 * controllers
 */

#include "router.hpp"
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

void Router::call(const PathAndType& pat, const Request& req, Response& res) {
    auto func = this->routing_table->find(pat);
    if (func != this->routing_table->end()) {
        func->second(req, res);
        auto file = res.file();
        if (!file.empty()) {
            res.set("Content-Type", mimes->at(file.extension().string()));
            res.set("Content-Length",
                    std::to_string(std::filesystem::file_size(file)));
        }
    } else {
        res.attachment("./public/notfound.html");
    }
}

// TODO: dinamicko resolvovanje rute
void Router::serve_static(const std::filesystem::path& path) {
    if (!std::filesystem::is_directory(path)) {
        SafeLogger::log("Provided path must be directory");
        exit(EXIT_FAILURE);
    }

    auto path_str = path.string();
    if (path_str[path_str.length() - 1] == '/') {
        path_str = path_str.substr(0, path_str.length() - 1);
    }

    const std::string index = "index.html";
    for (const auto& entry :
         std::filesystem::recursive_directory_iterator(path_str)) {
        if (entry.is_regular_file()) {
            const auto& entry_path = entry.path();
            auto        entry_str  = entry_path.string();

            auto func = [entry_path](const Request& /*req*/, Response& res) {
                res.status(OK).attachment(entry_path);
            };

            auto shorter_path =
                std::filesystem::path(entry_str.substr(path_str.length()));
            auto shorter_str = shorter_path.string();

            PathAndType pat(shorter_str.substr(shorter_str.find('/')),
                            HTTP_GET);

            // map GET / to index.html
            if (entry_str.compare(entry_str.length() - index.length(),
                                  index.length(), index) == 0) {
                this->routing_table->emplace(PathAndType("/", HTTP_GET), func);
            }

            // insert path into the routing table
            auto ret = this->routing_table->emplace(pat, func);
            if (!ret.second) {
                SafeLogger::log("You tried mapping Path (" + pat.path +
                                ") and method type (" + pat.method_type +
                                ") more than once");
                exit(EXIT_FAILURE);
            }
        }
    }
}
