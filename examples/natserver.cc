#include <vector>
#include <iostream>

#include "socket.hh"
#include "poller.hh"
#include "util.hh"

using namespace std;
using namespace PollerShortNames;

Address self_external_address(const Address& echo_server, UDPSocket* socket) 
{
  socket->sendto( echo_server, "ping" );
  auto echo = socket->recv().payload;
  size_t pos = echo.find(":");
  if (pos == string::npos) {
    throw runtime_error( string("bad echo response ") + echo );
  }
  auto ip = echo.substr(0, pos);
  auto port = atoi(echo.substr(pos + 1, string::npos).c_str());
  return Address(ip, port);
}


int main(int argc, char** argv)
{
  enum Args {
    PROGRAM = 0,
    LISTENING_PORT = 1,
    ECHO_SERVER_IP = 2,
    ECHO_SERVER_PORT = 3,
    NUM_ARGS = 4
  };

  if (argc < NUM_ARGS) {
    cerr << "Usage: " << argv[0] << " <listening_port> <echo_server> <echo_server_port>" << endl;
    return 1;
  }

  try {
    Poller poller;

    UDPSocket socket;
    socket.bind( Address( "0", argv[LISTENING_PORT]) );

    auto external = self_external_address( 
      Address( argv[ECHO_SERVER_IP], atoi(argv[ECHO_SERVER_PORT] ) ), 
      &socket);
    cout << "Self external address: " << external.to_string() << endl;

    poller.add_action( Action( socket,
				 Direction::In,
				 [&socket] () {
				   auto rec = socket.recv();
				   cout << "UDP datagram from " << rec.source_address.to_string() << "\n";
           socket.sendto(rec.source_address, "Hello From Server!");
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
