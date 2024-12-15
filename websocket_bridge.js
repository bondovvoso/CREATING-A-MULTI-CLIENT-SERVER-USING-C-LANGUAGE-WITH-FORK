// Mengimpor library yang dibutuhkan
const WebSocket = require('ws');
const net = require('net');

// Konfigurasi koneksi ke TCP server dan WebSocket server
const TCP_HOST = '127.0.0.1';
const TCP_PORT = 9090;
const WEBSOCKET_PORT = 9091;

// Membuat WebSocket server
const wsServer = new WebSocket.Server({ port: WEBSOCKET_PORT });

console.log(`WebSocket server berjalan di ws://localhost:${WEBSOCKET_PORT}`);

// Event ketika koneksi WebSocket baru diterima
wsServer.on('connection', (wsClient) => {
    console.log('Koneksi WebSocket baru diterima.');

    // Membuat koneksi ke TCP server
    const tcpClient = new net.Socket();
    tcpClient.connect(TCP_PORT, TCP_HOST, () => {
        console.log('Terhubung ke TCP server.');
    });

    // Meneruskan pesan dari WebSocket ke TCP server
    wsClient.on('message', (message) => {
        console.log(`Pesan dari WebSocket: ${message}`);
        tcpClient.write(message);
    });

    // Meneruskan pesan dari TCP server ke WebSocket
    tcpClient.on('data', (data) => {
        console.log(`Pesan dari TCP server: ${data}`);
        wsClient.send(data.toString());
    });

    // Menangani penutupan koneksi TCP
    tcpClient.on('close', () => {
        console.log('Koneksi ke TCP server terputus.');
        wsClient.close();
    });

    // Menangani penutupan koneksi WebSocket
    wsClient.on('close', () => {
        console.log('Koneksi WebSocket ditutup.');
        tcpClient.end();
    });
});
