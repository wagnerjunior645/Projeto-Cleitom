

const portaUsb = "COM4"

const serialport = require('serialport')

let SerialPort = serialport.SerialPort

var myPort = new SerialPort(portaUsb, {
    baudRate: 9600 ,
    parser: serialport.parsers.readline('\n')
});

myPort.on("open",(err)=>{
    console.log("Abriu connection com Arduino")
    setTimeout(()=>{
        myPort.write("wagner&");
    },5000)
})

//myPort.write("wagnern");

myPort.on("data",(data)=>{

    //myPort.write("wagner$");

    console.log(`Arduino enviando: ${data}`);

    //var buf = Buffer.from("wagner", 'utf8');

    //myPort.write(buf);

})