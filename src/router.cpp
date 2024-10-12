/**
 * @file
 * @brief Implements Router class for mapping http request to specified
 * controllers
 */

#include "router.hpp"
#include <string>
#include "logger.hpp"

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
              {".7z", "application/x-7z-compressed"}})),
      not_found_page("./public/notfound.html") {}

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

// bool Router::matches(const std::string& req_path,
//                      const std::string& route_path) {
//     if (req_path == route_path) {
//         return true;
//     }
//
//     // logic for comparing dynamic types
//     // return match_dynamic_path(req_path, route_path); // new PathAndType
//     // method, not implemented
// }
// void Router::extract_path_params(
//     const std::string&                            req_path,
//     const std::string&                            route_path,
//     std::unordered_map<std::string, std::string>& path_params) {
//     // yet to be implemented
// }
void Router::call(const Request& req, Response& res) {
    for (const auto& [path_and_type, handler] : *routing_table) {
        // if (matches(req.path, path_and_type.path)){
        //     //extract_path_params method call

        //     handle_route(handler,req, res);
        //     return;
        // }
    }
    potential_static(req, res);
}

void Router::serve_static(const std::filesystem::path& p) {
    set_static_root(p);
    map_root_to_index();

    for (const auto& entry :
         std::filesystem::recursive_directory_iterator(this->static_root)) {
        if (entry.is_regular_file()) {
            std::string path_str  = this->static_root.string();
            std::string entry_str = entry.path().string();
            auto        func = [entry](const Request& /*req*/, Response& res) {
                res.status(OK).attachment(entry.path());
            };

            auto uri = entry_str.substr(path_str.length());
            this->routing_table->emplace(PathAndType(uri, HTTP_GET), func);
        }
    }
}

void Router::not_found(const std::filesystem::path& p) {
    if (!std::filesystem::exists(p)) {
        return;
    }
    if (p.extension().compare(".html") != 0) {
        return;
    }

    this->not_found_page = p;
}

void Router::handle_route(
    std::function<void(const Request&, Response&)>& handler,
    const Request&                                  req,
    Response&                                       res) {
    handler(req, res);

    auto file = res.file();
    if (file.empty()) {
        return;
    }

    if (std::filesystem::exists(file)) {
        res.set("Content-Type", mimes->at(file.extension().string()));
        res.set("Content-Length",
                std::to_string(std::filesystem::file_size(file)));
    } else {
        res_not_found(req.path_and_type, res);
    }
}

void Router::potential_static(const Request& req, Response& res) {
    // If the requested function is not found, we will check if the client
    // is trying to access a file that was created after the server started.
    auto pat = req.path_and_type;
    if (!(pat.path.find("..") == std::string::npos)) {
        SafeLogger::log("Path traversal attempt");
        res.status(FORBIDDEN);
        return;
    }

    std::filesystem::path local = this->static_root;
    local.concat(pat.path);
    try {
        local = std::filesystem::weakly_canonical(local);
    } catch (std::filesystem::filesystem_error& err) {
        SafeLogger::log(err.what());
        res.status(FORBIDDEN);
        return;
    }

    if (is_req_file_legit(local)) {
        auto controller = [local](const Request& /*req*/, Response& ress) {
            ress.status(OK).attachment(local);
        };
        this->routing_table->emplace(pat, controller);
        controller(req, res);
    } else {
        res_not_found(pat, res);
    }
}

bool Router::is_req_file_legit(const std::filesystem::path& p) {
    auto err = p.string().find(this->static_root.string());
    if (err == std::string::npos) {
        return false;
    }
    return std::filesystem::exists(p) && std::filesystem::is_regular_file(p);
}

void Router::res_not_found(const PathAndType& pat, Response& res) {
    std::filesystem::path p = std::filesystem::path(pat.path);
    if (p.extension().compare(".html") == 0 || !p.has_extension()) {
        res.status(NOT_FOUND).attachment("./public/notfound.html");
    } else {
        res.status(NOT_FOUND);
    }
}

void Router::map_root_to_index() {
    auto index_path = this->static_root;
    index_path.concat("/index.html");
    if (std::filesystem::exists(index_path)) {
        auto index_func = [index_path](const Request& /*req*/, Response& res) {
            res.status(OK).attachment(index_path);
        };
        this->routing_table->emplace(PathAndType("/", HTTP_GET), index_func);
    }
}

void Router::set_static_root(const std::filesystem::path& p) {
    std::filesystem::path abs_path;
    try {
        abs_path = std::filesystem::canonical(p);
    } catch (std::filesystem::filesystem_error& err) {
        SafeLogger::log("Error converting to absolute path");
        SafeLogger::log(err.what());
        exit(EXIT_FAILURE);
    }

    if (!std::filesystem::is_directory(abs_path)) {
        SafeLogger::log("Router::serve_static Must provide directory!");
        exit(EXIT_FAILURE);
    }
    this->static_root = abs_path;
}

void Router::set_date_header(Response& res) {
    std::time_t time = std::time({});

    // Use a fixed-size buffer to format the date and time
    std::array<char, std::size("Sun, 06 Nov 1994 08:49:37 GMT")> buffer{};

    [[maybe_unused]]
    auto x = std::strftime(std::data(buffer), std::size(buffer),
                           "%a, %d %b %Y %H:%M:%S GMT", std::gmtime(&time));

    res.set("date", buffer.data());
}
