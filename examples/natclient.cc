#include <vector>
#include <iostream>
#include <thread>

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

    const string remote_ip = argv[ 1 ];
    UDPSocket socket;
    socket.bind( Address( "0", 50000 ) );

    for ( int i = 1; i < argc; i++ ) {
      const Address dest( argv[ i ], 60000 );
      cerr << "Sending to " << dest.to_string() << "... ";
      socket.sendto( Address( argv[ i ], 60000 ),
		     "Hello, Dima." );
      cerr << "done.\n";
      this_thread::sleep_for( 50ms );
    }
  } catch ( const exception & e ) {
    print_exception( e );
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
