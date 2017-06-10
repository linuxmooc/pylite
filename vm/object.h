#ifndef _PY_OBJECT_H
#define _PY_OBJECT_H

struct py_class;
struct py_object;
struct py_symbol;

typedef struct {
    struct py_symbol *py_name;
    struct py_object *py_value;
} field_t;

#define py_object_member_list                   \
    struct py_object *next;                     \
    struct py_class *py_class;                  \
    int flags;                                  \
    vector_template(field_t, field_vector)

typedef struct py_object {
    py_object_member_list;
} py_object_t;

extern void *py_object_alloc(int size, struct py_class *py_class);
extern py_object_t *py_object_new(struct py_class *py_class);
extern py_object_t *py_object_get_field(py_object_t *this, 
                                        struct py_symbol *py_name);
extern py_object_t *py_object_load_field(py_object_t *this, 
                                         struct py_symbol *py_name);
extern void py_object_set_field(py_object_t *this, 
            struct py_symbol *py_name, py_object_t *py_value);
extern void py_object_dump(py_object_t *this, xml_file_t *xml_file);

struct py_string;
extern struct py_string *py_object_to_string(py_object_t *this);

struct py_class;
extern struct py_class *py_object_class;
extern void py_object_class_init();

static inline bool py_object_is_class(py_object_t *this)
{
    return this->py_class == NULL;
}
#endif
