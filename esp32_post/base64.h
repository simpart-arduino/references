#ifndef __BASE64_H__
#define __BASE64_H__

char *base64_encode(const unsigned char *, size_t, size_t *);
unsigned char *base64_decode(const char *,size_t, size_t &);

#endif
