
#include "http_request_parser.h"

#include "http_request.h"

namespace TigerHttpd
{

HttpRequestParser::HttpRequestParser()
    : state_(Method_Start)
{
}

HttpRequestParser::~HttpRequestParser()
{
}

HttpRequestParser::ResultType HttpRequestParser::Parser(HttpRequest& request, const char* buf, const int len)
{
    ResultType result = INDETERMINATE;

    int i = 0;
    while (i++ < len)
    {
        result = Consume(request, buf[i]);
        if (result == GOOD or result == BAD)
        {
            return result;
        }
    }

    return result;
}

void HttpRequestParser::Reset()
{
  state_ = Method_Start;
}

HttpRequestParser::ResultType HttpRequestParser::Consume(HttpRequest& req, char input)
{
  switch (state_)
  {
  case Method_Start:
    if (!IsChar(input) || IsCtl(input) || IsTspecial(input))
    {
      return BAD;
    }
    else
    {
      state_ = Method;
      req.method_.push_back(input);
      return INDETERMINATE;
    }
  case Method:
    if (input == ' ')
    {
      state_ = Uri;
      return INDETERMINATE;
    }
    else if (!IsChar(input) || IsCtl(input) || IsTspecial(input))
    {
      return BAD;
    }
    else
    {
      req.method_.push_back(input);
      return INDETERMINATE;
    }
  case Uri:
    if (input == ' ')
    {
      state_ = Http_Version_H;
      return INDETERMINATE;
    }
    else if (IsCtl(input))
    {
      return BAD;
    }
    else
    {
      req.uri_.push_back(input);
      return INDETERMINATE;
    }
  case Http_Version_H:
    if (input == 'H')
    {
      state_ = Http_Version_T_1;
      return INDETERMINATE;
    }
    else
    {
      return BAD;
    }
  case Http_Version_T_1:
    if (input == 'T')
    {
      state_ = Http_Version_T_2;
      return INDETERMINATE;
    }
    else
    {
      return BAD;
    }
  case Http_Version_T_2:
    if (input == 'T')
    {
      state_ = Http_Version_P;
      return INDETERMINATE;
    }
    else
    {
      return BAD;
    }
  case Http_Version_P:
    if (input == 'P')
    {
      state_ = Http_Version_Slash;
      return INDETERMINATE;
    }
    else
    {
      return BAD;
    }
  case Http_Version_Slash:
    if (input == '/')
    {
      req.http_ver_major_ = 0;
      req.http_ver_minor_ = 0;
      state_ = Http_Version_Major_Start;
      return INDETERMINATE;
    }
    else
    {
      return BAD;
    }
  case Http_Version_Major_Start:
    if (IsDigit(input))
    {
      req.http_ver_major_ = req.http_ver_major_ * 10 + input - '0';
      state_ = Http_Version_Major;
      return INDETERMINATE;
    }
    else
    {
      return BAD;
    }
  case Http_Version_Major:
    if (input == '.')
    {
      state_ = Http_Version_Minor_Start;
      return INDETERMINATE;
    }
    else if (IsDigit(input))
    {
      req.http_ver_major_ = req.http_ver_major_ * 10 + input - '0';
      return INDETERMINATE;
    }
    else
    {
      return BAD;
    }
  case Http_Version_Minor_Start:
    if (IsDigit(input))
    {
      req.http_ver_minor_ = req.http_ver_minor_ * 10 + input - '0';
      state_ = Http_Version_Minor;
      return INDETERMINATE;
    }
    else
    {
      return BAD;
    }
  case Http_Version_Minor:
    if (input == '\r')
    {
      state_ = Expecting_Newline_1;
      return INDETERMINATE;
    }
    else if (IsDigit(input))
    {
      req.http_ver_minor_ = req.http_ver_minor_ * 10 + input - '0';
      return INDETERMINATE;
    }
    else
    {
      return BAD;
    }
  case Expecting_Newline_1:
    if (input == '\n')
    {
      state_ = Header_Line_Start;
      return INDETERMINATE;
    }
    else
    {
      return BAD;
    }
  case Header_Line_Start:
    if (input == '\r')
    {
      state_ = Expecting_Newline_3;
      return INDETERMINATE;
    }
    else if (!req.headers_.empty() && (input == ' ' || input == '\t'))
    {
      state_ = Header_Lws;
      return INDETERMINATE;
    }
    else if (!IsChar(input) || IsCtl(input) || IsTspecial(input))
    {
      return BAD;
    }
    else
    {
      req.headers_.push_back(Header());
      req.headers_.back().name.push_back(input);
      state_ = Header_Name;
      return INDETERMINATE;
    }
  case Header_Lws:
    if (input == '\r')
    {
      state_ = Expecting_Newline_2;
      return INDETERMINATE;
    }
    else if (input == ' ' || input == '\t')
    {
      return INDETERMINATE;
    }
    else if (IsCtl(input))
    {
      return BAD;
    }
    else
    {
      state_ = Header_Value;
      req.headers_.back().value.push_back(input);
      return INDETERMINATE;
    }
  case Header_Name:
    if (input == ':')
    {
      state_ = Space_Before_Header_Value;
      return INDETERMINATE;
    }
    else if (!IsChar(input) || IsCtl(input) || IsTspecial(input))
    {
      return BAD;
    }
    else
    {
      req.headers_.back().name.push_back(input);
      return INDETERMINATE;
    }
  case Space_Before_Header_Value:
    if (input == ' ')
    {
      state_ = Header_Value;
      return INDETERMINATE;
    }
    else
    {
      return BAD;
    }
  case Header_Value:
    if (input == '\r')
    {
      state_ = Expecting_Newline_2;
      return INDETERMINATE;
    }
    else if (IsCtl(input))
    {
      return BAD;
    }
    else
    {
      req.headers_.back().value.push_back(input);
      return INDETERMINATE;
    }
  case Expecting_Newline_2:
    if (input == '\n')
    {
      state_ = Header_Line_Start;
      return INDETERMINATE;
    }
    else
    {
      return BAD;
    }
  case Expecting_Newline_3:
    return (input == '\n') ? GOOD : BAD;
  default:
    return BAD;
  }
}

bool HttpRequestParser::IsChar(int c)
{
  return c >= 0 && c <= 127;
}

bool HttpRequestParser::IsCtl(int c)
{
  return (c >= 0 && c <= 31) || (c == 127);
}

bool HttpRequestParser::IsTspecial(int c)
{
  switch (c)
  {
  case '(': case ')': case '<': case '>': case '@':
  case ',': case ';': case ':': case '\\': case '"':
  case '/': case '[': case ']': case '?': case '=':
  case '{': case '}': case ' ': case '\t':
    return true;
  default:
    return false;
  }
}

bool HttpRequestParser::IsDigit(int c)
{
  return c >= '0' && c <= '9';
}


}


