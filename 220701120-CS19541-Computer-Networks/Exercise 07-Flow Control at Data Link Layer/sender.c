#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<windows.h>
int MAX_SEQ=7;
typedef struct frame{
    int seq_no;
    int ack_no;
    char info;
} F;
bool between(int a, int b, int c){
    return ((a <= b) && (b < c)) || ((c < a) && (a <= b)) || ((b < c) && (c < a));
}
void send_frame(int frame_no,int frame_expected,char data,int* pos){
    FILE* fptr=fopen("senderBuff.txt","a");
    fseek(fptr, 0, *pos);
    F s;
    s.seq_no=frame_no;
    s.ack_no=(frame_expected+MAX_SEQ)%(MAX_SEQ+1);
    if(data==' ')data='.';
    s.info=data;
    fprintf(fptr,"%d %d %c\n", s.seq_no,s.ack_no,s.info);
    *pos = ftell(fptr);
    printf("Sender: %d\n",*pos);
    fclose(fptr);
}
int inc(int* a){
    (*a)++;
    (*a)%=(MAX_SEQ+1);
    return *a;
}
bool receive_frame(F* r,int* pos){
    FILE* fptr=fopen("receiverBuff.txt","r");
    fseek(fptr, 0, SEEK_END); 
    if(*pos==ftell(fptr))return false;
    *pos = ftell(fptr);
    fseek(fptr, -7, SEEK_END);   
    bool a=fscanf(fptr,"%d %d %c\n",&r->seq_no,&r->ack_no,&r->info)==3;
    if(r->info=='.')r->info=' ';
    printf("Receiver: %d\n",*pos);
    fclose(fptr);
    return true;
}
int main(){
    FILE *fptr;
    fptr = fopen("sender_sending.txt", "r");
    fseek(fptr, 0, SEEK_END);
    long int size = ftell(fptr);
    printf("File size = %d\n",size);
    fseek(fptr, 0, SEEK_SET);
    char s[size+1];
    fread (s, 1, size, fptr);
    s[size]='\0'; 
    fclose(fptr);
    printf("String = %s\n",s);
    int buff_size=(MAX_SEQ+1)/2,
    cur_ack_no=7,
    frame_exp=0,
    next_frame_to_send=0,
    ack_expected=0,
    window_boundary=buff_size,
    ack_window_buffer=buff_size,
    ackbuffer[buff_size],
    outbuffer[buff_size],
    inbuffer[buff_size];
    bool arrived[buff_size];
    for(int i=0;i<buff_size;i++){
        ackbuffer[i]=false;
        arrived[i]=false;
    }
    FILE* sender_received=fopen("sender_received.txt","w");
    int send_buff=0,receive_buff=0,i=0;
    F r;
    bool f,fr=false;
    for(;i<buff_size;i++){
        outbuffer[i]=s[i];
    }
    printf("Started\n");
    while(true){
        f=true;
        if(i<size+buff_size){
            send_frame(next_frame_to_send,frame_exp,outbuffer[next_frame_to_send%buff_size],&send_buff);
            f=false;
        } 
        Sleep(10000);
        bool fe=true;
        if(receive_frame(&r,&receive_buff)){
            if(between(frame_exp,r.seq_no,window_boundary)&&arrived[r.seq_no%buff_size]==false){
                fe=false;
                fr=true;
                cur_ack_no=r.ack_no;
                arrived[r.seq_no%buff_size]=true;
                inbuffer[r.seq_no%buff_size]=r.info;
                while(arrived[frame_exp%buff_size]){
                    fprintf(sender_received,"%c",inbuffer[frame_exp%buff_size]);
                    arrived[frame_exp%buff_size]=false;
                    inc(&frame_exp);
                    inc(&window_boundary);
                }
            }
            f=false;
            printf("%d %d %c\n",r.seq_no,r.ack_no,r.info);
        }
        if(fe && fr)cur_ack_no+=1;
        if(between(ack_expected,cur_ack_no,ack_window_buffer)){
            ackbuffer[cur_ack_no%buff_size]=true;
            while(ackbuffer[ack_expected%buff_size]){
                outbuffer[ack_expected%buff_size]=s[i++];
                ackbuffer[ack_expected%buff_size]=false;
                inc(&ack_expected);
                next_frame_to_send=ack_expected;
                inc(&ack_window_buffer);
            }
        }
        if(f)break;
    }
    fclose(sender_received);
}