enum ClientMessage
{
	client_message_join,
	client_message_leave,
	client_message_input,
};

enum ServerMessage
{
	server_message_join_result,
	server_message_state,
};

struct Packet
{
	u8 type;
	u8 player_id;
	void *data;
};
