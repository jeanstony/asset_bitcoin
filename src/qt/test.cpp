#include <stdio.h>
#include <openssl/evp.h>

int main(int argc, char *argv[]){

EVP_MD_CTX *mdctx;
const EVP_MD *md;
unsigned char md_value[EVP_MAX_MD_SIZE];
int md_len, i;

md = EVP_get_digestbyname(argv[1]);
if(!md) {
    printf("Unknown message digest %s\n", argv[1]);
    exit(1);
}

mdctx = EVP_MD_CTX_new();
EVP_DigestInit_ex(mdctx, md, NULL);
EVP_DigestFinal_ex(mdctx, md_value, &md_len);
EVP_MD_CTX_free(mdctx);

printf("Digest is: ");
for (i = 0; i < md_len; i++)
    printf("%02x", md_value[i]);
printf("\n");

exit(0);
}

