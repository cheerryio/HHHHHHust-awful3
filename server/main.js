process.env.DEBUG = 'socket.io*';

const server = require('http').createServer();
const io = require('socket.io')(server);
io.on('connection', (client) => {
    console.log('connect 1243t54y6uyi');
    client.on('event', (data) => {
        console.log(1234);
    });
    client.on('disconnect', () => {
        console.log('213245tytj');
        /* … */
    });
});
server.listen(3000);
