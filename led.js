'use strict';

const wpn = require('./wiringPiNode');
const gpioLed = wpn.gpioLed();
const joystick = wpn.joystick();

const t = Number.parseInt(process.argv[2], 10);
const verbose = process.argv[3] == '-v';

let timeout = Number.isNaN(t) ? 1000 : t;
const DEBUG = verbose;

let isOn = false;
let last = Date.now();
let i;

const blink = () => {
  if (isOn) {
    gpioLed.off();
  } else {
    gpioLed.on();
  }

  const now = Date.now();
  DEBUG && console.log(now - last);
  DEBUG && (last = now);

  isOn = gpioLed.isOn();

  DEBUG && console.log(isOn);
};

i = setInterval(blink, timeout / 2);

const random = (min, max) => {
  return Math.floor(Math.random()*(max-min+1)+min);
};

const onClick = () => {
  DEBUG && console.log('onClick');

  timeout = random(100, 1000);
  DEBUG && console.log({ timeout });

  clearTimeout(i);
  i = setInterval(blink, timeout / 2);
}

const onHold = () => {
  DEBUG && console.log('onHold');

  clearTimeout(i);
  joystick.stopWatch();
};

joystick.watch(onClick, onHold);
