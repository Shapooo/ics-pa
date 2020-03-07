#include "klib.h"

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  assert(s);
  int result = 0;
  while (s[result] != '\0') {
    result++;
  }
  return result;
}

char *strcpy(char *dst, const char *src) {
  assert(dst && src);
  int index = 0;
  do {
    dst[index] = src[index];
  } while (src[index++] != '\0');
  return dst;
}

char* strncpy(char* dst, const char* src, size_t n) {
  assert(dst && src);
  int index = 0;
  while (index < n && src[index] != '\0') {
    dst[index] = src[index];
    index++;
  }
  while (index < n) {
    dst[index++] = 0;
  }
  return dst;
}

char* strcat(char* dst, const char* src) {
  assert(dst && src);
  int dst_index = 0, src_index = 0;
  while (dst[dst_index] != '\0') {
    dst_index++;
  }
  while (src[src_index] != '\0') {
    dst[dst_index++] = src[src_index++];
  }
  dst[dst_index] = '\0';
  return dst;
}

int strcmp(const char* s1, const char* s2) {
  assert(s1 && s2);
  int index = 0;
  while (s1[index] != '\0' && s2[index] != '\0') {
    if (s1[index] < s2[index]) {
      return -1;
    } else if (s1[index] > s2[index]) {
      return 1;
    }
    index++;
  }
  if (s1[index]) {
    return 1;
  } else if (s2[index]) {
    return -1;
  } else {
    return 0;
  }
}

int strncmp(const char* s1, const char* s2, size_t n) {
  assert(s1 && s2);
  int index = 0;
  while (index < n) {
    if (s1[index] < s2[index]) {
      return -1;
    } else if (s1[index] > s2[index]) {
      return 1;
    }
    index++;
  }
  return 0;
}

void* memset(void* v,int c,size_t n) {
  assert(v);
  int index = 0;
  unsigned char* tmpv = (unsigned char*) v;
  while (index < n) {
    tmpv[index++] = (unsigned char) c;
  }
  return v;
}

void* memcpy(void* out, const void* in, size_t n) {
  assert(out && in);
  char *tmpout = out;
  const char *tmpin = in;
  int index = 0;
  while (index < n) {
    tmpout[index] = tmpin[index];
    index++;
  }
  return out;
}

int memcmp(const void* s1, const void* s2, size_t n){
  const unsigned char *s1_tmp = s1, *s2_tmp = s2;
  int index = 0;
  while (index < n) {
    if (s1_tmp[index] < s2_tmp[index]) {
      return -1;
    } else if (s1_tmp[index] > s2_tmp[index]) {
      return 1;
    }
    index++;
  }
  return 0;
}

#endif
