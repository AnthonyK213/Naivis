#include "IO_LogStream.hxx"

IO_LogStream::IO_LogStream(std::ostream &stream, QTextBrowser *textBrowser)
    : myStream(stream) {
  mylogWindow = textBrowser;
  myOldBuf = stream.rdbuf();
  stream.rdbuf(this);
}

IO_LogStream::~IO_LogStream() {
  // output anything that is left
  if (!myString.empty())
    mylogWindow->append(myString.c_str());

  myStream.rdbuf(myOldBuf);
}

std::streambuf::int_type IO_LogStream::overflow(int_type v) {
  if (v == '\n') {
    mylogWindow->append(myString.c_str());
    myString.erase(myString.begin(), myString.end());
  } else
    myString += v;

  return v;
}

std::streamsize IO_LogStream::xsputn(const char *p, std::streamsize n) {
  myString.append(p, p + n);

  size_t pos = 0;
  while (pos != std::string::npos) {
    pos = myString.find('\n');
    if (pos != std::string::npos) {
      std::string tmp(myString.begin(), myString.begin() + pos);
      mylogWindow->append(tmp.c_str());
      myString.erase(myString.begin(), myString.begin() + pos + 1);
    }
  }

  return n;
}
