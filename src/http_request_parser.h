#ifndef __HTTP_REQUEST_PARSER_H__
#define __HTTP_REQUEST_PARSER_H__

namespace TigerHttpd
{

struct HttpRequest;

class HttpRequestParser
{
public:
    enum ResultType { GOOD, BAD, INDETERMINATE };

    HttpRequestParser();
    ~HttpRequestParser();

    ResultType Parser(HttpRequest& request, const char* buf, const int len);

    /// Reset to initial parser state.
    void Reset();

private:
  /// Handle the next character of input.
  ResultType Consume(HttpRequest& req, char input);

  /// Check if a byte is an HTTP character.
  static bool IsChar(int c);

  /// Check if a byte is an HTTP control character.
  static bool IsCtl(int c);

  /// Check if a byte is defined as an HTTP tspecial character.
  static bool IsTspecial(int c);

  /// Check if a byte is a digit.
  static bool IsDigit(int c);

  /// The current state of the parser.
  enum State
  {
    Method_Start,
    Method,
    Uri,
    Http_Version_H,
    Http_Version_T_1,
    Http_Version_T_2,
    Http_Version_P,
    Http_Version_Slash,
    Http_Version_Major_Start,
    Http_Version_Major,
    Http_Version_Minor_Start,
    Http_Version_Minor,
    Expecting_Newline_1,
    Header_Line_Start,
    Header_Lws,
    Header_Name,
    Space_Before_Header_Value,
    Header_Value,
    Expecting_Newline_2,
    Expecting_Newline_3
  } state_;
};

} // namespace TigerHttpd

#endif // __HTTP_REQUEST_PARSER_H__

