#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print(int* arr,int n){
    for(int i=0;i<n;i++)
        printf("%d",arr[i]);
    printf("\n");
}
int main()
{
    // Finding length of file and reading text
    FILE *fptr;
    fptr = fopen("sender.txt", "r");
    fseek(fptr, 0, SEEK_END);
    long int size = ftell(fptr);
    printf("File size = %d\n",size);
    fseek(fptr, 0, SEEK_SET);
    char s[size+1];
    fread (s, 1, size, fptr);
    s[size]='\0'; 
    fclose(fptr);
    printf("String = %s\n",s);

    // Converting anscii to binary
    size_t len = strlen(s);
    char *binary = malloc(len*8 + 1);
    int k=len*8;
    int bin[k];
    for(size_t i = 0; i < len; ++i) {
        char ch = s[i];
        for(int j = 7; j >= 0; --j)
            bin[i*8+(7-j)]=(ch & (1 << j)?1:0);
    }
    printf("Binary = ");
    print(bin,k);

    // Finding no. of redundant bits
    int r=0,twos=1;
    while(twos<k+r+1){
        twos*=2;
        r++;
    }
    printf("k = %d\nr = %d\n",k,r);

    // Adding redundant bits
    int ans[k+r];
    memset(ans, 0, (k+r+1)*sizeof(ans[0]));
    int pos=k-1;
    int t=1;
    for(int i=1;i<k+r+1;i++){
        if(i==t) t*=2;
        else ans[k+r-i]=bin[pos--];
    }
    printf("Binary without finding redundant bits = ");
    print(ans,k+r);
  
    // Applying Humming code
    t=1;
    for(int i=1;i<=r;i++){
        int c=0;
        for(int j=1;j<k+r+1;j++)
            if((j&t)&&ans[k+r-j]){c++;}
        // printf("%d\n",c);
        ans[k+r-t]=c%2;
        t*=2;
    }
    printf("Binary with finding redundant bits    = ");
    print(ans,k+r);

    // sending the result in channel
    fptr=fopen("channel.txt","w");
    for(int i=0;i<k+r;i++)
        fprintf(fptr,"%d", ans[i]);
    fclose(fptr);
}