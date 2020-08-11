#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#define NET_IPV4 AF_INET
#define NET_TCP SOCK_STREAM
#define NET_UDP SOCK_DGRAM
#define NET_IP IPPROTO_IP

#define NET_ADDRESS_ANY INADDR_ANY

struct Net_Address
{
	u16 family;
	u16 port;
	u32 address;
	u8 zero[8];
};

#define CONNECTION_BUFFER_MAX 1024

enum Net_ConnectionStatus
{
	net_connection_status_null,
	net_connection_status_open,
};

struct Net_Connection
{
	u32 socket;
	u32 status;
	struct Net_Address address;

	u8 buffer[CONNECTION_BUFFER_MAX];
};

struct Net_Connection *
net_alloc(void)
{
	struct Net_Connection *result = NULL;
	result = mem_alloc(1, sizeof(*result), true);
	return(result);
}

u32
net_from_host(u32 port)
{
	u32 result = 0;
	result = htons(port);
	return(result);
}

u32
addr_from_cstr(char *cstr)
{
	u32 result = 0;
	result = inet_addr(cstr);
	return(result);
}

int
net_socket(u32 family, u32 type, u32 protocol)
{
	int result = 0;
	result = socket(family, type, protocol);
	return(result);
}

void
net_bind(struct Net_Connection *connection)
{
	bind(connection->socket, (struct sockaddr *)&connection->address, sizeof(struct sockaddr));
}

void
net_connect(struct Net_Connection *connection)
{
	i32 result = connect(connection->socket,
			     (struct sockaddr *)&connection->address,
			     sizeof(connection->address));

	if(result < 0)
	{
		fprintf(stderr, "Net: Connect has failed\n");
	}

	connection->status = net_connection_status_open;
}

void
net_init(struct Net_Connection *connection, u32 protocol, u32 address, u32 port)
{
	/* Create socket */
	connection->socket = net_socket(NET_IPV4, protocol, NET_IP);

	connection->address.family = NET_IPV4;
	connection->address.port = net_from_host(port);

	connection->address.address = address;
}

u64
net_receive_out(struct Net_Connection *connection, struct Net_Address *out, u32 *len)
{
	u64 result = 0;
	recvfrom(connection->socket, connection->buffer, CONNECTION_BUFFER_MAX, 0,
		 (struct sockaddr *)out, len);
	return(result);
}

u64
net_receive(struct Net_Connection *connection)
{
	u64 result = 0;
	result = recv(connection->socket, connection->buffer, CONNECTION_BUFFER_MAX, 0);
	return(result);
}

void
net_send_to(struct Net_Connection *connection, char *data, struct Net_Address *address)
{
	i32 result = sendto(connection->socket, data, strlen(data), MSG_NOSIGNAL,
			    (struct sockaddr *)address, sizeof(*address));
	if(result < 0)
	{
		printf("Net: Send to has failed with code: %d\n", result);
	}
}

void
net_send(struct Net_Connection *connection, char *data)
{
	i32 result = send(connection->socket, data, strlen(data), MSG_NOSIGNAL);
	if(result < 0)
	{
		printf("Net: Send has failed with code: %d\n", result);
	}
}

void
net_print(struct Net_Connection *connection)
{
	printf("%s", connection->buffer);
}
