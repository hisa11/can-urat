#include "mbed.h"

// シリアル通信設定
BufferedSerial serial(USBTX, USBRX, 115200);
Thread sendThread;    // 送信スレッド
Thread receiveThread; // 受信スレッド

// 送信用データ（int 配列）
int pwm[4] = {100, 200, 300, 400};

// 受信用データ（int 配列）
int pwm_recv[4] = {0, 0, 0, 0};

// 送信処理
void sendArray() {
    while (true) {
        serial.write(reinterpret_cast<const int *>(pwm), sizeof(pwm));
        ThisThread::sleep_for(1000ms); // 1秒ごとに送信
    }
}

// 受信処理
void receiveArray() {
    while (true) {
        if (serial.readable()) {
            ssize_t len = serial.read(reinterpret_cast<int *>(pwm_recv), sizeof(pwm_recv));
            if (len == sizeof(pwm_recv)) {
                // 受信データの表示
                printf("Received: %d %d %d %d\n", pwm_recv[0], pwm_recv[1], pwm_recv[2], pwm_recv[3]);
            }
        }
        ThisThread::sleep_for(100ms);
    }
}

int main() {
    // printf を USB シリアルにリダイレクト
    FILE *stream = fdopen(&serial, "w");
    if (stream) {
        stdout = stream;
    }

    // スレッド起動
    sendThread.start(sendArray);
    receiveThread.start(receiveArray);

    while (true) {
        ThisThread::sleep_for(1000ms); // メインスレッドを待機
    }
}
