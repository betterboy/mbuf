#include "mbuf.h"

int main()
{
    mbuf_t *mbuf = (mbuf_t *)malloc(sizeof(mbuf_t));
    mbuf_init(mbuf, 1);
    mbuf->enable_log = 1;

    char data[] = "abcdefghijklmn";
    int len = strlen(data);
    MBUF_ENQ_WITH_TYPE(mbuf, &len, int);
    MBUF_ENQ(mbuf, data, len);
    assert(mbuf->data_size = strlen(data));
    printf("blk=%d,data_size=%d,alloc=%d\n", mbuf->blk_count, mbuf->data_size, mbuf->alloc_size);
    char buf[1024] = {0};
    uint32_t rc = mbuf_copy(mbuf, buf, 1024);
    buf[rc] = '\0';
    int str_len = 0;
    memcpy(&str_len, buf, 4);
    assert(rc == 18 && str_len == strlen(data) && strlen(buf+4) == strlen(data));
    printf("copy:rc=%d,len=%d,str=%s,real_len=%d\n", rc, str_len, buf+sizeof(int), strlen(buf+4));

    memset(buf, 0, sizeof(buf));
    rc = mbuf_remove(mbuf, buf, 1024);
    buf[rc] = '\0';
    mbuf_drain(mbuf, 10000);
    mbuf_drain(mbuf, 10000);
    mbuf_copy(mbuf, buf, 12);
    mbuf_remove(mbuf, buf, 12);
    mbuf_free(mbuf);

    mbuf_t *new_mbuf = (mbuf_t *)malloc(sizeof(mbuf_t));
    mbuf_init(new_mbuf, 1);
    int i = 0;
    int total = 0;
    for (i = 0; i <= 100000; ++i) {
        total += strlen(data) * 2;
        mbuf_add_span(new_mbuf, data, strlen(data));
        mbuf_add(new_mbuf, data, strlen(data));
        assert(new_mbuf->data_size == total);
    }

    printf("new_mbuf: data_size=%d,blk=%d,alloc_size=%d\n", new_mbuf->data_size, new_mbuf->blk_count, new_mbuf->alloc_size);

    for (i = 0; i <= 100000; ++i) {
        mbuf_drain(new_mbuf, i);
    }

    printf("drain new_mbuf: data_size=%d,blk=%d,alloc_size=%d\n", new_mbuf->data_size, new_mbuf->blk_count, new_mbuf->alloc_size);
    mbuf_add(new_mbuf, data, strlen(data));
    mbuf_add_span(new_mbuf, data, strlen(data));
    mbuf_free(new_mbuf);
    return 0;
}