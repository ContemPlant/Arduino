queue* queue_create(){
    queue* q = malloc(sizof(queue));
    q->first = NULL;
    q->last = NULL;
    q->count = 0;
    return q;
}

void queue_append(queue* q, data* new_elem) {
    new_elem->next = NULL;
    if (q->last){
        q->first = new_elem;
        q->last = new_elem;
    }
    else {
        q->last->next = new_elem;
        q->last = new_elem;
    }
    q->count++;
}

data* queue_peek(queue* q) {
    return q->first;
}

data* queue_pop(queue* q) {
    if (q->first){
        q->first = q->first->next;
        if (q->count == 1) {
            q->last = NULL
        }
    }
    q->count--;

    return q->first;
}

data* queue_compress(queue* q){
    if (q->count < 2) {
        return q->first;
    }
    data* cmp = malloc(sizeof(data));
    data* tmp = q->first->next;
    while (tmp) {
        merge_packets(cmp, tmp, cmp);
        tmp = tmp->next;
    }
    return cmp;
}

void queue_empty(queue* q) {
    data* next;
    data* cur = q->first;

    while(cur) {
        next = cur->next;
        free(cur);
        cur = next; 
    }
}
