
//프로그램 보낼때는 보드의 isp모드
//그다음 uarto모드
// sudo apt install moserial
//ls /dev/tty*
//sudo chmod 777 /dev/ttyUSB0
//ls -l /dev/ttyUSB0
#include "uart0.h"

#define MAX_INPUT_LENGTH 100 // 입력 문자열 최대 길이

volatile char inputBuffer[MAX_INPUT_LENGTH]; // 입력 버퍼
volatile uint8_t input_index = 0;            // 현재 입력 위치 (변수명 수정)

void UART0_init(uint16_t baudrate) {
    uint16_t ubrr_value = F_CPU / 16 / baudrate - 1; // UBRR 값 계산

    // UART 속도 설정
    UBRR0H = (uint8_t)(ubrr_value >> 8); // 상위 바이트
    UBRR0L = (uint8_t)ubrr_value;        // 하위 바이트

    // 송신(TX) 및 수신(RX) 활성화, 인터럽트 활성화
    UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);

    // 비동기 모드, 8비트 데이터, 1 스톱 비트, 패리티 없음
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void UART0_transmit(char data) {
    // 송신 가능 대기
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data; // 데이터 전송
}

void UART0_transmit_string(const char *str) {
    while (*str) {
        UART0_transmit(*str++); // 문자열의 각 문자 송신
    }
}

// UART 수신 인터럽트 서비스 루틴
ISR(USART0_RX_vect) {
    char received = UDR0; // 수신된 데이터 읽기

    if (received == '\r' || received == '\n') {
    inputBuffer[input_index] = '\0';          // 문자열 종료
    if (input_index > 0) { // 입력된 문자열이 비어 있지 않은 경우에만 처리
        UART0_transmit_string("You Entered: ");
        UART0_transmit_string((const char *)inputBuffer); // 문자열 출력
        UART0_transmit_string("\r\n");
    }
    input_index = 0;                          // 버퍼 인덱스 초기화
    } else if (input_index < MAX_INPUT_LENGTH - 1) {
        inputBuffer[input_index++] = received;    // 문자 저장
    } else {
        UART0_transmit_string("Input Too Long!\r\n");
        input_index = 0;                          // 버퍼 초기화
    }
}

