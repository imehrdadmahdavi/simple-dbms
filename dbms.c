#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

//constants defining max sizes (in bytes)
#define MAX_DATA 512
#define MAX_ROWS 100

//define fix-sized structures##############
//start####################################

//address book structure
struct Address{
	int id;
	//flag determining the address record is set
	int set;
	char name[MAX_DATA];
	char email[MAX_DATA];
};

struct Database{
	struct Address rows[MAX_ROWS];
};

struct Connection{
	//file struct
	FILE *file;
	struct Database *db;
};

//end######################################

//fucntions to work with structures########
//start####################################

void Database_close(struct Connection *conn);

//function to abort program with an error
void die(const char *message, struct Connection *conn)
{
		//check error number returned from the system
    if(errno){
    	//print errno
      perror(message);
      } else {
				if(message)
					printf("ERROR: %s\n", message);
	}
	Database_close(conn);
	exit(1);
}

void Address_print(struct Address *addr)
{
	printf("%d %s %s\n", addr->id, addr->name, addr->email);
}

void Database_load(struct Connection *conn)
{
	int rc = fread(conn->db, sizeof(struct Database), 1, conn->file);
	if(rc != 1){
		die("Failed to load database!", conn);
	}
}

struct Connection *Database_open(const char *filename, char mode)
{
	/*
	using malloc() to ask OS for a large amount of
	memory when we create the Database (this is heap)
	*/
	struct Connection *conn = malloc(sizeof(struct Connection));

	//equal to: if(conn == NULL)
	if(!conn){
		die("Memory error!", conn);
	}

	conn->db = malloc(sizeof(struct Database));
	if(!conn->db){
		die("Memory error!", conn);
	}

	if(mode == 'c'){
		/*
		w: creates an empty file for writing. If a file with
		the same name already exists, its content is erased
		and the file is considered as a new empty file.
		*/
		conn->file = fopen(filename, "w");
	} else {
		/*
		r+: opens a file to update both reading and writing.
		The file must exist.
		*/
		conn->file = fopen(filename, "r+");

		if(!conn->file){
			die("Failed to open the file!", conn);
		}

		if(conn->file){
			Database_load(conn);
		}
	}

	return conn;
}

void Database_close(struct Connection *conn)
{
	if(conn){
		if(conn->file)
			fclose(conn->file);
		if(conn->db)
			free(conn->db);
		free(conn);
	}
}

void Database_write(struct Connection *conn)
{
	/*
	set the file position to the beginning of
	the file of the given stream
	*/
	rewind(conn->file);

	/*
	write data from the array pointed to, by
	conn->db pointer to the given stream
	*/
	int rc = fwrite(conn->db, sizeof(struct Database), 1, conn->file);
	if(rc != 1)
		die("Failed to write database!", conn);

	//flush the output buffer of the stream
	rc = fflush(conn->file);
	if(rc == -1)
		die("Cannot flush database!", conn);
}

void Database_create(struct Connection *conn)
{
	int i = 0;

	for(i = 0; i < MAX_ROWS; i++){
		struct Address addr = {.id = i, .set = 0};
		conn->db->rows[i] = addr;
	}
}

void Database_set(struct Connection *conn, int id, const char *name,
	const char *email)
{
	struct Address *addr = &conn->db->rows[id];

	if(addr->set)
		die("This record is already set, delete it first!", conn);

	addr->set = 1;
	/*
	strncpy: copies up to MAX_DATA characters from the string pointed
	to, by name (source) to addr->name(destination). In a case where
	the length of name is less than that of MAX_DATA, the remainder
	of dest will be padded with null bytes.
	*/
	char *res = strncpy(addr->name, name, MAX_DATA);

	if(!res)
		die("Name copy failed!", conn);

	res = strncpy(addr->email, email, MAX_DATA);
	if(!res)
		die("Email copy failed!", conn);
}

void Database_get(struct Connection *conn, int id)
{
	struct Address *addr = &conn->db->rows[id];

	if(addr->set){
		Address_print(addr);
	} else {
		die("The provided ID doesn't exist in database!", conn);
	}

}

void Database_delete(struct Connection *conn, int id)
{
	/*
	copying structure prototypes by creating a temp local
	Address and only init its id and set fields and copying
	it into the actual location which we want to delete its
	value.

	Note: memcpy() should not be used as modern C allows us
	to simply assign one struct to another and it’ll handle
	the copying for us.
	*/
	struct Address addr = {.id = id,.set = 0 };
	conn->db->rows[id] = addr;
}

void Database_list(struct Connection *conn)
{
	int i = 0;
	struct Database *db = conn->db;

	for(i = 0; i < MAX_ROWS; i++){
		struct Address *cur = &db->rows[i];
		if(cur->set){
			Address_print(cur);
		}
	}
}
//END######################################

int main(int argc, char *argv[])
{
	if(argc < 3){
		printf("USAGE: dbms <dbfile> <action> [action params]");
		exit(1);
	}

	char *filename = argv[1];
	char action = argv[2][0];
	struct Connection *conn = Database_open(filename, action);
	int id = 0;

	if(argc > 3)
	// atoi(): convert string to int
	id = atoi(argv[3]);
	if(id > MAX_ROWS)
		die("Out of ragne ID! There's not that many records!", conn);

	switch(action){
	case 'c':
		Database_create(conn);
		Database_write(conn);
		break;

	case 'g':
		if(argc != 4)
			die("Need an id to get!", conn);
		Database_get(conn, id);
		break;

	case 's':
		if (argc != 6)
			die("Need id, name, email to set!", conn);
		Database_set(conn, id, argv[4], argv[5]);
		Database_write(conn);
		break;

	case 'd':
		if (argc != 4)
			die("Need id to delete", conn);

		Database_delete(conn, id);
		Database_write(conn);
		break;

	case 'l':
		Database_list(conn);
	break;

	default:
		die("Invalid action: c=create, g=get, s=set, d=del, l=list", conn);
	}

	die(NULL,conn);
	return 0;
}
