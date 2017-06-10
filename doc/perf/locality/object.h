typedef struct object {
    struct object *next;
    int ref_count;
    int value;
} object_t;

object_t *object_list;

void *my_alloc()
{
    object_t *object = object_list;
    object_list = object_list->next;
    return object;
}

void my_free(object_t *object)
{
    object->next = object_list;
    object_list = object;
}

void my_init(int n)
{
    int i;
    for (i = 0; i < n; i++) {
        object_t *object = malloc(sizeof(object_t));
        my_free(object);
    }
}

object_t *object_new(int value)
{
    object_t *this = my_alloc(sizeof(object_t));
    this->ref_count = 1;
    this->value = value;
    return this;
}

void object_free(object_t *this)
{
    this->ref_count -= 1;
    if (this->ref_count == 0)
        my_free(this);
}
