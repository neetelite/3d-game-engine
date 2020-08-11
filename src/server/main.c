#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include "../type.c"
#include "../memory.c"
#include "../network.c"

#define CONNECTION_PORT 5000

int
main(void)
{

	#if 1
	struct Net_Connection *connection = net_alloc();
	net_init(connection, NET_UDP, NET_ADDRESS_ANY, CONNECTION_PORT);
	net_bind(connection);

	struct Net_Address address;
	u32 len = sizeof(address);

	net_receive_out(connection, &address, &len);
	net_print(connection);

	#else
	char server_message[256] = "You have reached the server!\n";

	/* Create the server socket */
	int socket;
	socket = socket(NET_IPV4, NET_UDP, NET_IP);

	/* Define the server address */
	struct Net_Address address;
	address.family = NET_IPV4;
	address.port = net_from_host(9002);
	address.address = NET_ADDRESS_ANY;

	/* Bind the socket to our specified IP and port */
	bind(socket, (struct sockaddr *)&address, sizeof(address));

	u32 connection_max = 5;
	listen(socket, connection_max);

	int client_socket;
	client_socket = accept(socket, NULL, NULL);

	/* Send the message */
	send(client_socket, server_message, sizeof(server_message), 0);

	/* Close */
	//close(server_socket);
	#endif

	return(0);
}
