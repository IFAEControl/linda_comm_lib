#include <iostream>

#include <Poco/Net/DNS.h>

#include "linda-lib.hpp"

using Poco::Net::DNS;
using Poco::Net::IPAddress;
using Poco::Net::HostEntry;

// TODO: Ejemplo de una función de biblioteca
void fnlindalib()
{
  const HostEntry& entry = DNS::hostByName("www.appinf.com");
}

int test() {
	return 0;
}
