/*
* Fader function receives a character buffer containing snippets (partial or full)
* of tuples in string form. Fader interprets the tuple and sets the variables ledTargetInt
* and ledFadeTime. 
* 
* Incomplete snippets are buffered. Incorrect tuples are discarded. Buffer is never overrun.
* Upon seeing the right-brace '}', The tuple is checked for correct and usable form.
*
* Tuple is: '{' offON ',' maxInt ',' fadeTime '}'
*
*	offON := 0 | 1;
*	maxInt := 0 - 8191; 	// 13-bit PWM duty resolution
*	fadeTime := 0 - 10000;	// 10-thousand milliseconds
*
* TODO:
* - Respond to 'h' and send tuple description 
*/


#include <stdint.h>

/*
* Settings for fader method
*/
enum {
	CNT_INTENSITY_BITS = 13,
	MAX_INTENSITY = (1u << CNT_INTENSITY_BITS),
	MAX_INTENSITY_MASK = (MAX_INTENSITY - 1u),
	MAX_FADE_TIME = 10000,
	MAX_TUPLE_LEN = 
		(1 // left-brace '{'
		+ 1 //offON
		+ 1 // ','
		+ 4 // 13-bit PWM
		+ 1 // ','
		+ 5 // 10-thousand milliseconds
		+ 1 // right-brace '}'
		)
};


/*
* - Does not need the complete tuple to work correctly
* - Expect character to arrive in the correct sequence
*/
extern void fader(int bufLen, const char *rx_buf);


/*
* Fader manipulates the following variables.  
*/
extern uint32_t ledTargetInt;
extern int ledFadeTime;


