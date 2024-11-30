#include "ap.h"
#include "driver/led.h"
#include "driver/button.h"
#include "driver/pwm8.h"
#include "driver/uart0.h"
#include <avr/interrupt.h>
#include <driver/pwm16.h>

void apInit() {
    UART0_init(9600); // UART 초기화 (9600bps)
    sei();            // 전역 인터럽트 활성화
    UART0_transmit_string("UART Echo Ready\r\n");
}

void apMain() {
    while (1) {
        // 메인 루프는 인터럽트에서 처리
    }
}
