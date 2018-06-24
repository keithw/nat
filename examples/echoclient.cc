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
    OTHER_IP = 2,
    OTHER_PORT = 3,
    LOCAL_PORT = 4,
    NUM_ARGS = 5
  };

  if (argc < NUM_ARGS) {
    cerr << "Usage: " << argv[0] << " <echo_server> <other_ip> <other_port> <local_port>" << endl;
    return EXIT_FAILURE;
  }

  try {
    UDPSocket sock;
    sock.bind( Address( "0", argv[LOCAL_PORT] ) );
    
    Poller poller;
    poller.add_action( Action( sock,
                               Direction::In,
                               [&sock] () {
                                 auto echo = sock.recv();
                                 cout << sock.local_address().to_string() << " => " << echo.source_address.to_string() << " => " << echo.payload << endl;
                                 return ResultType::Continue;
                               } ) );

    uint16_t next_port = 60000;
    while ( true ) {
      try {
        sock.sendto( Address( argv[ECHO_SERVER_IP], next_port ), "ping" );
        sock.sendto( Address( argv[OTHER_IP], argv[OTHER_PORT] ), "data" );
      } catch ( const unix_error & e ) {
        if ( e.code().value() == EAGAIN ) {
          cerr << "(continuing after EAGAIN...)\n";
        }
      }
        
      const auto ret = poller.poll( 100 );

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
