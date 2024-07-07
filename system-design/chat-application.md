# Chat Application System Design

This problem covers the following  system design problems:
1. Messenger
2. Whatsapp
3. Telegram

## Design Scope

1. We should have 1:1 and group chats
2. Group chat limit is 100
3. DAU is 50 million users
4. Support sending documents and other media as well
5. Online indicators like active and last seen
6. end-to-end encryption
7. Chat history storage
8. push notifications

## High level design

### Chat service

Each client doesn't communicate with each other. Client sends the message to a chat service which relays the message to the correct recipient.

**Choosing the right protocol**

- HTTP protocol could be used on the client side because it is the client which will initiate the message and send the message to the server. The client can include a "keep-alive" header to keep the connection open and avoid addition TCP handshakes.
- Receiver side cannot use HTTP because it is a client-initiated protocol.
- Polling: The client checks with the server if any messages are available. Polling could be costly depending on the frequency
- Long polling: The client opens a long polling connection with the server and keeps the connection open until new message is received or timeout occurs. There are a few drawbacks
  - 
