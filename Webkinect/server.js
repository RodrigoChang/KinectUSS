const express = require('express');
const http = require('http');
const zmq = require('zeromq');
const WebSocket = require('ws');

const app = express();
const server = http.createServer(app);
const wss = new zmq.Subscriber();
const wssWebSocket = new WebSocket.Server({ noServer: true });

wss.connect('tcp://0.0.0.0:3001');
wss.subscribe('');

server.on('upgrade', (request, socket, head) => {
    wssWebSocket.handleUpgrade(request, socket, head, (ws) => {
        wssWebSocket.emit('connection', ws, request);
    });
});

wssWebSocket.on('connection', (ws) => {
    console.log('Cliente WebSocket conectado');

    wss.receive().then(([topic, message]) => {
        // Envia el frame de video directamente al cliente WebSocket
        ws.send(message, { binary: true });  // Asegúrate de que el mensaje se envíe como binario
    }).catch((error) => {
        console.error('Error en la recepción de mensajes:', error);
    });
});

server.listen(3000, () => {
    console.log('Servidor escuchando en http://localhost:3000');
});
