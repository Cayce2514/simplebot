'use strict';
// Simplebot program that backs off when something is detected in front of the
// robot and waits until it's safe, then returns to original position 

const SENSOR_PIN = 2
const LEFT_PIN = 9
const RIGHT_PIN = 8
const LEFT_DEADBAND = [88, 88]
const RIGHT_DEADBAND = [92, 92]
const MIN_DISTANCE = 10
const SAFE_DISTANCE = 15
const WAIT = 1000
const INTERVAL = 10

const VirtualSerialPort = require('udp-serial').SerialPort;
const firmata = require('firmata');
const five = require("johnny-five");

//create the udp serialport and specify the host and port to connect to
const sp = new VirtualSerialPort({
// Bot1 - 192.168.1.54
//  host: '192.168.1.54',
  host: '192.168.1.54',
  type: 'udp4',
  port: 1025
});

//use the serial port to send a command to a remote firmata(arduino) device
var io = new firmata.Board(sp);
io.once('ready', function(){
    console.log('IO Ready');
    io.isReady = true;

const board = new five.Board({io: io, repl: true});


board.on('ready', function(){
  const left_wheel = new five.Servo({pin: LEFT_PIN, type: 'continuous', invert: true, deadband: LEFT_DEADBAND});
  const right_wheel = new five.Servo({pin: RIGHT_PIN, type: 'continuous', invert: false, deadband: RIGHT_DEADBAND});
  const proximity = new five.Proximity({
    controller: "HCSR04",
    pin: SENSOR_PIN
  });


  let backoff = 0
  let backingOff = false
  let waiting = 0
  let safe = true
  
  setInterval(() => {
    if (backingOff) {
      left_wheel.cw()
      right_wheel.cw()
      backoff = backoff + INTERVAL
      waiting = 0
    } else if (waiting < WAIT && safe) {
      waiting = waiting + INTERVAL
      left_wheel.stop()
      right_wheel.stop()
    } else if (backoff > 0 && safe) {
      left_wheel.ccw()
      right_wheel.ccw()
      backoff = backoff - INTERVAL
    } else {
      left_wheel.stop()
      right_wheel.stop()
    }
    // console.log(backoff, waiting, backingOff, safe)
  }, INTERVAL)


// wander unless object is close
  proximity.on("change", function() {
    //backingOff = (this.cm < MIN_DISTANCE)
    backingOff = (this.cm < MIN_DISTANCE)
    safe = (this.cm > SAFE_DISTANCE)
  });


        // Configure stdin for the keyboard controller
        process.stdin.resume();
        process.stdin.setEncoding("utf8");
        process.stdin.setRawMode(true);

        process.stdin.on("keypress", function(ch, key) {

          if (!key) {
            return;
          }

          if (key.name == "q") {

            console.log("Quitting");
            left_wheel.stop();
            right_wheel.stop();
            process.exit();

          }
        });
    });
    this.repl.inject({
      left_wheel: left_wheel, right_wheel: right_wheel, proximity: proximity
    })
})
