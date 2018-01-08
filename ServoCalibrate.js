var five = require("johnny-five");
var board = new five.Board();

board.on("ready", function() {
  var servo = new five.Servo.Continuous({pin:8});

  servo.cw(1);

});
