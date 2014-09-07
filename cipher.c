/* Fix me */
#include <string.h>
#include "blowfish.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>


int main(int argc,char **argv)
{
  extern int optind;
  extern char *optarg;
  unsigned char from[128], to[128];
  int len = 128;
  char *file1;
  char *file2;
  int c = 0;
  int eflag = 0;//Encrypt input to output
  int dflag = 0;//Decrypt input to output using supplied pass
  //only one of d or e is supplied! eflag==0 && dflag==1 | eflag==1 && dflag == 0
  int pflag = 0;//If p set, then let user input pass (-p PASS)on cmd line. Else, use getpass()
  char *password = "default";
  int hflag = 0;
  int vflag = 0;
  int err = 0;
  //getopt(argc, argv,"edp:hv")

  /* define a structure to hold the key */
  BF_KEY key;
  while((c = getopt(argc, argv,"edp:hv")) != -1){
    switch (c){
      case 'e': eflag = 1;
                break;
      case 'd': dflag = 1;
                break;
      case 'p': pflag = 1;
                password = optarg;
                break;
      case 'h': hflag = 1;
                break;
      case 'v': vflag = 1;
                break;
      case '?': err = 1;
                break;
      default: break;
    }
  }
  
  if(err == 0){//no error
    if(hflag == 0){
        if(pflag == 0)password = getpass("Please enter password: ");
        if(eflag == 0 && dflag == 1){
          
            //for(int i = 0;i < argc;++i){
              //printf("Arguments left are %s\n",argv[i]);
              file1 = argv[optind];
              file2 = argv[optind+1];
            //}
          
          printf("You want to decrypt %s to %s\n",file1,file2);
        }else if(eflag == 1 && dflag == 0){
          
            //for(int i = 0;i < argc;++i){
              //printf("Arguments left are %s\n",argv[i]);
              file1 = argv[optind];
              file2 = argv[optind+1];
            //}
          
          printf("You want to encrypt %s to %s",file1,file2);
        }else{
          fprintf(stderr,"You must supply only one e or d option.\n");
          return -1;
        }
    }else{
      fprintf(stderr,"Program will terminate.\n");
      return -1;
    }
  }else{
    fprintf(stderr,"Unrecognized command option.\n");
    return -1;
  }
  /* a temp buffer to read user input (the user's password) */
  unsigned char temp_buf[16];

  /* don't worry about these two: just define/use them */
  int n = 0;			/* internal blowfish variables */
  unsigned char iv[8];		/* Initialization Vector */

  /* fill the IV with zeros (or any other fixed data) */
  memset(iv, 0, 8);

  /* call this function once to setup the cipher key */
  BF_set_key(&key, 16, temp_buf);

  /*
   * This is how you encrypt an input char* buffer "from", of length "len"
   * onto output buffer "to", using key "key".  Jyst pass "iv" and "&n" as
   * shown, and don't forget to actually tell the function to BF_ENCRYPT.
   */
  BF_cfb64_encrypt(from, to, len, &key, iv, &n, BF_ENCRYPT);

  /* Decrypting is the same: just pass BF_DECRYPT instead */
  BF_cfb64_encrypt(from, to, len, &key, iv, &n, BF_DECRYPT);
  return 0;
}
