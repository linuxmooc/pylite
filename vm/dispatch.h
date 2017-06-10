#define load_const() \
    ((void *) (const_vector[load_short(py_code_vector + pc + 1)]))
#define load_immed() load_short(py_code_vector + pc + 1)

#define interpret(opcode)                           \
    case opcode:

#define vm_abort()                                  \
    do {                                            \
        sp = bp;                                    \
        assert(py_error);                           \
        return NULL;                                \
    } while(0)

#define vm_raise(py_class)                          \
    do {                                            \
        py_error = py_error_new(py_class);          \
        goto raise_error;                           \
    } while (0)

#define vm_check_result(py_result, argc)            \
    do {                                            \
        vm_stack_eject(argc);                       \
        if (py_result == NULL) {                    \
            assert(py_error);                       \
            goto raise_error;                       \
        }                                           \
    } while(0)

#define vm_push_result(py_result, argc)             \
    do {                                            \
        vm_check_result(py_result, argc);           \
        vm_stack_push(py_result);                   \
    } while(0)

#define vm_jump(ok, offset)                         \
    do {                                            \
        if (ok)                                     \
            pc = offset;                            \
        else                                        \
            pc += 3;                                \
    } while (0)

interpret(OP_NOP)
{
    pc += 1;
    break;
}

interpret(OP_PRINT)
{
    py_object_t *py_object = vm_stack_pop();
    py_string_t *py_string = py_object_to_string(py_object);
    vm_check_result(py_string, 0);
    printf("%s\n", py_string->value);
    pc += 1;
    break;
}

interpret(OP_HALT)
{
    pc += 1;
    break;
}

interpret(OP_LABEL)
{
    pc += 0;
    break;
}

interpret(OP_NOT)
{
    pc += 1;
    py_object_t *py_bool = vm_stack_pop();
    bool ok = compute_bool(py_bool);
    if (ok)
        vm_stack_push(py_false);
    else
        vm_stack_push(py_true);
    break;
}

interpret(OP_NEG)
{
    py_object_t *py_result = unary_op(OP_NEG, 1, vm_stack + sp);
    vm_push_result(py_result, 1);
    pc += 1;
    break;
}

interpret(OP_COM)
{
    py_object_t *py_result = unary_op(OP_COM, 1, vm_stack + sp);
    vm_push_result(py_result, 1);
    pc += 1;
    break;
}

interpret(OP_AND)
{
    py_object_t *py_result = integer_op_integer(OP_AND, 2, vm_stack + sp);
    vm_push_result(py_result, 2);
    pc += 1;
    break;
}

interpret(OP_OR)
{
    py_object_t *py_result = integer_op_integer(OP_OR, 2, vm_stack + sp);
    vm_push_result(py_result, 2);
    pc += 1;
    break;
}

interpret(OP_XOR)
{
    py_object_t *py_result = integer_op_integer(OP_XOR, 2, vm_stack + sp);
    vm_push_result(py_result, 2);
    pc += 1;
    break;
}

interpret(OP_LSH)
{
    py_object_t *py_result = integer_op_integer(OP_LSH, 2, vm_stack + sp);
    vm_push_result(py_result, 2);
    pc += 1;
    break;
}

interpret(OP_RSH)
{
    py_object_t *py_result = integer_op_integer(OP_RSH, 2, vm_stack + sp);
    vm_push_result(py_result, 2);
    pc += 1;
    break;
}

interpret(OP_ADD)
{
    py_object_t *py_result = double_op_double(OP_ADD, 2, vm_stack + sp);
    vm_push_result(py_result, 2);
    pc += 1;
    break;
}

interpret(OP_SUB)
{
    py_object_t *py_result = double_op_double(OP_SUB, 2, vm_stack + sp);
    vm_push_result(py_result, 2);
    pc += 1;
    break;
}

interpret(OP_MUL)
{
    py_object_t *py_result = double_op_double(OP_MUL, 2, vm_stack + sp);
    vm_push_result(py_result, 2);
    pc += 1;
    break;
}

interpret(OP_DIV)
{
    py_object_t *py_result = double_op_double(OP_DIV, 2, vm_stack + sp);
    vm_push_result(py_result, 2);
    pc += 1;
    break;
}

interpret(OP_MOD)
{
    py_object_t *py_result = double_op_double(OP_MOD, 2, vm_stack + sp);
    vm_push_result(py_result, 2);
    pc += 1;
    break;
}

interpret(OP_JMP)
{
    int offset = load_immed();
    pc = offset;
    break;
}

interpret(OP_JTRUE)
{
    int offset = load_immed();
    py_object_t *py_bool = vm_stack_pop();
    bool ok = compute_bool(py_bool);
    vm_jump(ok, offset);
    break;
}

interpret(OP_JFALSE)
{
    int offset = load_immed();
    py_object_t *py_bool = vm_stack_pop();
    bool ok = compute_bool(py_bool);
    vm_jump(!ok, offset);
    break;
}

interpret(OP_JLT)
{
    int offset = load_immed();
    bool ok = jcompare(OP_JLT, 2, vm_stack + sp);
    vm_stack_eject(2);
    vm_jump(ok, offset);
    break;
}

interpret(OP_JLE)
{
    int offset = load_immed();
    bool ok = jcompare(OP_JLE, 2, vm_stack + sp);
    vm_stack_eject(2);
    vm_jump(ok, offset);
    break;
}

interpret(OP_JEQ)
{
    int offset = load_immed();
    bool ok = jcompare(OP_JEQ, 2, vm_stack + sp);
    vm_stack_eject(2);
    vm_jump(ok, offset);
    break;
}

interpret(OP_JNE)
{
    int offset = load_immed();
    bool ok = jcompare(OP_JNE, 2, vm_stack + sp);
    vm_stack_eject(2);
    vm_jump(ok, offset);
    break;
}

interpret(OP_JGE)
{
    int offset = load_immed();
    bool ok = jcompare(OP_JGE, 2, vm_stack + sp);
    vm_stack_eject(2);
    vm_jump(ok, offset);
    break;
}

interpret(OP_JGT)
{
    int offset = load_immed();
    bool ok = jcompare(OP_JGT, 2, vm_stack + sp);
    vm_stack_eject(2);
    vm_jump(ok, offset);
    break;
}

interpret(OP_ITERATOR)
{
    py_object_t *py_object = vm_stack_pop();
    py_object_t *py_field = py_object_load_field(py_object, py_symbol__iter__);
    vm_check_result(py_field, 0);
    py_object_t *py_iterator = vm_call(py_field, 0, vm_stack + sp);
    vm_push_result(py_iterator, 0);
    pc += 1;
    break;
}

interpret(OP_NEXT)
{
    int offset = load_immed();
    py_object_t *py_iterator = vm_stack_pop();
    py_object_t *py_next;
    py_object_t *py_item;
    if (py_iterator->py_class == py_range_class) {
        py_item = py_range_next_fast($(py_iterator));
    } else {
        py_next = py_object_load_field(py_iterator, py_symbol__next__);
        vm_check_result(py_next, 0);
        py_item = vm_call(py_next, 0, vm_stack + sp);
        vm_stack_eject(0);
    }

    /* Not finish */
    if (py_item != NULL) {
        vm_stack_push(py_item);
        pc = offset;
        break;
    }

    if (py_error_is_stop()) {
        vm_stack_push(py_error);
        pc += 3;
        break;
    } else {
        goto raise_error;
    }
}

interpret(OP_CALL)
{
    int argc = load_immed();
    py_object_t *py_callee = vm_stack_pop();
    py_object_t *py_result = vm_call(py_callee, argc, vm_stack + sp);
    vm_push_result(py_result, argc);
    pc += 3;
    break;
}

interpret(OP_RETURN)
{
    py_object_t *py_result = vm_stack_pop();
    vm_stack_eject(py_lambda->local_count);
    assert(sp == bp);
    return py_result;
}

interpret(OP_DUP_TOP)
{
    int offset = load_immed();
    py_object_t *py_object = vm_stack_top(offset);
    vm_stack_push(py_object);
    pc += 3;
    break;
}

interpret(OP_POP)
{
    vm_stack_pop();
    pc += 1;
    break;
}

interpret(OP_LOAD_CONST)
{
    py_object_t *source = load_const();
    vm_stack_push(source);
    pc += 3;
    break;
}

interpret(OP_LOAD_GLOBAL)
{
    py_symbol_t *py_name = load_const();
    py_object_t *py_object = py_object_get_field($(py_module), py_name);
    if (py_object == NULL) {
        py_object = py_object_get_field($(py_builtin_module), py_name);
        if (py_object == NULL)
            vm_raise(py_name_error);
    }
    vm_stack_push(py_object);
    pc += 3;
    break;
}

interpret(OP_STORE_GLOBAL)
{
    py_object_t *py_object = vm_stack_pop();
    py_symbol_t *py_name = load_const();
    py_object_set_field($(py_module), py_name, py_object);
    pc += 3;
    break;
}

interpret(OP_LOAD_LOCAL)
{
    int offset = load_immed();
    py_object_t *py_object= vm_stack[bp + offset];
    vm_stack_push(py_object);
    pc += 3;
    break;
}

interpret(OP_STORE_LOCAL)
{
    py_object_t *py_object = vm_stack_pop(); 
    int offset = load_immed();
    vm_stack[bp + offset] = py_object;
    pc += 3;
    break;
}

// LOAD_ITEM
// sp -> vector 
//       index
//
// item = vector[index]
interpret(OP_LOAD_ITEM)
{
    py_object_t *py_vector = vm_stack_top(0);
    py_class_t *py_class = py_vector->py_class;
    py_object_t *py_item;
    py_object_t *py_field;

    py_object_t **args = vm_stack + sp;
    if (py_class == py_list_class) {
        py_item = py_list_get_item(2, args);
    } else if (py_class == py_dict_class) {
        py_item = py_dict_get_item(2, args);
    } else if (py_class == py_tuple_class) {
        py_item = py_tuple_get_item(2, args);
    } else {
        py_field = py_class_load_field(py_class, py_symbol__getitem__);
        vm_check_result(py_field, 0);
        py_item = vm_call(py_field, 2, vm_stack + sp);
    }
    vm_push_result(py_item, 2);

    pc += 1;
    break;
}

// STORE_ITEM
// sp -> vector
//       index
//       item
//
// vector[index] = item
interpret(OP_STORE_ITEM)
{
    py_object_t *py_vector = vm_stack_top(0);
    py_class_t *py_class = py_vector->py_class;
    py_object_t *py_result;
    py_object_t *py_field;

    py_object_t **args = vm_stack + sp;
    if (py_class == py_list_class) {
        py_result = py_list_set_item(3, args);
    } else if (py_class == py_dict_class) {
        py_result = py_dict_set_item(3, args);
    } else {
        py_field = py_class_load_field(py_class, py_symbol__setitem__);
        vm_check_result(py_field, 0);
        py_result = vm_call(py_field, 3, vm_stack + sp);
    }
    vm_check_result(py_result, 3);

    pc += 1;
    break;
}

// LOAD_FIELD name
// sp -> object 
//
// value = object.name
interpret(OP_LOAD_FIELD)
{
    py_symbol_t *py_name = load_const();
    assert(py_name->py_class == py_symbol_class);
    py_object_t *py_object = vm_stack_pop();

    py_object_t *py_value = py_object_load_field(py_object, py_name);
    if (py_value == NULL)
        vm_raise(py_attr_error);
    vm_stack_push(py_value);
    pc += 3;
    break;
}

// STORE_FIELD name
// sp -> object
//       value
//
// object.name = value
interpret(OP_STORE_FIELD)
{
    py_symbol_t *py_name = load_const();
    assert(py_name->py_class == py_symbol_class);
    py_object_t *py_object = vm_stack_pop();
    py_object_t *py_value = vm_stack_pop();
    py_object_set_field(py_object, py_name, py_value);
    pc += 3;
    break;
}

interpret(OP_NEW_CLASS)
{
    py_symbol_t *py_name = load_const();
    py_class_t *py_class = py_class_new(py_name->value);
    vm_stack_push(py_class);
    pc += 3;
    break;
}

interpret(OP_NEW_LIST)
{
    int count = load_immed();
    py_list_t *py_list = py_list_new(count);
    vm_stack_push(py_list);
    pc += 3;
    break;
}

interpret(OP_NEW_DICT)
{
    py_dict_t *py_dict = py_dict_new();
    vm_stack_push(py_dict);
    pc += 1;
    break;
}

interpret(OP_NEW_TUPLE)
{
    py_tuple_t *py_tuple = py_tuple_new();
    int count = load_immed();
    for (int i = 0; i < count; i++) {
        py_object_t *py_item = vm_stack_pop();
        py_tuple_append(py_tuple, py_item);
    }
    vm_stack_push(py_tuple);
    pc += 3;
    break;
}

interpret(OP_NEW_SLICE)
{
    py_slice_t *py_slice = py_slice_new(2, vm_stack + sp);
    vm_stack_eject(2);
    vm_stack_push(py_slice);
    pc += 1;
    break;
}

interpret(OP_LOAD_MODULE)
{
    py_symbol_t *py_name = load_const();
    py_module_t *new_module = load_py_module(py_module->dir, py_name->value);
    vm_push_result(new_module, 0);
    pc += 3;
    break;
}

interpret(OP_LOAD_THIS_MODULE)
{
    vm_stack_push(py_module);
    pc += 1;
    break;
}

interpret(OP_INHERIT)
{
    py_class_t *child = vm_stack_pop();
    py_class_t *parent = vm_stack_pop();
    assert(child->py_class == NULL);
    assert(parent->py_class == NULL);
    child->parent = parent;
    child->py_alloc = parent->py_alloc;
    pc += 1;
    break;
}

interpret(OP_RAISE)
{
    py_error = vm_stack_pop();
    if (py_error->py_class == NULL) {
        if (py_class_is_subclass($(py_error), py_exception))
            py_error = py_error_new($(py_error));
        else
            py_error = py_error_new(py_type_error);
        goto raise_error;
    }

    if (!py_class_is_subclass(py_error->py_class, py_exception))
        py_error = py_error_new(py_type_error);
    goto raise_error;
    break;
}

interpret(OP_CATCH)
{
    py_class_t *favor = vm_stack_pop();
    py_error = vm_stack_top(0);

    if (py_class_is_subclass(py_error->py_class, favor))
        pc += 1;
    else
        goto catch_failed;
    break;
}

raise_error:
{
    int line_number = vector_get(&py_lambda->line_number_map, pc);
    py_error_add(py_error, py_module->name, py_lambda->name, line_number);

catch_failed:
    // recalibrate the stack
    sp = bp - py_lambda->local_count; 
    vm_stack_push(py_error);

    guard_t *guard = py_lambda_locate_guard(py_lambda, pc);
    if (guard == NULL)
        vm_abort();

    pc = guard->handler;
    break;
}

interpret(OP_FINALLY_CALL)
{
    py_double_t *py_temp = py_double_new(pc + 3);
    vm_stack_push(py_temp);
    int offset = load_immed();
    pc = offset;
    break;
}

interpret(OP_FINALLY_EXIT)
{
    int index = load_immed();
    py_object_t *py_offset = vm_stack[bp + index]; 

    /* rethrow the exception */
    if (py_offset->py_class != py_double_class)
        vm_abort();

    /* finally_call, return to caller */
    int offset = py_object_to_integer(py_offset);
    pc = offset;
    break;
}
