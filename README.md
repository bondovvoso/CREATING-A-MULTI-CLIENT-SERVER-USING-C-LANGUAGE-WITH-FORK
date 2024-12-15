# CREATING-A-MULTI-CLIENT-SERVER-USING-C-LANGUAGE-WITH-FORK
### *HOW TO Running the Application*

#### 1. *Launch the C TCP Server*
   ```
   gcc server.c -o server
   ./server
   ```
#### 2. Start the WebSocket Bridge
  ```
  node websocket_bridge.js
  ```
#### 3. Open the HTML Client in Chrome
-  Access index.html in Chrome using the file:// protocol or host it with a local web server, such as:
  ```
  python3 -m http.server
  ```
#### 4. Begin Chatting
-  Enter your name in the browser when prompted.
-  Send and receive messages through the server and interact with other clients via the WebSocket bridge.
