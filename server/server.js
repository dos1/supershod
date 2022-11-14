const WebSocket = require('ws');
const randomstring = require('randomstring');

const wss = new WebSocket.Server({ port: 8989 });

let ingame = false;

// Broadcast to all.
wss.broadcast = function broadcast(data) {
  wss.clients.forEach(function each(client) {
    if (client.readyState === WebSocket.OPEN) {
      client.send(data);
    }
  });
};


function updatePlayers() {
    let players = 0, stillplaying = 0, lastplayer = null;
    wss.clients.forEach(function each(client) {
      if (client.readyState === WebSocket.OPEN) {
        players++;
        if (client.data) { stillplaying++; lastplayer = client; }
        else { client.send("L"); }
      }
    });
    wss.broadcast('P' + players);
    wss.broadcast('S' + stillplaying);
    if (ingame && (stillplaying == 1) && lastplayer) {
      lastplayer.send('W');
    }
}

wss.on('connection', function connection(ws) {
console.log("connection");
  //wss.broadcast("connected");
  ws.data = !ingame;
  updatePlayers();

  ws.on('close', function() {
    updatePlayers();
  });

  ws.on('message', function incoming(data) {
console.log( data);
    // Broadcast to everyone.
    if (data.startsWith('B')) {
      wss.broadcast(data);
    } else if (data.startsWith('L')) {
      ws.data = false;
      updatePlayers();
    } else if (data.startsWith('R')) {
ws.data = false;
wss.broadcast('R');

      ingame = false;
      resetPlayers();
    } else if (data.startsWith('G')) {
      ingame = true;
ws.data = false;
wss.broadcast('G');
      updatePlayers();
    }

  });
});
