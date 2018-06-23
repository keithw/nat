/* -*-mode:c++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#include <vector>
#include <iostream>

#include "socket.hh"
#include "poller.hh"
#include "util.hh"

using namespace std;
using namespace PollerShortNames;

int main(int argc, char** argv)
{
  try {
    Poller poller;
    UDPSocket socket;
    uint16_t port = 60000;
    if ( argc > 1 ) {
      port = stoi(argv[1]);
    }

    socket.bind( { "0", port } );
    poller.add_action( Action( socket,
                               Direction::In,
                               [&socket] () {
                                 auto rec = socket.recv();
                                 cout << "\tUDP datagram from " << rec.source_address.to_string() << "\n";
                                 socket.sendto(rec.source_address, rec.source_address.to_string());
                                 return ResultType::Continue;
                               } ) );

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
