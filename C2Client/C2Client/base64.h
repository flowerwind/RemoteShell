#pragma once
char * base64_encode(const unsigned char * bindata, char * base64, int binlength);
int base64_decode(const char * base64, unsigned char * bindata);
