
#ifndef HTTP_STATUS_CODES_HPP
#define HTTP_STATUS_CODES_HPP

/**
 * @file This file implements HTTP status enums, codes and corresponding
 * messages
 * @brief This file implements HTTP status enums, codes and corresponding
 * messages
 */
#define HTTP_STATUS_CODES                                                      \
    X(CONTINUE, "CONTINUE", 100)                                               \
    X(SWITCHING_PROTOCOLS, "SWITCHING_PROTOCOLS", 101)                         \
    X(PROCESSING, "PROCESSING", 102)                                           \
    X(EARLY_HINTS, "EARLY_HINTS", 103)                                         \
    X(OK, "OK", 200)                                                           \
    X(CREATED, "CREATED", 201)                                                 \
    X(ACCEPTED, "ACCEPTED", 202)                                               \
    X(NON_AUTHORITATIVE_INFORMATION, "NON_AUTHORITATIVE_INFORMATION", 203)     \
    X(NO_CONTENT, "NO_CONTENT", 204)                                           \
    X(RESET_CONTENT, "RESET_CONTENT", 205)                                     \
    X(PARTIAL_CONTENT, "PARTIAL_CONTENT", 206)                                 \
    X(MULTI_STATUS, "MULTI_STATUS", 207)                                       \
    X(ALREADY_REPORTED, "ALREADY_REPORTED", 208)                               \
    X(IM_USED, "IM_USED", 226)                                                 \
    X(MULTIPLE_CHOICES, "MULTIPLE_CHOICES", 300)                               \
    X(MOVED_PERMANENTLY, "MOVED_PERMANENTLY", 301)                             \
    X(FOUND, "FOUND", 302)                                                     \
    X(SEE_OTHER, "SEE_OTHER", 303)                                             \
    X(NOT_MODIFIED, "NOT_MODIFIED", 304)                                       \
    X(TEMPORARY_REDIRECT, "TEMPORARY_REDIRECT", 307)                           \
    X(PERMANENT_REDIRECT, "PERMANENT_REDIRECT", 308)                           \
    X(BAD_REQUEST, "BAD_REQUEST", 400)                                         \
    X(UNAUTHORIZED, "UNAUTHORIZED", 401)                                       \
    X(PAYMENT_REQUIRED, "PAYMENT_REQUIRED", 402)                               \
    X(FORBIDDEN, "FORBIDDEN", 403)                                             \
    X(NOT_FOUND, "NOT_FOUND", 404)                                             \
    X(METHOD_NOT_ALLOWED, "METHOD_NOT_ALLOWED", 405)                           \
    X(NOT_ACCEPTABLE, "NOT_ACCEPTABLE", 406)                                   \
    X(PROXY_AUTHENTICATION_REQUIRED, "PROXY_AUTHENTICATION_REQUIRED", 407)     \
    X(REQUEST_TIMEOUT, "REQUEST_TIMEOUT", 408)                                 \
    X(CONFLICT, "CONFLICT", 409)                                               \
    X(GONE, "GONE", 410)                                                       \
    X(LENGTH_REQUIRED, "LENGTH_REQUIRED", 411)                                 \
    X(PRECONDITION_FAILED, "PRECONDITION_FAILED", 412)                         \
    X(PAYLOAD_TOO_LARGE, "PAYLOAD_TOO_LARGE", 413)                             \
    X(URI_TOO_LONG, "URI_TOO_LONG", 414)                                       \
    X(UNSUPPORTED_MEDIA_TYPE, "UNSUPPORTED_MEDIA_TYPE", 415)                   \
    X(RANGE_NOT_SATISFIABLE, "RANGE_NOT_SATISFIABLE", 416)                     \
    X(EXPECTATION_FAILED, "EXPECTATION_FAILED", 417)                           \
    X(IM_A_TEAPOT, "IM_A_TEAPOT", 418)                                         \
    X(MISDIRECTED_REQUEST, "MISDIRECTED_REQUEST", 421)                         \
    X(UNPROCESSABLE_CONTENT, "UNPROCESSABLE_CONTENT", 422)                     \
    X(LOCKED, "LOCKED", 423)                                                   \
    X(FAILED_DEPENDENCY, "FAILED_DEPENDENCY", 424)                             \
    X(TOO_EARLY, "TOO_EARLY", 425)                                             \
    X(UPGRADE_REQUIRED, "UPGRADE_REQUIRED", 426)                               \
    X(PRECONDITION_REQUIRED, "PRECONDITION_REQUIRED", 428)                     \
    X(TOO_MANY_REQUESTS, "TOO_MANY_REQUESTS", 429)                             \
    X(REQUEST_HEADER_FIELDS_TOO_LARGE, "REQUEST_HEADER_FIELDS_TOO_LARGE", 431) \
    X(UNAVAILABLE_FOR_LEGAL_REASON, "UNAVAILABLE_FOR_LEGAL_REASON", 451)       \
    X(INTERNAL_SERVER_ERROR, "INTERNAL_SERVER_ERROR", 500)                     \
    X(NOT_IMPLEMENTED, "NOT_IMPLEMENTED", 501)                                 \
    X(BAD_GATEWAY, "BAD_GATEWAY", 502)                                         \
    X(SERVICE_UNAVAILABLE, "SERVICE_UNAVAILABLE", 503)                         \
    X(GATEWAY_TIMEOUT, "GATEWAY_TIMEOUT", 504)                                 \
    X(HTTP_VERSION_NOT_SUPPORTED, "HTTP_VERSION_NOT_SUPPORTED", 505)           \
    X(VARIANT_ALSO_NEGOTIATES, "VARIANT_ALSO_NEGOTIATES", 506)                 \
    X(INSUFFICIENT_STORAGE, "INSUFFICIENT_STORAGE", 507)                       \
    X(LOOP_DETECTED, "LOOP_DETECTED", 508)                                     \
    X(NOT_EXTENDED, "NOT_EXTENDED", 510)                                       \
    X(NETWORK_AUTHENTICATION_REQUIRED, "NETWORK_AUTHENTICATION_REQUIRED", 511)

#define X(HttpStatusMessage, HttpStatusString, HttpStatusCode) \
    HttpStatusMessage,
/// Enum for status codes
enum HttpStatus : int { HTTP_STATUS_CODES };
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
