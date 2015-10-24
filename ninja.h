struct message {
	int recp;	/* recipient of the message */
	int sndr;	/* sender of the message */
	int size;	/* size of the message, in bytes */
	char *msg;	/* message data */
	int type;	/* msg data type, text, picture, etc */
}
