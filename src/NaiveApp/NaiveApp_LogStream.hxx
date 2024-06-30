#ifndef _Naivis_NaiveApp_LogStream_HeaderFile
#define _Naivis_NaiveApp_LogStream_HeaderFile

#include <QTextBrowser>

#include <iostream>
#include <streambuf>
#include <string>

class NaiveApp_LogStream : public std::basic_streambuf<char> {
public:
  NaiveApp_LogStream(std::ostream &stream, QTextBrowser *textBrowser);

  ~NaiveApp_LogStream();

protected:
  virtual int_type overflow(int_type v) override;

  virtual std::streamsize xsputn(const char *p, std::streamsize n) override;

private:
  std::ostream &myStream;
  std::streambuf *myOldBuf;
  std::string myString;
  QTextBrowser *mylogWindow;
};

#endif
