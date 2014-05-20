#include <malloc.h>

void *emalloc(size_t amt){
#ifdef DEBUG
    printf("At line number %d in file %s\n", __LINE__, __FILE__);
    printf("Allocating memory of %d size\n", amt);
#endif
    void *v = malloc(amt);  
    if(!v){
        fprintf(stderr, "out of mem\n");
        exit(-1);
    }
#ifdef DEBUG
    printf("At line number %d in file %s\n", __LINE__, __FILE__);
#endif
    return v;
}
