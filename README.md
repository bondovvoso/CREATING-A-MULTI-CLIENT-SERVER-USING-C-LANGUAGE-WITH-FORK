# MEMBUAT SERVER MULTI-KLIENT MENGGUNAKAN BAHASA C DENGAN FORK  
### *CARA MENJALANKAN APLIKASI*  

#### 1. Jalankan Server TCP yang Dibuat dalam C  
   - Kompilasi program server menggunakan perintah:  
   ```bash  
   gcc server.c -o server  
   ./server  
   ```  

#### 2. Jalankan WebSocket Bridge  
   - Eksekusi file bridge WebSocket menggunakan Node.js:  
   ```bash  
   node websocket_bridge.js  
   ```  

#### 3. Buka Client HTML di Browser Chrome  
   - Klik dua kali file `index.html` yang telah dibuat untuk membukanya di browser Chrome.  

#### 4. Mulai Berinteraksi  
   - Masukkan nama kamu saat diminta melalui browser.  
   - Kirim dan terima pesan melalui server serta berinteraksi dengan klien lain menggunakan jembatan WebSocket.  
