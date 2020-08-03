#include <iostream>

#include "Poco/Net/SocketAddress.h"
#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/SocketStream.h>
#include <Poco/StreamCopier.h>

#include "linda-lib.hpp"

using Poco::Net::IPAddress;

// TODO: Ejemplo de una función de biblioteca
void fnlindalib()
{
  //const HostEntry& entry = DNS::hostByName("www.appinf.com");
}

int test() {
  Poco::Net::SocketAddress sa("127.0.0.1", 8080);
  Poco::Net::StreamSocket socket(sa);
  Poco::Net::SocketStream str(socket);
  str << "GET / HTTP/1.1\r\n"
         "Host: www.appinf.com\r\n"
         "\r\n";

  str.flush();
  Poco::StreamCopier::copyStream(str, std::cout);
	return 0;
}
