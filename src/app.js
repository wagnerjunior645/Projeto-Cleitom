

// Setando os tipos de dados para área de login
const usuariosLogin = [
    { usuario: "admim", senha: "admim", type: "admim" },
    { usuario: "aluno", senha: "aluno", type: "aluno" }
];

const privateKey = "123456789"
// fim Setando os tipos de dados



const cookieParser = require('cookie-parser')
const express = require("express");
const jwt = require('jsonwebtoken');
const app = express();
const bodyParser = require('body-parser');

let serialport = require("serialport");
let SerialPort = serialport.SerialPort;

let portaUSB = "COM3";
let baudrate = 9600;
let myPort = new SerialPort(portaUSB, {
    baudrate: baudrate,
    parser: serialport.parsers.readline("\n")
});

myPort.on("open", function () {
    console.log("\nARDUINO CONECTADO");
    console.log(`Porta USB: ${portaUSB}`);
    console.log(`Velocidade Porta Serial: ${9600}\n`);
})

const os = require('os');
let cpuCount = os.cpus().length;
console.log(`Número de CPU: ${cpuCount}`);

//Config EXPRESS

app.use(express.static(__dirname + "/node_modules"));
app.use(express.static(__dirname + "/assets"));

app.use(cookieParser());

app.use(bodyParser.json()); // for parsing application/json
app.use(bodyParser.urlencoded({ extended: true })); // for parsing application/x-www-form-urlencoded

//Fim Config EXPRESS

const http = require("http").Server(app);
const io = require("socket.io")(http);



//Rotas

app.get("/", (req, res) => {
    res.redirect("/login");
});

app.get("/login", (req, res) => {
    res.sendFile(__dirname + "/views/login.html")
});

app.post("/login", (req, res) => {

    let body = req.body;
    let find = [];

    if (body) {
        find = usuariosLogin.filter((value) => {
            return value.usuario == body.user && value.senha == body.password;
        })
    }

    if (body && find.length != 0) {
        jwt.sign({ user: find[0].usuario, type: find[0].type }, privateKey, (err, token) => {
            if (err) {
                return res.sendStatus(500);
            }
            return res.send(token);
        });
    } else {
        return res.sendStatus(401);
    }

    //401 usuario nao autenticado
    //403 usuario nao autorizado

})

app.use("/home", (req, res, next) => {

    cookies = req.cookies;

    token = cookies.token;

    if (cookies) {

        jwt.verify(token, privateKey, function (err, result) {
            if (err) {
                if (err.message == "invalid signature") {
                    res.redirect("/login");
                } else {
                    return res.sendStatus(500);
                }
            }

            next();

        });

    } else {
        return res.sendStatus(401);
    }

});

app.get("/home", (req, res) => {
    res.sendFile(__dirname + "/views/index.html")
});

app.get("/testando", (req, res) => {
    res.sendFile(__dirname + "/views/testando.html")
});

//Fim Rotas



io.emit('some event', { for: 'everyone' });

io.on('connection', function (socket) {

    console.log('Usuario Conectado ao Socket');

    /* myPort.on("data", function (data) {
        socket.broadcast.emit("msg", data);
    }); */

    socket.on('msg', function (inputMsg) {

        let token = "";
        let msg = "";

        try {
            jsonMsg = JSON.parse(inputMsg);
        } catch (err) {
            console.log("Error");
            return;
        }

        token = jsonMsg.token;
        msg = jsonMsg.msg;

        jwt.verify(token, privateKey, (err) => {
            if (err) {
                console.log(`Error ao decodificar o token`);
                return;
            }

            console.log(`Enviando para o Arduino: ${msg}`);

            myPort.write(`${msg}$`);

        })

    });

    myPort.on("data", function (data) {
        console.log(data);
        console.log("\n")
        socket.broadcast.emit("arduino", data);
    });

    //Canal usado apenas para comunicacao com o arduino
    //para escutar o arduino
    /* socket.on('arduino', function () {

        myPort.on("data", function (data) {
            socket.broadcast.emit("msg", data);
        });

    }); */

    socket.on('disconnect', function () {
        console.log('user disconnected');
    });

});

// STARTANDO O SERVER

const porta = 4200

http.listen(porta, function () {
    console.log(`Server On Rodando na porta ${porta}`);
});

//let numeroStance = process.env.NODE_APP_INSTANCE | 1;
//console.log(`NODE STANCE: ${process.env.NODE_APP_INSTANCE} + ${numeroStance}`);