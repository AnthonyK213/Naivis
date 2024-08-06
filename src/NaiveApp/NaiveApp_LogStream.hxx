#ifndef _Naivis_NaiveApp_LogStream_HeaderFile
#define _Naivis_NaiveApp_LogStream_HeaderFile

#include <QTextBrowser>

#include <iostream>
#include <streambuf>
#include <string>

class NaiveApp_LogStream : public ::std::basic_streambuf<char> {
public:
  NaiveApp_LogStream(::std::ostream &theStream, QTextBrowser *theTB);

  ~NaiveApp_LogStream();

protected:
  virtual int_type overflow(int_type theV) override;

  virtual ::std::streamsize xsputn(const char *theP,
                                   ::std::streamsize theN) override;

private:
  ::std::string myString;
  ::std::ostream &myStream;
  ::std::streambuf *myOldBuf;
  QTextBrowser *myLogWindow;
};

#endif
