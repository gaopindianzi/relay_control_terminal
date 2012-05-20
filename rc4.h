#ifndef __RC4_H__
#define __RC4_H__


class QEncryptRc4
{
    const static int sbox_len = 256;
public:
    QEncryptRc4() ;
    void SetKey(unsigned char * key,int keylen);
    void Encrypt(const unsigned char * source,unsigned char * target,int len);
private:
    void init_sbox(void);
public:
    unsigned char sbox[sbox_len];
    unsigned char kbox[sbox_len];
};

#endif

