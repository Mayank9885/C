#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include <signal.h>

typedef struct {
	char *appName;
	char *arg;
} Config;

Config *configs = NULL;
int configCount = 0;


typedef struct {
	pid_t pid; // process id of the client
	FILE *fromClient;
	FILE *toClient;
	char *name;
	bool online;
	bool nameSet;
} Client;

Client *clients;
int clientCount;


int size_of(char **arr) {
	
	int size = 0;
	while (arr[size]) {
		size++;
	}
	return size;
}

bool add_config(Config *cfg, char *line) {
	// asdasdas:dasdas
	
	const char *delim = ":";
	char *tok;
	
	tok = strtok(line, delim);
	if (!tok) {
		return false;
	}
	cfg->appName = strdup(tok);
	
	tok = strtok(NULL, delim);
	if (!tok) {
		return false;
	}
	cfg->arg = strdup(tok);
	
	return true;
	
}

void add_configs(char **lines) {
	int count = size_of(lines);
	configs = malloc(sizeof(Config) * count);
	clients = malloc(sizeof(Client) * count);
	clientCount= configCount = count;
	
	for (int i = 0; i < count; i++) {
		if (!add_config(&configs[i], lines[i])) {
			continue;
			exit (0);
		}
	}
	
}

char *read_line(FILE *file) {
	
	char *line = malloc(80);
	
	int pos, next;
	pos = 0;
	
	while (true) {
		next = fgetc(file);
		if ((next == EOF || next == '\n') && pos == 0) {
			free(line);
			return NULL;
		}
		if (next == EOF || next == '\n') {
			line[pos] = 0;
			break;
		}
		line[pos++] = (char) next;
	}
	
	return line;
		
}



char **read_lines(const char *filename) {
	FILE *fp = fopen(filename, "r");
	if (!fp) {
		return NULL;
	}
	char **result = malloc(sizeof(char *) * 2);
	int capacity = 1;
	int size = 0;
	char *line;
	
	while ((line = read_line(fp)) != NULL) {
		if (line[0] == '#') {
			free(line);
			continue;
		}
		
		result[size++] = line;
		capacity++;
		result = realloc(result, sizeof(char *) * capacity);
	}
	result[size] = NULL;
	return result;
}



void parse_configs(const char *filename) {
	char **lines = read_lines(filename);
	if (!lines) {
		// file open err exit accordingly
		fprintf(stderr, "Usage: server configfile\n");
		exit (1);
	}
	add_configs(lines);
}


void exec_client(Client *client, const Config *cfg) {
	
	int readPipe[2];
	int writePipe[2];
	pipe(readPipe);
	pipe(writePipe);
	client->fromClient = fdopen(readPipe[0], "r");
	client->toClient = fdopen(writePipe[1], "w");
	pid_t pid = fork();
	
	if (!pid) {
        (void) close(writePipe[1]);
        (void) close(readPipe[0]);
        if (writePipe[0] != fileno(stdin)) {
            (void) dup2(writePipe[0], fileno(stdin));
        }
        if (readPipe[1] != fileno(stdout)) {
            (void) dup2(readPipe[1], fileno(stdout));
        }
        int devNull = open("/dev/null", 2, "w");
        dup2(devNull, 2); //stderr now prints to devnull
        
		char *argv[3] = {cfg->appName, cfg->arg, NULL};
		
		execvp(cfg->appName, &argv[0]);
        fprintf(stderr," could not exec %s\n", cfg->appName);
        raise(SIGCHLD);
		exit(1);
	}
	(void) close(readPipe[1]);
	(void) close(writePipe[0]);
	client->pid = pid;
}

void exec_clients() {
	
	for (int i = 0; i < clientCount; i++) {
		
		exec_client(&clients[i], &configs[i]);
		clients[i].name = NULL;
		clients[i].online = true;
		clients[i].nameSet = false;
	}
}


char *read_message(Client *client) {
	/*
	char *line = read_line(client->fromClient);
	if (!line) {
		return NULL;
	}*/
	
	static char buf[100];
	
	if (!fgets(buf, 99, client->fromClient)) {
		return NULL;
	}
	if (buf[strlen(buf) - 1] == '\n') {
		buf[strlen(buf) - 1] = 0;
	}
        //printf("%s\n",buf);
	return strdup(&buf[0]);
}

void send_message(const char *message, Client *client) {
	
	fprintf(client->toClient, "%s\n", message);
	fflush(client->toClient);
}

void send_who_message(Client *client) {
	
	send_message("WHO:", client);
}

void send_msg_message(Client *client, const char *name, const char *text) {
	
	char message[1024];
	sprintf(message, "MSG:%s:%s", name, text);
        fflush(stdout);
	send_message(&message[0], client);
}

void send_msg_message_all(const char *name, const char *text) {
	
	for (int i = 0; i < clientCount; i++) {
		send_msg_message(&clients[i], name, text);
	}
}

bool is_name_in_use(const char *name) {
	
	for (int i = 0; i < clientCount; i++) {
		if (clients[i].name == NULL) {
			continue;
		}
		if (!strcmp(clients[i].name, name)) {
			return true;
		}
	}
	return false;
}


bool is_all_names_set(void) {
	for (int i = 0; i < clientCount; i++) {
		if (clients[i].online == false) {
			continue;
		}
		if (clients[i].name == NULL) {
			return false;
		}
	}
	return true;
	
}


Client *get_client_by_name(char *name) {
	if (name == NULL || strlen(name) == 0) {
		return NULL;
	}
	for (int i = 0; i < clientCount; i++) {
		if (clients[i].name == NULL) {
			continue;
		}
		
		if (!strcmp(name, clients[i].name)) {
			return (&clients[i]);
		}
	}
	return NULL; // indicating no such client with that name
}

void receive_messages(Client *client) {
	
	char *line;
	char *data;
	
	while (true) {
		
		line = read_message(client);
                //printf("%s\n",line);
		if (!line) {
			printf("(%s has left the chat)\n", client->name);
			client->online = false;
			break;
		} else if (!strncmp("CHAT:", line, 5)) {
			// chat 
			data = line + 5;
			printf("(%s) %s\n", client->name, data);
			
			// send every client MSG type message 
			send_msg_message_all(client->name, data);
		} else if (!strncmp("KICK:", line, 5)) {
			data = line + 5;
		//	printf("this is data %s\n",line);
			Client *clientToKick = get_client_by_name(data);
			if (clientToKick) {
				send_message("KICK:", clientToKick);
			}			
		} else if (!strncmp("DONE:", line, 5)) {
			//printf("im here\n");    
                        free(line);
			break;
			
		}  else if (!strcmp("QUIT:", line)) {
			printf("(%s has left the chat)\n", client->name);
			client->online = false;
			break;
		} else {
			// comm error from client 
			client->online = false;
			break;
		}
		free(line);
                //break;
	}
}

void round_robin_chat(int index) {
	Client *client = &clients[index];
	
	if (!client->online) {
		return;
	}
		
	// send YT 
	send_message("YT:", client);
        //printf("%d\n", index);
	
	// read as many messages as possible 
	// until read DONE or QUIT
	// if quit came then make him offline
	receive_messages(client);
}


void round_who(int index) {
	Client *client = &clients[index];
	char *line;
	if (!client->online) {
		return;
	}
	
	// who protocol
	// ask for name 
	send_who_message(client);
	line = read_message(client);
	if (!line || strncmp(line, "NAME:", 5)) {
		//client dc or message not started with NAME
		//printf("(%s has left the chat)\n", client->name);
		client->online = false;
		return;
	}
	char *name = line + 5;
	if (is_name_in_use(name)) {
		
		send_message("NAME_TAKEN:", client);
		round_who(index);
		return;
	} else {
		client->name = strdup(name);
		printf("(%s has entered the chat)\n", client->name);
	}
	
	
}


bool is_everybody_offline() {
	for (int i = 0; i < clientCount; i++) {
		if (clients[i].online) {
			return false;
		}
	}
	return true;
}

void run() {
	
	while (!is_all_names_set()) {
		for (int i = 0; i < clientCount; i++) {
			
			if (clients[i].online && clients[i].name == NULL) {
				//fprintf(stderr, "calling rh for %d\n", i);
				round_who(i);
				//fprintf(stderr, "called rh for %d his name is %s\n", i, clients[i].name);
			}
		}
	}
	
	while (!is_everybody_offline()) {
		
		
		
	    for (int i = 0; i < clientCount; i++) {
			round_robin_chat(i);
		}
    }
	// chat is done no more online clients
	exit(0);
}

void s_handle(int sig) {
	
	//printf("received %d\n", sig);
}



int main(int argc, char** argv) {
    if (argc != 2) {
		// usage err
		fprintf(stderr, "Usage: server configfile\n");
		exit (1);
	}
	signal(SIGCHLD, s_handle);
	signal(SIGPIPE, s_handle);
	parse_configs(argv[1]);
	exec_clients();
	run();
	exit (0);
}
