

const portaUsb = "COM4"

const serialport = require('serialport')

let SerialPort = serialport.SerialPort

var myPort = new SerialPort(portaUsb, {
    baudRate: 9600 ,
    parser: serialport.parsers.readline('\n')
});

myPort.on("open",(err)=>{
    console.log("Abriu connection com Arduino")
})

myPort.on("data",(data)=>{

    console.log(`Arduino enviando: ${data}`);

    var buf = Buffer.from("wagner", 'utf8');

    myPort.write(buf);

})