/**
 * @file
 * @brief Implements Router class for mapping http request to specified
 * handlers
 */

#include "router.hpp"
#include <brotli/encode.h>
#include <brotli/types.h>
#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include "constants.hpp"
#include "logger.hpp"

Router::Router() noexcept
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

void Router::route(const MethodType method_type,
                   const char*      path,
                   const std::function<void(const Request& req, Response& res)>&
                       handler) noexcept {
    const PathAndType pat = PathAndType(path, method_type);

    const auto inserted = routing_table->insert(std::pair(pat, handler)).second;

    if (!inserted) {
        SafeLogger::log("You tried mapping Path (" + pat.path +
                        ") and method type (" + pat.method_type +
                        ") more than once");
        exit(EXIT_FAILURE);
    }
}

void Router::get(const char* path,
                 const std::function<void(const Request& req, Response& res)>&
                     handler) noexcept {
    route(HTTP_GET, path, handler);
}

void Router::post(const char* path,
                  const std::function<void(const Request& req, Response& res)>&
                      handler) noexcept {
    route(HTTP_POST, path, handler);
}

void Router::put(const char* path,
                 const std::function<void(const Request& req, Response& res)>&
                     handler) noexcept {
    route(HTTP_PUT, path, handler);
}

// TODO: handle errors
void Router::call(const Request& req, Response& res) noexcept {
    const auto route_handler_it = this->routing_table->find(req.path_and_type);
    if (route_handler_it != routing_table->end()) {
        handle_route(route_handler_it->second, req, res);
    } else {
        potential_static(req, res);
    }

    compress_response(req, res);

    set_date_header(res);
}

void Router::serve_static(const std::filesystem::path& p) noexcept {
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

void Router::not_found(const std::filesystem::path& p) noexcept {
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
    Response&                                       res) noexcept {
    handler(req, res);

    const auto file = res.file();
    if (file.empty()) {
        return;
    }

    if (std::filesystem::exists(file)) {
        const auto it = mimes->find(file.extension().string());
        if (it != mimes->end()) {
            res.set("Content-Type", it->second);
        } else {
            res.set("Content-Type", "text/plain");
        }
        res.set("Content-Length",
                std::to_string(std::filesystem::file_size(file)));
    } else {
        res_not_found(res);
    }
}

void Router::potential_static(const Request& req, Response& res) noexcept {
    // If the appropriate handler is not found, we will check if the client
    // is trying to access a file that was created after the server started.
    const auto pat = req.path_and_type;
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
        auto handler = [local, this](const Request& /*req*/, Response& ress) {
            ress.status(OK).attachment(local);
            const auto it = mimes->find(local.extension().string());
            if (it != mimes->end()) {
                ress.set("Content-Type", it->second);
            } else {
                ress.set("Content-Type", "text/plain");
            }
            ress.set("Content-Length",
                     std::to_string(std::filesystem::file_size(local)));
        };
        this->routing_table->emplace(pat, handler);
        handler(req, res);
    } else {
        res_not_found(res);
    }
}

bool Router::is_req_file_legit(const std::filesystem::path& p) const noexcept {
    const auto err = p.string().find(this->static_root.string());
    if (err == std::string::npos) {
        return false;
    }
    return std::filesystem::exists(p) && std::filesystem::is_regular_file(p);
}

void Router::res_not_found(Response& res) const noexcept {
    if (!not_found_page.empty()) {
        res.status(NOT_FOUND).attachment(this->not_found_page);
    } else {
        res.status(NOT_FOUND).attachment(PAGE_NOT_FOUND_HTML);
    }
    res.set("Content-Type", "text/html");
}

void Router::map_root_to_index() noexcept {
    auto index_path = this->static_root;
    index_path.concat("/index.html");
    if (std::filesystem::exists(index_path)) {
        auto index_func = [index_path](const Request& /*req*/, Response& res) {
            res.status(OK).attachment(index_path);
        };
        this->routing_table->emplace(PathAndType("/", HTTP_GET), index_func);
    }
}

void Router::set_static_root(const std::filesystem::path& p) noexcept {
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

void Router::set_date_header(Response& res) const noexcept {
    const std::time_t time = std::time({});

    std::array<char, std::size("Sun, 06 Nov 1994 08:49:37 GMT")> buffer{};

    std::strftime(std::data(buffer), std::size(buffer),
                  "%a, %d %b %Y %H:%M:%S GMT", std::gmtime(&time));

    res.set("date", buffer.data());
}

void Router::compress_response(const Request& req,
                               Response&      res) const noexcept {
    const auto accept_encoding_header = req.headers.find("Accept-Encoding");

    if (accept_encoding_header == req.headers.end()) return;

    if (accept_encoding_header->second.find("br") != std::string::npos) {
        if (std::filesystem::exists(res.file())) {
            compress_file_br(res);
        } else if (res.get_data().size() != 0) {
            compress_data_br(res);
        }

        // set empty attachment
        // this will signal dispatcher that there is no file to serve
        // This is wanted behaviour because we already loaded the compressed
        // file into the response data and set correct headers
        res.attachment({});
    }
}

void Router::compress_data_br(Response& res) const noexcept {
    const auto& input_data  = res.get_data();
    const auto  input_size  = input_data.size();
    size_t      output_size = BrotliEncoderMaxCompressedSize(input_size);

    std::string output(output_size, 0);

    BROTLI_BOOL success = BrotliEncoderCompress(
        BROTLI_DEFAULT_QUALITY, BROTLI_DEFAULT_WINDOW, BROTLI_DEFAULT_MODE,
        input_size, reinterpret_cast<const uint8_t*>(input_data.data()),
        &output_size, reinterpret_cast<uint8_t*>(output.data()));

    output.resize(output_size);
    if (success == BROTLI_TRUE) {
        res.send(output);
        res.set("content-encoding", "br");
    }
}

void Router::compress_file_br(Response& res) const noexcept {
    const auto       file      = res.file();
    const size_t     file_size = std::filesystem::file_size(file);
    constexpr size_t MEMORY_LIMIT =
        static_cast<long>(10 * 1024) * 1024;  // 10 MB

    if (file_size <= MEMORY_LIMIT) {
        // Use one-shot compression for small files
        oneshot_compress_file_br(res);
    } else {
        // Use streaming compression for large files
        streaming_compress_file_br(res);
    }
}

void Router::oneshot_compress_file_br(Response& res) const noexcept {
    const auto file = res.file();

    // load file into memory
    std::ifstream file_stream(file, std::ios::binary);
    if (!file_stream) {
        return;
    }
    const std::string input_data((std::istreambuf_iterator<char>(file_stream)),
                                 std::istreambuf_iterator<char>());

    const size_t input_size  = input_data.size();
    size_t       output_size = BrotliEncoderMaxCompressedSize(input_size);

    // Compress the data
    std::string output(output_size, 0);
    BROTLI_BOOL success = BrotliEncoderCompress(
        BROTLI_DEFAULT_QUALITY, BROTLI_DEFAULT_WINDOW, BROTLI_DEFAULT_MODE,
        input_size, reinterpret_cast<const uint8_t*>(input_data.data()),
        &output_size, reinterpret_cast<uint8_t*>(output.data()));

    if (success == BROTLI_TRUE) {
        output.resize(output_size);
        res.send(output);
        res.set("content-encoding", "br");
    }
}

void Router::streaming_compress_file_br(Response& res) const noexcept {
    const auto file = res.file();

    std::ifstream file_stream(file, std::ios::binary);
    if (!file_stream) {
        return;
    }

    auto* enc = BrotliEncoderCreateInstance(nullptr, nullptr, nullptr);
    if (enc == nullptr) {
        return;
    }

    BrotliEncoderSetParameter(enc, BROTLI_PARAM_QUALITY,
                              BROTLI_DEFAULT_QUALITY);
    BrotliEncoderSetParameter(enc, BROTLI_PARAM_LGWIN, BROTLI_DEFAULT_WINDOW);

    std::array<uint8_t, 4096> input_buffer;
    std::array<uint8_t, 4096> output_buffer;
    std::string               compressed_data;
    size_t                    total_read = 0;

    while (file_stream) {
        // Read data into the input buffer
        file_stream.read(reinterpret_cast<char*>(input_buffer.data()), 4096);
        const size_t bytes_read = file_stream.gcount();
        total_read += bytes_read;

        if (bytes_read == 0) {
            break;
        }

        // Compress the input data
        const uint8_t* next_in  = input_buffer.data();
        size_t         avail_in = bytes_read;

        while (avail_in > 0) {
            uint8_t* next_out  = output_buffer.data();
            size_t   avail_out = 4096;

            BrotliEncoderOperation op = BROTLI_OPERATION_PROCESS;
            if (file_stream.eof()) {
                op = BROTLI_OPERATION_FINISH;
            }

            if (BrotliEncoderCompressStream(enc, op, &avail_in, &next_in,
                                            &avail_out, &next_out,
                                            nullptr) == 0) {
                BrotliEncoderDestroyInstance(enc);
                return;
            }

            // Append the compressed data to the output string
            compressed_data.append(
                reinterpret_cast<char*>(output_buffer.data()),
                4096 - avail_out);
        }
    }

    // Finalize compression
    while (BrotliEncoderHasMoreOutput(enc) != 0) {
        uint8_t* next_out  = output_buffer.data();
        size_t   avail_out = 4096;

        BrotliEncoderCompressStream(enc, BROTLI_OPERATION_FINISH, nullptr,
                                    nullptr, &avail_out, &next_out, nullptr);

        compressed_data.append(reinterpret_cast<char*>(output_buffer.data()),
                               4096 - avail_out);
    }

    BrotliEncoderDestroyInstance(enc);
    res.send(compressed_data);
    res.set("content-encoding", "br");
}
