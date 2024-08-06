#include "NaiveApp_LogStream.hxx"

#include <cstdio>

NaiveApp_LogStream::NaiveApp_LogStream(::std::ostream &theStream,
                                       QTextBrowser *theTB)
    : myStream(theStream) {
  myLogWindow = theTB;
  myOldBuf = theStream.rdbuf();
  theStream.rdbuf(this);
}

NaiveApp_LogStream::~NaiveApp_LogStream() {
  if (!myString.empty())
    myLogWindow->append(myString.c_str());
  myStream.rdbuf(myOldBuf);
}

::std::streambuf::int_type NaiveApp_LogStream::overflow(int_type theV) {
  if (theV == '\n') {
#ifndef NDEBUG
    ::std::printf("%s\n", myString.c_str());
    ::std::fflush(stdout);
#endif
    myLogWindow->append(myString.c_str());
    myString.erase(myString.begin(), myString.end());
  } else
    myString += theV;

  return theV;
}

::std::streamsize NaiveApp_LogStream::xsputn(const char *theP,
                                             std::streamsize theN) {
  myString.append(theP, theP + theN);

  size_t aPos = 0;
  while (aPos != ::std::string::npos) {
    aPos = myString.find('\n');
    if (aPos != ::std::string::npos) {
      ::std::string aTmp(myString.begin(), myString.begin() + aPos);
#ifndef NDEBUG
      ::std::printf("%s\n", aTmp.c_str());
      ::std::fflush(stdout);
#endif
      myLogWindow->append(aTmp.c_str());
      myString.erase(myString.begin(), myString.begin() + aPos + 1);
    }
  }

  return theN;
}
