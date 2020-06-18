#include "mbuf.h"

int main()
{
    mbuf_t *mbuf = (mbuf_t *)malloc(sizeof(mbuf_t));
    mbuf_init(mbuf, 1);
    mbuf->enable_log = 1;

    char data[] = "abcdef";
    int len = strlen(data);
    MBUF_ENQ_WITH_TYPE(mbuf, &len, int);
    MBUF_ENQ(mbuf, data, len);
    printf("blk=%d,data_size=%d,alloc=%d\n", mbuf->blk_count, mbuf->data_size, mbuf->alloc_size);
    char buf[12] = {0};
    uint32_t rc = mbuf_copy(mbuf, buf, 12);
    buf[rc] = '\0';
    int str_len = 0;
    memcpy(&str_len, buf, 4);
    printf("copy:rc=%d,len=%d,str=%s\n", rc, str_len, buf+sizeof(int));

    memset(buf, 0, sizeof(buf));
    rc = mbuf_remove(mbuf, buf, 12);
    buf[rc] = '\0';
    printf("rc=%d,str=%s\n", rc, buf+4);

    mbuf_drain(mbuf, 10000);
    mbuf_drain(mbuf, 10000);
    mbuf_copy(mbuf, buf, 12);
    mbuf_remove(mbuf, buf, 12);

    mbuf_free(mbuf);

    return 0;
}