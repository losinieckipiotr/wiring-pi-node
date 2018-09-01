'use strict';

const wpn = require('./wiringPiNode');
const gpioLed = wpn.gpioLed();
const joystick = wpn.joystick();
const buzzer = wpn.buzzer();
const bmp180 = wpn.bmp180();

const t = Number.parseInt(process.argv[2], 10);
const verbose = process.argv[3] == '-v';

let timeout = Number.isNaN(t) ? 1000 : t;
const DEBUG = verbose;

const measure = () => {
  const now = Date.now();
  bmp180.measure((t, p) => {
    const after = Date.now();
    DEBUG && console.log(`T: ${t.toFixed(2)}`);
    DEBUG && console.log(`P: ${p.toFixed(2)}`);
    DEBUG && console.log(`measure t: ${after - now} ms`);
  });
};

let ledIsOn = false;
let buzzerIsOn = false;
let last = Date.now();
let i;

const blink = () => {
  if (ledIsOn) {
    gpioLed.off();
  } else {
    gpioLed.on();
  }

  const now = Date.now();
  DEBUG && console.log(now - last);
  DEBUG && (last = now);

  ledIsOn = gpioLed.isOn();
  buzzerIsOn = buzzer.isOn();

  DEBUG && console.log(ledIsOn);

  measure();
};

i = setInterval(blink, timeout / 2);

const random = (min, max) => {
  return Math.floor(Math.random()*(max-min+1)+min);
};

const onClick = () => {
  DEBUG && console.log('onClick');

  timeout = random(100, 1000);
  DEBUG && console.log({ timeout });

  if (buzzerIsOn) {
    buzzer.off();
  } else {
    buzzer.on();
  }

  buzzerIsOn = buzzer.isOn();

  clearTimeout(i);
  i = setInterval(blink, timeout / 2);
}

const onHold = () => {
  DEBUG && console.log('onHold');

  clearTimeout(i);
  buzzer.off();
  joystick.stopWatch();
};

joystick.watch(onClick, onHold);
