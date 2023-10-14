#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

using namespace std;

struct Message {
    long mType;
    char mData[50];
};

int main() {
    // Create a message queue
    key_t key = ftok(".", 'm');
    int msgId = msgget(key, 0666 | IPC_CREAT);
    if (msgId == -1) {
        perror("msgget");
        return 1;
    }

    // send a message
    Message msg;
    msg.mType = 1;
    strcpy(msg.mData, "Hello World");

    if (msgsnd(msgId, &msg, sizeof(msg.mData), 0) == -1) {
        perror("msgsnd");
        return 1;
    }

    // receive a message
    if (msgrcv(msgId, &msg, sizeof(msg.mData), 1, 0) == -1) {
        perror("msgrcv");
        return 1;
    }

    cout << "Out recvied message: " << msg.mData << endl;

    // close the message queue
    if (msgctl(msgId, IPC_RMID, 0) == -1) {
        perror("msgctl");
        return 1;
    }

    return 0;
};