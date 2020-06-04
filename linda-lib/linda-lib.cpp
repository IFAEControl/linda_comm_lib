#include <iostream>

#include <Poco/Net/DNS.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/SocketStream.h>
#include <Poco/StreamCopier.h>

#include "linda-lib.hpp"

// TODO: Ejemplo de una función de biblioteca
void fnlindalib()
{
}

int test() {
	Poco::Net::SocketAddress sa("www.google.com", 80);
	Poco::Net::StreamSocket socket(sa);
	Poco::Net::SocketStream str(socket);
	
	str << "GET / HTTP/1.1\r\n"
		"Host: www.google.com\r\n"
		"\r\n";
	str.flush();
	
	Poco::StreamCopier::copyStream(str, std::cout);
	return 0;
}
