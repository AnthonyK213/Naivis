#ifndef _Naivis_IO_LogStream_HeaderFile
#define _Naivis_IO_LogStream_HeaderFile

#include <QTextBrowser>

#include <iostream>
#include <streambuf>
#include <string>

class IO_LogStream : public std::basic_streambuf<char> {
public:
  IO_LogStream(std::ostream &stream, QTextBrowser *textBrowser);

  ~IO_LogStream();

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
