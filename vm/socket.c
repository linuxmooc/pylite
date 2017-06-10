#include "root.h"
#include "socket.h"
#include "tuple.h"
#include "file.h"
#include "cast.h"
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include <netdb.h>

py_class_t *py_socket_class;

py_object_t *py_socket_alloc()
{
    py_socket_t *this = py_object_alloc(sizeof(py_socket_t), py_socket_class);
    this->family = -1;
    this->fd = -1;
    return $(this);
}

py_object_t *py_socket_init(int argc, py_object_t *argv[])
{
    assert_argc(argc, 4);
    py_socket_t *this = $(argv[0]);
    int family = cast_integer(argv[1]);
    int type = cast_integer(argv[2]);
    int proto = cast_integer(argv[3]);

    int fd = socket(family, type, proto);
    if (fd < 0)
        vm_throw(py_io_error);
    this->fd = fd;
    this->family = family;

    return py_none;
}

py_object_t *py_socket_new(int fd, int family)
{
    py_socket_t *this = py_object_alloc(sizeof(py_socket_t), py_socket_class);
    this->family = family;
    this->fd = fd;
    return $(this);
}

static py_object_t *to_address(py_tuple_t *py_tuple, struct sockaddr_in *sin)
{
    if (py_tuple->vector.count != 2) 
        vm_throw(py_type_error);

    py_object_t *py_host = vector_get(&py_tuple->vector, 0);
    py_object_t *py_port = vector_get(&py_tuple->vector, 1);
    char *host = cast_string(py_host);
    int port = cast_integer(py_port);

    struct hostent *entry = gethostbyname(host);
    if (entry == NULL)
        vm_throw(py_type_error);
    in_addr_t *in_addr = (in_addr_t *) entry->h_addr_list[0];

    sin->sin_family = AF_INET;
    sin->sin_addr.s_addr = *in_addr;
    sin->sin_port = htons(port);

    return py_none;
}

py_object_t *py_socket_bind(int argc, py_object_t *argv[])
{
    assert_argc(argc, 2);
    py_socket_t *this = $(argv[0]);
    py_tuple_t *py_tuple = cast_object(argv[1], tuple);

    struct sockaddr_in sin;
    if (to_address(py_tuple, &sin) == NULL)
        vm_rethrow();

    int error = bind(this->fd, $(&sin), sizeof(sin));
    if (error < 0)
        vm_throw(py_io_error);
    return py_none;
}

py_object_t *py_socket_connect(int argc, py_object_t *argv[])
{
    assert_argc(argc, 2);
    py_socket_t *this = $(argv[0]);
    py_tuple_t *py_tuple = cast_object(argv[1], tuple);

    struct sockaddr_in sin;
    if (to_address(py_tuple, &sin) == NULL)
        vm_rethrow();

    int error = connect(this->fd, $(&sin), sizeof(sin));
    if (error < 0)
        vm_throw(py_io_error);
    return py_none;
}

py_object_t *py_socket_listen(int argc, py_object_t *argv[])
{
    assert_argc(argc, 2);
    py_socket_t *this = $(argv[0]);
    int backlog = cast_integer(argv[1]);

    listen(this->fd, backlog);
    return py_none;
}

py_object_t *py_socket_accept(int argc, py_object_t *argv[])
{
    assert_argc(argc, 1);
    py_socket_t *this = cast_object(argv[0], socket);

    struct sockaddr_in client_addr;
    socklen_t client_addr_size = sizeof(sin);
    int client_fd = accept(this->fd, $(&client_addr), &client_addr_size);
    if (client_fd < 0)
        vm_throw(py_io_error);

    py_object_t *py_client = $(py_socket_new(client_fd, this->family));
    py_object_t *py_client_addr = py_none; // Ignore the address

    py_tuple_t *py_tuple = py_tuple_new();
    py_tuple_append(py_tuple, py_client);
    py_tuple_append(py_tuple, py_client_addr); 
    return $(py_tuple);
}

py_object_t *py_socket_setsockopt(int argc, py_object_t *argv[])
{
    assert_argc(argc, 4);
    py_socket_t *this = $(argv[0]);
    int level = cast_integer(argv[1]);
    int name = cast_integer(argv[2]);
    int value = cast_integer(argv[3]);

    socklen_t len = sizeof(value);
    int error = setsockopt(this->fd, level, name, &value, len);
    if (error < 0)
        vm_throw(py_io_error);
    return py_none;
}

py_object_t *py_socket_getsockopt(int argc, py_object_t *argv[])
{
    assert_argc(argc, 2);
    py_socket_t *this = $(argv[0]);
    return py_none;
}

py_object_t *py_socket_send(int argc, py_object_t *argv[])
{
    assert_argc(argc, 2);
    py_socket_t *this = $(argv[0]);

    char *buffer = cast_string(argv[1]);
    int count = strlen(buffer);
    int error = write(this->fd, buffer, count);
    if (error < 0)
        vm_throw(py_io_error);

    return py_none;
}

py_object_t *py_socket_recv(int argc, py_object_t *argv[])
{
    assert_argc(argc, 2);
    py_socket_t *this = $(argv[0]);
    int count = cast_integer(argv[1]);

    char buffer[count];
    int error = read(this->fd, buffer, count);
    if (error < 0)
        vm_throw(py_io_error);
    buffer[error] = 0; 

    py_string_t *py_string = py_string_new(buffer);
    return $(py_string);
}

py_object_t *py_socket_close(int argc, py_object_t *argv[])
{
    assert_argc(argc, 1);
    py_socket_t *this = $(argv[0]);
    close(this->fd);
    return py_none;
}

py_object_t *py_socket_makefile(int argc, py_object_t *argv[])
{
    assert_argc(argc, 2);
    py_socket_t *this = $(argv[0]);
    char *mode = cast_string(argv[1]);

    py_file_t *py_file = py_file_fdopen(this->fd, mode);
    if (py_file == NULL)
        vm_rethrow();
    return $(py_file);
}

native_t py_socket_natives[] = {
    {"__init__", py_socket_init},
    {"accept", py_socket_accept},
    {"send", py_socket_send},
    {"recv", py_socket_recv},
    {"close", py_socket_close},
    {"makefile", py_socket_makefile},
    {"bind", py_socket_bind},
    {"listen", py_socket_listen},
    {"connect", py_socket_connect},
    {"setsockopt", py_socket_setsockopt},
    {"getsockopt", py_socket_getsockopt},
    {NULL}
};

void py_socket_class_init()
{
    py_socket_class = py_class_new("socket");
    py_socket_class->py_alloc = py_socket_alloc;
    py_class_register_natives(py_socket_class, py_socket_natives);
}
