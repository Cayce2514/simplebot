'use strict';

var VirtualSerialPort = require('udp-serial').SerialPort;
var firmata = require('firmata');
var five = require("johnny-five");

//create the udp serialport and specify the host and port to connect to
var sp = new VirtualSerialPort({

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

var board = new five.Board({io: io, repl: true});
//    var board = new five.Board({
//         port: "COM8"
//		});

board.on('ready', function(){
  var proximity = new five.Proximity({
    controller: "HCSR04",
    pin: 2
  });
  console.log('five ready');
  //Full Johnny-Five support here:
  console.log("Control the bot with the w, a, s, and d keys, the space bar to stop, q to exit.");

  var left_wheel = new five.Servo.Continuous(9);
  var right_wheel = new five.Servo.Continuous(8);

function forward() {
  left_wheel.cw();
  right_wheel.ccw();
}

function reverse() {
  left_wheel.ccw();
  right_wheel.cw();
}

function left() {
  left_wheel.stop();
  right_wheel.ccw();
}

function right() {
  left_wheel.cw();
  right_wheel.stop();
}

function stop() {
  left_wheel.stop();
  right_wheel.stop();
}

// wander unless object is close
  proximity.on("data", function() {
      //console.log("\033[2J");
      //console.log("Proximity: ");
      //console.log("  cm  : ", this.cm);
      //console.log("  in  : ", this.in);
      //console.log("-----------------");
    if (this.cm > 15) {
      console.log("  cm  : ", this.cm);
      console.log("All Clear - Moving...")
      forward();
    } else {
      console.log("  cm  : ", this.cm);
      console.log("WHOAH!!!  Found an obstruction! Stopping!")
      stop();
      setTimeout(reverse, 3000);
      setTimeout(left, 3000);
      setTimeout(forward, 1000);

      //process.exit();
    }
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
            // clear the screen before we write something new.
            //console.log("\033[2J");
            console.log("Quitting");
            left_wheel.stop();
            right_wheel.stop();
            process.exit();

          }
        });
    });
});
