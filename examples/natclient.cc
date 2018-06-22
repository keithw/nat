#include <vector>
#include <iostream>

#include "socket.hh"
#include "poller.hh"
#include "util.hh"

using namespace std;
using namespace PollerShortNames;

int main( int argc, char *argv[] )
{
  try {
    if ( argc <= 0 ) {
      abort();
    }

    if ( argc != 2 ) {
      cerr << "Usage: " << argv[ 0 ] << " IP\n";
      return EXIT_FAILURE;
    }

    const string remote_ip = argv[ 1 ];
    UDPSocket socket;

    for ( unsigned int i = 0; i < 16; i++ ) {
      socket.sendto( Address( remote_ip, 60000 + i ),
		     "Hello, Dima." );
    }
  } catch ( const exception & e ) {
    print_exception( e );
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
