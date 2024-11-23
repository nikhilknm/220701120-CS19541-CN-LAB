#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print(int* arr,int n){
    for(int i=0;i<n;i++)
        printf("%d",arr[i]);
    printf("\n");
}
char* decode(int* bin,int n){
    char a[2];a[1]='\0';a[0]=0;
    char *ans=malloc(n/8+1);ans[0]='\0';
    for(int i=0;i<n;){
        a[0]+=(bin[i]<<(7-i%8));
        i++;
        if(i%8==0){
            strcat(ans,a);
            a[0]=0;
        }
    }
    return ans;
}
int main()
{
    FILE *fptr;
    fptr = fopen("channel.txt", "r");
    fseek(fptr, 0, SEEK_END);
    long int size = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);
  
    char s[size+1];
    fread (s, 1, size+1, fptr);
    size_t len = strlen(s);
  
    int k=len;
    int bin[k];
    for(int i=0;i<k;i++)bin[i]=s[i]-'0';
    printf("Binary = ");
    print(bin,k);

    // Finding no of redundant bit
    int r=0,twos=len;
    while(twos){
        twos/=2;
        r++;
    }

    int t=1;
    for(int i=1;i<=r;i++){
        int c=0;
        for(int j=1;j<k+1;j++)
            if((j&t)&&bin[k-j])c++;
        bin[k-t]=c%2;
        t*=2;
    }
    fclose(fptr);

    int ans[k-r],rv=0;
    memset(ans, 0, (k-r)*sizeof(ans[0]));
    int pos=k-r-1;
    int p=0;
    t=1;
    // Appling humming code
    for(int i=1;i<k+1;i++){  
        if(i==t) {t*=2;rv+=bin[k-i]<< p;p++ ;}
        else ans[pos--]=bin[k-i];
    }
    printf("Binary after removing redundant bits = ");
    print(ans,k-r);
    pos=k-r;
    if(rv!=0)printf("Error at %d",rv);
    else {
        char* dans=decode(ans,pos);
        fopen("received.txt","w");
        for(int i=0;i<pos/8;i++)
            fprintf(fptr,"%c", dans[i]);
        fclose(fptr);
        printf("String = %s\n",dans);
    }
}