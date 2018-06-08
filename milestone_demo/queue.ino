queue* queue_create(){
    return (queue*) calloc(sizeof(queue), 1);
}

boolean queue_append(queue* q, data* body) {
    if(!q) return false;

    queue_elem* new_elem = (queue_elem*) malloc(sizeof(queue_elem));
    new_elem->body = body;
    new_elem->next = NULL;

    if (!q->last){
        q->first = new_elem;
        q->last = new_elem;
    }
    else {
        q->last->next = new_elem;
        q->last = new_elem;
    }
    q->count++;
    return true;
}

data* queue_peek(queue* q) {
    return q->first->body;
}

data* queue_pop(queue* q) {
    if (!q || !q->first) return NULL;

    queue_elem* tmp = q->first;
    data* body = tmp->body;
    q->first = q->first->next;

    if (!q->first) q->last = NULL;

    free(tmp);
    q->count--;
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
    else{
        return NULL;
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
