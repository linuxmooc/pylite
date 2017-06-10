#include "root.h"
#include "mod_builtin.h"
#include "mod_math.h"
#include "mod_sys.h"
#include "mod_os.h"
#include "mod_socket.h"

void py_modules_init(void)
{
    py_builtin_module_init();
    py_math_module_init();
    py_sys_module_init();
    py_os_module_init();
    py_socket_module_init();
}
