'use strict';
var connection = null;
var clientID = 0;
var onlineUser = {};
var publicHistory = [];
var currentRenderTarget = 'public';
var myUserName = '';
var myHostname = window.location.hostname;

if (!myHostname) {
    myHostname = 'localhost';
}

/**
 * userName
 * itemElement
 * history
 * rtcpeerconnection
 * datachannel
 */
function setUserName() {
    console.log('set username');
    myUserName = document.getElementById('name').value;
    var msg = {
        name: document.getElementById('name').value,
        date: Date.now(),
        id: clientID,
        type: 'username',
    };
    connection.send(JSON.stringify(msg));
}
function parseMessageEvent(msg) {
    console.log('parse message event', msg);
    var connection;
    var desc;
    switch (msg.type) {
        case 'id':
            clientID = msg.id;
            setUserName();
            break;
        case 'rejectusername':
            myUserName = msg.name;
            document.getElementById('name').value = myUserName;
            break;
        case 'userlist':
            handleUserlistMsg(msg);
            break;
        case 'chat-offer':
            if (!onlineUser[msg.name].rtcpeerconnection) {
                createPeerConnection(msg.name);
            }
            connection = onlineUser[msg.name].rtcpeerconnection;
            if (!msg.sdp) {
                return;
            }
            desc = new RTCSessionDescription(msg.sdp);
            connection.setRemoteDescription(desc).catch((e) => {
                console.error(e);
                connection.close();
                onlineUser[msg.name].rtcpeerconnection = null;
            });

            connection.setRemoteDescription(desc);
            connection
                .createAnswer()
                .then((answer) => connection.setLocalDescription(answer))
                .then(() => {
                    sendToServer({
                        name: myUserName,
                        target: msg.name,
                        type: 'chat-answer',
                        sdp: connection.localDescription,
                    });
                })
                .catch((e) => {
                    console.error(e);
                    connection.close();
                    onlineUser[msg.name].rtcpeerconnection = null;
                });
            break;
        case 'chat-answer':
            connection = onlineUser[msg.name].rtcpeerconnection;
            desc = new RTCSessionDescription(msg.sdp);
            connection.setRemoteDescription(desc).catch((e) => {
                console.error(e);
                connection.close();
                onlineUser[msg.name].rtcpeerconnection = null;
            });
            break;
        case 'new-ice-candidate':
            connection = onlineUser[msg.name].rtcpeerconnection;
            var candidate = new RTCIceCandidate(msg.candidate);
            connection.addIceCandidate(candidate).catch((e) => {
                console.error(e);
                connection.close();
                onlineUser[msg.name].rtcpeerconnection = null;
            });
            break;
    }
}

function parseMessageText(msg) {
    console.log('parse message text', msg);
    var text = '';
    var time = new Date(msg.date);
    var timeStr = time.toLocaleTimeString();

    switch (msg.type) {
        case 'username':
            text = '<b>User <em>' + msg.name + '</em> signed in at ' + timeStr + '</b><br>';
            break;
        case 'message':
            text = '(' + timeStr + ') <b>' + msg.name + '</b>: ' + msg.text + '<br>';
            break;
        case 'rejectusername':
            text =
                '<b>Your username has been set to <em>' +
                msg.name +
                '</em> because the name you chose is in use.</b><br>';
            break;
    }
    return text;
}

function sendToServer(msg) {
    console.log('send to server', msg);
    connection.send(JSON.stringify(msg));
}

function sendToUser(msg, target) {
    console.log('send to user', msg);
    onlineUser[target].datachannel.send(JSON.stringify(msg));
}

function createPeerConnection(target) {
    console.log('create peer connection', target);
    var connection = new RTCPeerConnection({
        iceServers: [
            // Information about ICE servers - Use your own!
            {
                urls: 'turn:' + myHostname, // A TURN server
                username: 'webrtc',
                credential: 'turnserver',
            },
        ],
    });
    onlineUser[target].rtcpeerconnection = connection;
    connection.onicecandidate = (evt) => {
        if (evt.candidate) {
            sendToServer({
                type: 'new-ice-candidate',
                target: target,
                name: myUserName,
                candidate: evt.candidate,
            });
        }
    };
    connection.onnegotiationneeded = () => {
        connection
            .createOffer()
            .then((offer) => connection.setLocalDescription(offer))
            .then((offer) => {
                sendToServer({
                    name: myUserName,
                    target: target,
                    type: 'chat-offer',
                    sdp: connection.localDescription,
                });
            })
            .catch((e) => {
                connection.close();
                onlineUser[target].rtcpeerconnection = null;
                console.error(e);
            });
    };
    connection.ondatachannel = (evt) => {
        var datachannel = evt.channel;
        createDataChannel_self(target, datachannel);
        onlineUser[target].datachannel = datachannel;
    };
}

function createDataChannel_self(target, datachannel) {
    console.log('create data channel', target, datachannel);
    datachannel.addEventListener('open', (evt) => {
        console.log('datachannel create');
    });
    datachannel.addEventListener('close', (evt) => {
        console.log('datachannel close');
    });
    datachannel.onmessage = (evt) => {
        var msg = JSON.parse(evt.data);
        var chatBox = document.getElementById('chatbox');
        parseMessageEvent(msg);
        var text = parseMessageText(msg);
        if (text.length) {
            onlineUser[target].history.push(msg);
            if (currentRenderTarget === msg.from) {
                chatBox.innerHTML += text;
                chatBox.scrollTop = chatBox.scrollHeight - chatBox.clientHeight;
            }
        }
    };
    onlineUser[target].datachannel = datachannel;
}

function renderChatDiv(history, target) {
    console.log('render chat div', history, target);
    if (target === currentRenderTarget) {
        return;
    }
    var chatBox = document.getElementById('chatbox');
    var text = '';
    history.forEach((msg) => {
        text += parseMessageText(msg);
    });
    chatBox.innerHTML = text;
    chatBox.scrollTop = chatBox.scrollHeight - chatBox.clientHeight;
    currentRenderTarget = target;
}

function handleNameClick(evt) {
    var target = evt.target.textContent;
    console.log('handleNameClick', target);
    if (target === 'public') {
        renderChatDiv(publicHistory, 'public');
        return;
    }
    if (target === currentRenderTarget || target === myUserName) {
        return;
    }
    if (!onlineUser[target].datachannel) {
        createPeerConnection(target);
        createDataChannel_self(target, onlineUser[target].rtcpeerconnection.createDataChannel('chat-channel'));
    }
    renderChatDiv(onlineUser[target].history, target);
}

function handleUserlistMsg(msg) {
    var listElem = document.getElementById('userlistbox');
    console.log('handle user list message', msg);
    msg.users.forEach((username) => {
        if (username in onlineUser) {
            return;
        } else {
            onlineUser[username] = {
                userName: username,
                itemElemnt: (() => {
                    var item = document.createElement('li');
                    item.appendChild(document.createTextNode(username));
                    item.addEventListener('click', handleNameClick, false);
                    listElem.appendChild(item);
                    return item;
                })(),
                history: [],
                rtcpeerconnection: null,
                datachannel: null,
            };
        }
    });
    Object.keys(onlineUser).forEach((user) => {
        if (msg.users.includes(user)) {
            return;
        } else {
            if (onlineUser[user].itemElemnt) {
                //console.log(user, msg.users, user in msg.users);
                onlineUser[user].itemElemnt.remove();
            }
            if (onlineUser[user].rtcpeerconnection) {
                onlineUser[user].rtcpeerconnection.close();
            }
            if (onlineUser[user].datachannel) {
                onlineUser[user].datachannel.close();
            }
            delete onlineUser[user];
        }
    });
}

function connect() {
    console.log('connect to server');
    var serverUrl;
    var scheme = 'ws';

    // If this is an HTTPS connection, we have to use a secure WebSocket
    // connection too, so add another "s" to the scheme.

    if (document.location.protocol === 'https:') {
        scheme += 's';
    }

    serverUrl = scheme + '://' + document.location.hostname + ':6503';

    connection = new WebSocket(serverUrl, 'json');

    connection.onopen = function (evt) {
        document.getElementById('text').disabled = false;
        document.getElementById('send').disabled = false;
    };

    connection.onmessage = function (evt) {
        var chatBox = document.getElementById('chatbox');
        var msg = JSON.parse(evt.data);
        parseMessageEvent(msg);
        var text = parseMessageText(msg);
        if (text.length) {
            publicHistory.push(msg);
            if (currentRenderTarget === 'public') {
                chatBox.innerHTML += text;
                chatBox.scrollTop = chatBox.scrollHeight - chatBox.clientHeight;
            }
        }
    };
}

function handleSendButton() {
    console.log('send chat message', currentRenderTarget);
    if (currentRenderTarget === 'public') {
        sendToServer({
            text: document.getElementById('text').value,
            type: 'message',
            id: clientID,
            date: Date.now(),
        });
    } else {
        var msg = {
            text: document.getElementById('text').value,
            type: 'message',
            id: clientID,
            name: myUserName,
            date: Date.now(),
        };
        sendToUser(msg, currentRenderTarget);
        var chatBox = document.getElementById('chatbox');
        var text = parseMessageText(msg);
        if (text.length) {
            onlineUser[currentRenderTarget].history.push(msg);
            chatBox.innerHTML += text;
            chatBox.scrollTop = chatBox.scrollHeight - chatBox.clientHeight;
        }
    }
}

function handleKey(evt) {
    if (evt.keyCode === 13 || evt.keyCode === 14) {
        if (!document.getElementById('send').disabled) {
            handleSendButton();
            evt.preventDefault();
        }
    }
}

window.addEventListener('load', () => {
    var listElem = document.getElementById('userlistbox');
    var item = document.createElement('li');
    item.appendChild(document.createTextNode('public'));
    item.addEventListener('click', handleNameClick, false);
    listElem.appendChild(item);
});
window.onlineUser = onlineUser;
