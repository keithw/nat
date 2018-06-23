/* -*-mode:c++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#include <vector>
#include <iostream>
#include <thread>

#include "socket.hh"
#include "poller.hh"
#include "util.hh"

using namespace std;
using namespace PollerShortNames;

int main(int argc, char** argv)
{
  enum Args {
    PROGRAM = 0,
    ECHO_SERVER_IP = 1,
    ECHO_SERVER_PORT = 2,
    NUM_ARGS = 3
  };

  if (argc < NUM_ARGS) {
    cerr << "Usage: " << argv[0] << " <echo_server> <echo_server_port>" << endl;
    return EXIT_FAILURE;
  }

  try {
    Address echo_server( argv[ ECHO_SERVER_IP ], argv[ ECHO_SERVER_PORT ] );
    vector<UDPSocket> sockets;
    for ( uint16_t port = 1024; port < 1024 + 512; port++ ) {
      sockets.emplace_back();
      try {
        sockets.back().bind( { "0", port } );
      } catch ( const exception & e ) {
        /* ... */
      }
    }

    Poller poller;
    for ( auto & sock : sockets ) {
      poller.add_action( Action( sock,
                                 Direction::In,
                                 [&sock] () {
                                   auto echo = sock.recv().payload;
                                   cout << sock.local_address().to_string() << " => " << echo << endl;
                                   return ResultType::Continue;
                                 } ) );
    }

    unsigned int next_sock = 0;
    while ( true ) {
      for ( int x = 0; x < 16; x++ ) {
        try {
          sockets.at( (next_sock++) % sockets.size() ).sendto( echo_server, "ping" );
        } catch ( const unix_error & e ) {
          if ( e.code().value() == EAGAIN ) {
            cerr << "(continuing after EAGAIN...)\n";
          }
        }
      }
        
      const auto ret = poller.poll( 1 );

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
