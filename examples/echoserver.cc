/* -*-mode:c++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

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
    if ( argc != 3 ) {
      abort();
    }
    
    vector<UDPSocket> sockets;
    sockets.emplace_back();
    sockets.back().bind( { argv[ 1 ], uint16_t{ 60000 } } );
    sockets.emplace_back();
    sockets.back().bind( { argv[ 2 ], uint16_t{ 60100 } } );

    Poller poller;
    for ( auto & sock : sockets ) {
      poller.add_action( Action( sock,
                                 Direction::In,
                                 [&sock] () {
                                   auto rec = sock.recv();
                                   cout << "\tUDP datagram from " << rec.source_address.to_string() << "\n";
                                   sock.sendto(rec.source_address, rec.source_address.to_string());
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
