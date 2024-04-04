<?php



include_once "database.php"; // importo el archivo que ejecuta las sentencias sql

// la linea 13 es lo mismo que hacer:
// if (isset($_POST['user_dni'])) {
//     $dni = $_POST['user_dni'];
// } else {
//     $dni = '';
// }

try{
    $dni = isset($_POST['user_dni']) ? $_POST['user_dni'] : ''; // recibe el dni escaneado en el GM65 conectado a la ESP32
    } catch (Exception $e){
    echo "100";
}
if ($dni == ""){
    echo "2 (No recibio ningun dni)";
} else {
    $conexion = conexion(); // la variable recibe el retorno de la funcion 'conexion()'

    $resultado_producto = comp_dni($dni, $conexion); // la funcion consulta si el dni enviado está registrado en la db
    if ($resultado_producto) {  // se revisa si hubo un resultado válido
        echo "1"; // en caso de que el dni esté registrado, se muestra un '1'
    } else {
        echo "0"; // en caso de que el dni no esté registrado, se muestra un '0'
    }
}
// se muestra el numero del resultado, para que la ESP32 pueda seguir adelante con su codigo en base a lo que se mostró


?>