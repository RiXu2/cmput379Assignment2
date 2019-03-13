#include <stdio.h>
#include <openssl/md5.h>
#include <stdlib.h>

int main()
{
    unsigned char c[MD5_DIGEST_LENGTH];
    char *filename="1.txt";
    int i;
    FILE *inFile = fopen (filename, "rb");
    MD5_CTX mdContext;
    int bytes;
    unsigned char data[1024];

    if (inFile == NULL) 
	{
        printf ("%s can't be opened.\n", filename);
        return 0;
    }

    MD5_Init (&mdContext);
    while ((bytes = fread (data, 1, 1024, inFile)) != 0)
        MD5_Update (&mdContext, data, bytes);
    MD5_Final (c,&mdContext);
    for(i = 0; i < MD5_DIGEST_LENGTH; i++) printf("%02x", c[i]);
    printf (" %s\n", filename);
    fclose (inFile);


    char * hash = "50104b087b31a40db9b9f8aaea058d92";
    long long number = strtol(hash, NULL, 16);
    printf("hash %02llx\n", number);
    return 0;
}
