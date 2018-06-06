
const textoIncendio = `Detectamos um possível princípio de Incêndio. Se deseja ignorar essa MSG e 
desabilitar o alarme click no boão abaixo mais a direita.`

const textoLadrao = `Detectamos uma possível invasão a sua rezidencia. Se deseja ignorar essa MSG e 
desabilitar o alarme click no boão abaixo mais a direita.`

function main() {

    let boblioteca = [
        { key: "l3", value: 0, state: 0 },
        { key: "l4", value: 0, state: 0 },
        { key: "l5", value: 0, state: 0 },
        { key: "l6", value: 0, state: 0 },
        { key: "l7", value: 0, state: 0 },
    ]

    function construct() {

    }

}

$(document).ready(function () {

    //Classe que vai avisar se foi apertado quando o botao for apertado
    $(".apertou").click(function () {

        let dataValue = this.getAttribute("data-value")
        let idComponent = this.getAttribute("id")

        if (dataValue == 0) {
            $(this.firstElementChild).removeClass("desligado").addClass("ligado");
            $(this).attr("data-value", "1");
        } else {
            $(this.firstElementChild).removeClass("ligado").addClass("desligado");
            $(this).attr("data-value", "0");
        }

        let dataValueAgora = dataValue == 0 ? 1 : 0;

        socket.emit('msg', JSON.stringify({ msg: `${idComponent}${dataValueAgora}`, token: $.cookie("token") }));

        console.log(this.getAttribute("id"))
        console.log(this.getAttribute("data-value"))

        //$(this.firstElementChild).removeClass( "desligado" ).addClass( "ligado" );

    })

    $(".gg").click(function () {
        let value = confirm ("Realmente deseja cancelar o alarme")

        if(value == 1){
            socket.emit('msg', JSON.stringify({ msg: this.getAttribute("id")+"0", token: $.cookie("token") }));
        }

    })

});