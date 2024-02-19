#include <windows.h>
#include <stdio.h>

#define MESSAGE_SIZE 61
#define BUFFER_SIZE (MESSAGE_SIZE - 1) * 2
#define LUX_THRESHOLD 100

static const char* COM_PORT = "COM4";
static HANDLE hCom = INVALID_HANDLE_VALUE;
static DCB dcb = {0};
static char messageBuffer[BUFFER_SIZE];
static unsigned long messageLength;
static char currentMessage[MESSAGE_SIZE];

int openHandle();
int closeHandle();
void receiveMessage();
void debugPrintMessage();
int extractMessage();
int validateCurrentMessage();
void outputData();


int main(int argc, char* argv[])
{
    openHandle();

    dcb.DCBlength = sizeof(DCB);

    if (!GetCommState(hCom, &dcb))
    {
        printf("Failed to get COM State (%d)\r\nexiting\r\n", GetLastError());
        exit(1);
    }

    dcb.BaudRate = 11520;
    dcb.ByteSize = 8;
    dcb.Parity = PARITY_EVEN;
    dcb.StopBits = ONESTOPBIT;

    if (!GetCommState(hCom, &dcb))
    {
        printf("Failed to get COM State (%d)\r\nexiting\r\n", GetLastError());
        exit(1);
    }

    printf("Serial Port %s configured\r\n", COM_PORT);

    int i = atoi(argv[1]);
    while (i != 0)
    {
        if (i > 0) i--;
        receiveMessage();
        printf("Reading %d (%d):\r\n%s\r\n", i, messageLength, messageBuffer);

        extractMessage();
        if (validateCurrentMessage())
        {
            printf("Found Message:\r\n%s\r\n", currentMessage);
            outputData();
        }
        else
        {
            printf("Invalid Message:\r\n%s\r\n", currentMessage);
        }

        printf("\r\n");

        sleep(1);
    }
    // debugPrintMessage();
    closeHandle();
}

int openHandle()
{
    printf("Opening handle\r\n");
    hCom = CreateFile(
        COM_PORT,
        GENERIC_READ,
        0,
        0,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        0
    );

    if (hCom == INVALID_HANDLE_VALUE)
    {
        printf("CreateFile failed (%d)\r\nexiting\r\n", GetLastError());
        CloseHandle(hCom);
        exit(1);
    }
}

int closeHandle()
{
    printf("Closing handle\r\n");
    CloseHandle(hCom);
}

void receiveMessage()
{
    if (!ReadFile(
        hCom,
        (void*) messageBuffer,
        BUFFER_SIZE,
        &messageLength,
        NULL
    ))
    {
        printf("ReadFile failed (%d)", GetLastError());
        exit(1);
    }
}

void debugPrintMessage()
{

    printf("Received message of length %d: %s\r\n", messageLength, messageBuffer);
    printf("Raw data:\r\n");

    sleep(1);

    int i = 0;
    for (i = 0; i < messageLength; i++)
    {
        printf(":%d-%c", messageBuffer[i], messageBuffer[i]);
    }
}

int extractMessage()
{
    unsigned long i = 0;
    unsigned long messageIndex = 0;
    for (i = 0; i < BUFFER_SIZE; i++)
    {
        if (messageIndex > 0 && messageIndex < MESSAGE_SIZE - 1)
        {
            currentMessage[messageIndex] = messageBuffer[i];
            messageIndex++;
        }
        else if (messageBuffer[i] == '$')
        {
            currentMessage[messageIndex] = messageBuffer[i];
            messageIndex = 1;
        }

        if (messageIndex == MESSAGE_SIZE - 1)
        {
            currentMessage[messageIndex] == 0;
            messageIndex++;
        }
    }

    return messageIndex > 0;
}

int validateCurrentMessage()
{
    if (currentMessage[0] == '$' &&
        currentMessage[1] == ';' &&
        currentMessage[2] == 'l' &&
        currentMessage[3] == 'u' &&
        currentMessage[4] == 'x' &&
        currentMessage[5] == ':' &&
        currentMessage[10] == ';' &&
        currentMessage[11] == 'h' &&
        currentMessage[12] == 'u' &&
        currentMessage[13] == 'm' &&
        currentMessage[14] == ':' &&
        currentMessage[17] == '.' &&
        currentMessage[22] == ';' &&
        currentMessage[23] == 't' &&
        currentMessage[24] == 'm' &&
        currentMessage[25] == 'p' &&
        currentMessage[26] == ':' &&
        currentMessage[29] == '.' &&
        currentMessage[34] == ';' &&
        currentMessage[35] == 'g' &&
        currentMessage[36] == 'y' &&
        currentMessage[37] == 'r' &&
        currentMessage[38] == ':' &&
        currentMessage[41] == '.' &&
        currentMessage[46] == ';' &&
        currentMessage[47] == 'a' &&
        currentMessage[48] == 'c' &&
        currentMessage[49] == 'c' &&
        currentMessage[50] == ':' &&
        currentMessage[53] == '.' &&
        currentMessage[58] == ';' &&
        currentMessage[59] == '!' &&
        currentMessage[60] == 0)
    {
        return 1;
    }

    return 0;
}

void outputData()
{
    HANDLE hFile = CreateFile(
        "data.txt",
        GENERIC_WRITE,
        1,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    unsigned long written = 0;
    if (!WriteFile(
        hFile,
        (void*) currentMessage,
        MESSAGE_SIZE - 1,
        &written,
        NULL
    ))
    {
        printf("WriteFile failed (%d)", GetLastError());
        exit(1);
    }

    CloseHandle(hFile);
}
