# Tuple-Buffer

  Function receives a character buffer containing snippets (partial or full)
  of tuples in string form. Fader interprets the tuple and sets the variables ledTargetInt
  and ledFadeTime. 
  
  Incomplete snippets are buffered. Incorrect tuples are discarded. Buffer is never overrun.
  Upon seeing the right-brace '}', The tuple is checked for correct and usable form.
  
  Tuple is: '{' offON ',' maxInt ',' fadeTime '}'
  
  offON := 0 | 1;
  maxInt := 0 - 8191; 	// 13-bit PWM duty resolution
  fadeTime := 0 - 10000;	// 10-thousand milliseconds

  TODO:
  - Respond to 'h' and send tuple description 

This program can be used to control the intensity and fade time of LEDs.
