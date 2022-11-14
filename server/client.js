const WebSocket = require('ws');

const ws = new WebSocket('ws://ssbr.dosowisko.net:8989/');

ws.on('open', function open() {
  //ws.send('something');

const readline = require('readline');

const rl = readline.createInterface({
  input: process.stdin,
  output: process.stdout
});

var ask = function() {
  rl.question('=> ', (answer) => {
    ws.send(answer);
    ask();
  });
};

ask();

});

ws.on('message', function incoming(data) {
  console.log(data);
});
