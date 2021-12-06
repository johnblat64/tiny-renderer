#ifndef util_h
#define util_h

#define swapValues(a, b, T){ \
    T temp = a; \
    a = b; \
    b = temp; \
}

#define max(a,b)(a > b ? a : b)
#define min(a,b)(a < b ? a : b)

#endif