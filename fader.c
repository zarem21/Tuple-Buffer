/*
* This file contains the code for fader function and unit tests in
* emulation environment.
*/
#include "fader.h"
#include <stdio.h>
#include <stdint.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#ifdef EMULATION_TEST
	/* Shamelessly stolen from cppreference.doc variadic functions*/
	#include <stdarg.h>

	void ESP_LOGI(const char *tag, const char *fmt, ...) {
		va_list args;

		va_start(args, fmt);
		printf("ESP_LOGI %s: ", tag);
		vprintf(fmt, args);
		printf("\n");
	}

	extern void fade_led(uint32_t, int);
#else

#include "esp_log.h"

#endif


uint32_t ledTargetInt = 0;
int ledFadeTime = 0;


void fader(int bufLen, const char *rx_buf) {
	ESP_LOGI("Fader", "Called fader: >> rx_buf: %s", rx_buf);
	static char tuple[MAX_TUPLE_LEN + 1]; // space for zero terminator
	static int pos = 0;

	// Process the tuple from the rx_buffer
	for (int i = 0; i < bufLen; i++) {
		if (rx_buf[i] == '{') {
			pos = 0;
			tuple[pos++] = rx_buf[i];
		} else if (pos < MAX_TUPLE_LEN) {
			tuple[pos++] = rx_buf[i];
		} else {
			// Invalidate entire message that is too long for contract
			// To avoid buffer overflow
			pos = 0;
		}

		tuple[pos] = '\0';
		ESP_LOGI("Fader", "Tuple: \'%s\'       pos: %d,    bufLen: %d", tuple, pos, bufLen);


		if (pos > 0 && (tuple[pos - 1] == '}')) {
			unsigned int offON;
			unsigned int maxInt;
			unsigned int fadeTime;
			int c; // Return value from sscanf

			assert((unsigned long) pos < (sizeof(tuple) / sizeof(tuple[0])));

			c = sscanf(tuple, "{%u,%u,%u}", &offON, &maxInt, &fadeTime);
			ESP_LOGI("Fader", "Tuple: \'%s\'", tuple);

			if (c == 3 && (maxInt < MAX_INTENSITY) && (fadeTime < MAX_FADE_TIME)) {
				ESP_LOGI("Fader", "Message Interpreted into command: maxInt: %d, fadeTime: %d", maxInt, fadeTime);

#ifdef EMULATION_TEST
				fade_led((uint32_t)(maxInt & MAX_INTENSITY_MASK), fadeTime);
#else
				if (offON == 1) {
					ledTargetInt = maxInt;
					ledFadeTime = fadeTime;
				} else {
					ledTargetInt = 0;
					ledFadeTime = fadeTime;
				}
#endif
			}
			pos = 0;
			tuple[pos] = 0;
		}
	}
}

#ifdef EMULATION_TEST
#include <string.h>

bool result = 0;

void fade_led(uint32_t target, int fade_time) {
	printf("\nfade_led(%u,%u)\n", target, fade_time);
	result = 1;
}

void test(const char *rx_buf) {
	int bufLen = strlen(rx_buf);

	int result = 0;

	for (int i = 0; i < bufLen; i++) {
		putchar(rx_buf[i]);
	}

	fader(bufLen, rx_buf);

	if (result) {
		printf(" SUCCESS\n");
	}
}


int main(void) {
	const char *arr[] = {
        "{0,10,99}",
        "{1,13,98}",
        "{1,",
        "24",
        ",50}",
        "{{{{"
        "{1,",
        "84",
        ",30}",
        "}}}}"
        "{0,",
        "86",
        ",80}\r\n",
        "{a,bcde,fgegeee}12345456575665643",
        "gghgyyehhdtyye",
        "{1,13,98}12",
        "4566{1,323,98}566{1,343,48}"	
	};

	for (unsigned long i = 0; i < sizeof(arr)/sizeof(arr[0]); i++) {
		test(arr[i]);
	}

	return 0;
}
#endif /* EMULATION_TEST */