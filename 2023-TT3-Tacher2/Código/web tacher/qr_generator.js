const codigoqr = () => {
    const contenedorQR = document.getElementById('contenedorQR');

    const QR = new QRCode(contenedorQR);


    QR.makeCode(dni);
};

codigoqr()

document.addEventListener('DOMContentLoaded', (event) => {
    const myModal = document.getElementById('myModal');
    const myInput = document.getElementById('myInput');
    
    if (myModal) {
        myModal.addEventListener('shown.bs.modal', () => {
            myInput.focus();
        });
    }
});
