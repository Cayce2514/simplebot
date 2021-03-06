'use strict';

var VirtualSerialPort = require('udp-serial').SerialPort;
var firmata = require('firmata');
var five = require("johnny-five");

//create the udp serialport and specify the host and port to connect to
var sp = new VirtualSerialPort({
  host: '192.168.4.1',
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
        console.log('five ready');
        //Full Johnny-Five support here:
        console.log("Control the bot with the w, a, s, and d keys, the space bar to stop, q to exit.");

        //var led = new five.Led(13);
        //led.blink();
        var left_wheel = new five.Servo.Continuous(8);
        var right_wheel = new five.Servo.Continuous(9);

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
            process.exit();

          } else if (key.name == 'w') {

            console.log("Forward");
            left_wheel.cw();
            right_wheel.ccw();

          } else if (key.name == 's') {

            console.log("Backward");
            left_wheel.ccw();
            right_wheel.cw();

          } else if (key.name == 'd') {

            console.log("Right");
            left_wheel.cw();
            right_wheel.stop();

          } else if (key.name == 'a') {

            console.log("Left");
            left_wheel.stop();
            right_wheel.ccw();

          } else if (key.name == 'space') {

            console.log("Stopping");
            left_wheel.to(90);
            right_wheel.to(90);
          }
       });
    });
});
