
#ifndef HTTP_STATUS_CODES_HPP
#define HTTP_STATUS_CODES_HPP

/**
 * @file This file implements HTTP status enums, codes and corresponding
 * messages
 * @brief This file implements HTTP status enums, codes and corresponding
 * messages
 */
#include <cstdint>
#define HTTP_STATUS_CODES                                                      \
    X(CONTINUE, "CONTINUE", 100)                                               \
    X(SWITCHING_PROTOCOLS, "SWITCHING PROTOCOLS", 101)                         \
    X(PROCESSING, "PROCESSING", 102)                                           \
    X(EARLY_HINTS, "EARLY HINTS", 103)                                         \
    X(OK, "OK", 200)                                                           \
    X(CREATED, "CREATED", 201)                                                 \
    X(ACCEPTED, "ACCEPTED", 202)                                               \
    X(NON_AUTHORITATIVE_INFORMATION, "NON AUTHORITATIVE INFORMATION", 203)     \
    X(NO_CONTENT, "NO CONTENT", 204)                                           \
    X(RESET_CONTENT, "RESET CONTENT", 205)                                     \
    X(PARTIAL_CONTENT, "PARTIAL CONTENT", 206)                                 \
    X(MULTI_STATUS, "MULTI STATUS", 207)                                       \
    X(ALREADY_REPORTED, "ALREADY REPORTED", 208)                               \
    X(IM_USED, "IM USED", 226)                                                 \
    X(MULTIPLE_CHOICES, "MULTIPLE CHOICES", 300)                               \
    X(MOVED_PERMANENTLY, "MOVED PERMANENTLY", 301)                             \
    X(FOUND, "FOUND", 302)                                                     \
    X(SEE_OTHER, "SEE OTHER", 303)                                             \
    X(NOT_MODIFIED, "NOT MODIFIED", 304)                                       \
    X(TEMPORARY_REDIRECT, "TEMPORARY REDIRECT", 307)                           \
    X(PERMANENT_REDIRECT, "PERMANENT REDIRECT", 308)                           \
    X(BAD_REQUEST, "BAD REQUEST", 400)                                         \
    X(UNAUTHORIZED, "UNAUTHORIZED", 401)                                       \
    X(PAYMENT_REQUIRED, "PAYMENT REQUIRED", 402)                               \
    X(FORBIDDEN, "FORBIDDEN", 403)                                             \
    X(NOT_FOUND, "NOT FOUND", 404)                                             \
    X(METHOD_NOT_ALLOWED, "METHOD NOT ALLOWED", 405)                           \
    X(NOT_ACCEPTABLE, "NOT ACCEPTABLE", 406)                                   \
    X(PROXY_AUTHENTICATION_REQUIRED, "PROXY AUTHENTICATION REQUIRED", 407)     \
    X(REQUEST_TIMEOUT, "REQUEST TIMEOUT", 408)                                 \
    X(CONFLICT, "CONFLICT", 409)                                               \
    X(GONE, "GONE", 410)                                                       \
    X(LENGTH_REQUIRED, "LENGTH REQUIRED", 411)                                 \
    X(PRECONDITION_FAILED, "PRECONDITION FAILED", 412)                         \
    X(PAYLOAD_TOO_LARGE, "PAYLOAD TOO LARGE", 413)                             \
    X(URI_TOO_LONG, "URI TOO LONG", 414)                                       \
    X(UNSUPPORTED_MEDIA_TYPE, "UNSUPPORTED MEDIA TYPE", 415)                   \
    X(RANGE_NOT_SATISFIABLE, "RANGE NOT SATISFIABLE", 416)                     \
    X(EXPECTATION_FAILED, "EXPECTATION FAILED", 417)                           \
    X(IM_A_TEAPOT, "IM A TEAPOT", 418)                                         \
    X(MISDIRECTED_REQUEST, "MISDIRECTED REQUEST", 421)                         \
    X(UNPROCESSABLE_CONTENT, "UNPROCESSABLE CONTENT", 422)                     \
    X(LOCKED, "LOCKED", 423)                                                   \
    X(FAILED_DEPENDENCY, "FAILED DEPENDENCY", 424)                             \
    X(TOO_EARLY, "TOO EARLY", 425)                                             \
    X(UPGRADE_REQUIRED, "UPGRADE REQUIRED", 426)                               \
    X(PRECONDITION_REQUIRED, "PRECONDITION REQUIRED", 428)                     \
    X(TOO_MANY_REQUESTS, "TOO MANY REQUESTS", 429)                             \
    X(REQUEST_HEADER_FIELDS_TOO_LARGE, "REQUEST HEADER FIELDS TOO LARGE", 431) \
    X(UNAVAILABLE_FOR_LEGAL_REASON, "UNAVAILABLE FOR LEGAL REASON", 451)       \
    X(INTERNAL_SERVER_ERROR, "INTERNAL SERVER ERROR", 500)                     \
    X(NOT_IMPLEMENTED, "NOT IMPLEMENTED", 501)                                 \
    X(BAD_GATEWAY, "BAD GATEWAY", 502)                                         \
    X(SERVICE_UNAVAILABLE, "SERVICE UNAVAILABLE", 503)                         \
    X(GATEWAY_TIMEOUT, "GATEWAY TIMEOUT", 504)                                 \
    X(HTTP_VERSION_NOT_SUPPORTED, "HTTP VERSION NOT SUPPORTED", 505)           \
    X(VARIANT_ALSO_NEGOTIATES, "VARIANT ALSO NEGOTIATES", 506)                 \
    X(INSUFFICIENT_STORAGE, "INSUFFICIENT STORAGE", 507)                       \
    X(LOOP_DETECTED, "LOOP DETECTED", 508)                                     \
    X(NOT_EXTENDED, "NOT EXTENDED", 510)                                       \
    X(NETWORK_AUTHENTICATION_REQUIRED, "NETWORK AUTHENTICATION REQUIRED", 511)

#define X(HttpStatusMessage, HttpStatusString, HttpStatusCode) \
    HttpStatusMessage,
/// Enum for status codes
enum HttpStatus : std::uint8_t { HTTP_STATUS_CODES };
#undef X

#define X(HttpStatusMessage, HttpStatusString, HttpStatusCode) HttpStatusString,
/// Array of status message strings
const char* const http_status_message[] = {HTTP_STATUS_CODES};
#undef X

#define X(HttpStatusMessage, HttpStatusString, HttpStatusCode) HttpStatusCode,
/// Array of status codes
inline const int http_status_code[]{HTTP_STATUS_CODES};
#undef X

#endif  // HTTP_STATUS_CODES_HPP
