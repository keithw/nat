#include <vector>
#include <iostream>

#include "socket.hh"
#include "poller.hh"
#include "util.hh"

using namespace std;
using namespace PollerShortNames;

int main()
{
  try {
    Poller poller;
    vector<UDPSocket> sockets( 16 );

    for ( unsigned int i = 0; i < 16; i++ ) {
      sockets.at( i ).bind( Address( "0", 60000 + i ) );
      poller.add_action( Action( sockets.at( i ),
				 Direction::In,
				 [i, &sockets] () {
				   auto rec = sockets.at( i ).recv();
				   cout << "UDP datagram from " << rec.source_address.to_string() << "\n";
				   return ResultType::Continue;
				 } ) );
    }

    while ( true ) {
      const auto ret = poller.poll( -1 );
      if ( ret.result == PollResult::Exit ) {
	return ret.exit_status;
      }
    }
  } catch ( const exception & e ) {
    print_exception( e );
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
