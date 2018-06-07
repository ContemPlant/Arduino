queue* queue_create(){
    queue* q = (queue*) malloc(sizeof(queue));
    q->first = NULL;
    q->last = NULL;
    q->count = 0;
    return q;
}

void queue_append(queue* q, data* body) {
    queue_elem* new_elem = (queue_elem*) malloc(sizeof(queue_elem));
    new_elem->body = body;
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
    return q->first->body;
}

data* queue_pop(queue* q) {
    if (q->first){
        q->first = q->first->next;
        if (q->count == 1) {
            q->last = NULL;
        }
    }
    q->count--;

    data* body = q->first->body;
    free(q);
    return body;
}

data* queue_compress(queue* q){
    if (q->count) {
        data* cmp = (data*) malloc(sizeof(data));
        memcpy(cmp, q->first->body, sizeof(data));

        queue_elem* tmp = q->first->next;
        while (tmp) {
            merge_packets(cmp, tmp->body, cmp);
            tmp = tmp->next;
        }
        return cmp;
    }
}

void queue_empty(queue* q) {
    queue_elem* next;
    queue_elem* cur = q->first;

    while(cur) {
        next = cur->next;
        free(cur->body);
        free(cur);
        cur = next; 
    }
}
